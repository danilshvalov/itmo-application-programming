#include "server.hpp"

#include <algorithm>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "certificates.hpp"

namespace {

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace ssl = boost::asio::ssl;
using tcp = boost::asio::ip::tcp;

template <class Body, class Allocator>
http::message_generator HandleRequest(
    http::request<Body, http::basic_fields<Allocator>>&& req) {
  http::response<http::string_body> res{http::status::ok, req.version()};
  res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  // res.set(http::field::content_type, req[http::field::content_type]);
  res.body() = req.body();
  res.prepare_payload();
  res.keep_alive(req.keep_alive());
  return res;
}

void LogError(beast::error_code ec, char const* what) {
  if (ec == net::ssl::error::stream_truncated) {
    return;
  }
  std::cerr << what << ": " << ec.message() << "\n";
}

class Session : public std::enable_shared_from_this<Session> {
 public:
  explicit Session(tcp::socket&& socket, ssl::context& ctx)
      : stream_(std::move(socket), ctx) {}

  void Run() {
    net::dispatch(
        stream_.get_executor(),
        beast::bind_front_handler(&Session::OnRun, shared_from_this()));
  }

  void OnRun() {
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    stream_.async_handshake(
        ssl::stream_base::server,
        beast::bind_front_handler(&Session::OnHandshake, shared_from_this()));
  }

  void OnHandshake(beast::error_code ec) {
    if (ec) {
      return LogError(ec, "handshake");
    }
    DoRead();
  }

  void DoRead() {
    req_ = {};
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    http::async_read(
        stream_, buffer_, req_,
        beast::bind_front_handler(&Session::OnRead, shared_from_this()));
  }

  void OnRead(beast::error_code ec, std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec == http::error::end_of_stream) {
      return DoClose();
    }
    if (ec) {
      return LogError(ec, "read");
    }
    SendResponse(HandleRequest(std::move(req_)));
  }

  void SendResponse(http::message_generator&& msg) {
    bool keep_alive = msg.keep_alive();
    beast::async_write(
        stream_, std::move(msg),
        beast::bind_front_handler(&Session::OnWrite, this->shared_from_this(),
                                  keep_alive));
  }

  void OnWrite(bool keep_alive, beast::error_code ec,
               std::size_t bytes_transferred) {
    boost::ignore_unused(bytes_transferred);
    if (ec) {
      return LogError(ec, "write");
    }
    if (!keep_alive) {
      return DoClose();
    }
    DoRead();
  }

  void DoClose() {
    beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(30));
    stream_.async_shutdown(
        beast::bind_front_handler(&Session::OnShutdown, shared_from_this()));
  }

  void OnShutdown(beast::error_code ec) {
    if (ec) {
      return LogError(ec, "shutdown");
    }
  }

 private:
  beast::ssl_stream<beast::tcp_stream> stream_;
  beast::flat_buffer buffer_;
  http::request<http::string_body> req_;
};

class Listener : public std::enable_shared_from_this<Listener> {
 public:
  Listener(net::io_context& ioc, ssl::context& ctx, tcp::endpoint endpoint)
      : ioc_(ioc), ctx_(ctx), acceptor_(ioc) {
    beast::error_code ec;

    acceptor_.open(endpoint.protocol(), ec);
    if (ec) {
      LogError(ec, "open");
      return;
    }

    acceptor_.set_option(net::socket_base::reuse_address(true), ec);
    if (ec) {
      LogError(ec, "set_option");
      return;
    }

    acceptor_.bind(endpoint, ec);
    if (ec) {
      LogError(ec, "bind");
      return;
    }

    acceptor_.listen(net::socket_base::max_listen_connections, ec);
    if (ec) {
      LogError(ec, "listen");
      return;
    }
  }

  void Run() { DoAccept(); }

 private:
  void DoAccept() {
    acceptor_.async_accept(
        net::make_strand(ioc_),
        beast::bind_front_handler(&Listener::OnAccept, shared_from_this()));
  }

  void OnAccept(beast::error_code ec, tcp::socket socket) {
    if (ec) {
      LogError(ec, "accept");
      return;
    } else {
      std::make_shared<Session>(std::move(socket), ctx_)->Run();
    }
    DoAccept();
  }

  net::io_context& ioc_;
  ssl::context& ctx_;
  tcp::acceptor acceptor_;
};

}  // namespace

Server::Server() : ioc_(1), ctx_(ssl::context::tlsv12) {}

void Server::Start(const net::ip::address& address, uint16_t port,
                   const std::string& cert_filename,
                   const std::string& key_filename,
                   const std::string& dh_filename,
                   const std::string& password) {
  LoadCertificate(ctx_, cert_filename, key_filename, dh_filename, password);

  if (ioc_.stopped()) {
    ioc_.restart();
  }
  std::make_shared<Listener>(ioc_, ctx_, tcp::endpoint{address, port})->Run();
  std::thread([&] { ioc_.run(); }).detach();
}

void Server::Stop() { ioc_.stop(); }
