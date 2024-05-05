#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

#include "math.hpp"
#include "models.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;

void LogError(beast::error_code ec, char const* what) {
  std::cerr << what << ": " << ec.message() << "\n";
}

template <class Body, class Allocator>
http::message_generator HandleRequest(
    http::request<Body, http::basic_fields<Allocator>>&& request) {
  RequestData request_data;
  try {
    request_data = json::value_to<RequestData>(json::parse(request.body()));
  } catch (const std::exception& e) {
    ErrorData response_data{
        static_cast<int>(http::status::bad_request),
        "Failed to parse request data",
    };
    http::response<http::string_body> response{
        http::status::bad_request,
        request.version(),
    };
    response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
    response.set(http::field::content_type, "text/json");
    response.body() = json::serialize(json::value_from(response_data));
    response.keep_alive(request.keep_alive());
    response.prepare_payload();
    return response;
  }

  Coefficients coefficients = FindCoefficients(request_data.points);
  std::vector<Point> calculated_points =
      FindCalculatedPoints(request_data.points, coefficients);
  ResponseData response_data{
      std::move(coefficients),
      std::move(calculated_points),
  };

  http::response<http::string_body> response{
      http::status::ok,
      request.version(),
  };
  response.set(http::field::server, BOOST_BEAST_VERSION_STRING);
  response.set(http::field::content_type, "text/json");
  response.body() = json::serialize(json::value_from(response_data));
  response.keep_alive(request.keep_alive());
  response.prepare_payload();
  return response;
}

void DoSession(tcp::socket& socket) {
  beast::error_code ec;
  beast::flat_buffer buffer;

  for (;;) {
    http::request<http::string_body> req;
    http::read(socket, buffer, req, ec);
    if (ec == http::error::end_of_stream) {
      break;
    }
    if (ec) {
      return LogError(ec, "read");
    }

    http::message_generator msg = HandleRequest(std::move(req));
    bool keep_alive = msg.keep_alive();
    beast::write(socket, std::move(msg), ec);

    if (ec) {
      return LogError(ec, "write");
    }
    if (!keep_alive) {
      break;
    }
  }

  socket.shutdown(tcp::socket::shutdown_send, ec);
}

int main(int argc, char* argv[]) {
  try {
    const auto address = net::ip::make_address("0.0.0.0");
    const uint16_t port = 8080;

    net::io_context ioc{1};
    tcp::acceptor acceptor{ioc, {address, port}};
    for (;;) {
      tcp::socket socket{ioc};
      acceptor.accept(socket);
      std::thread{std::bind(&DoSession, std::move(socket))}.detach();
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }
}
