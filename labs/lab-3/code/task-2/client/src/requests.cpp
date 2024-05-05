#include "requests.hpp"

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
namespace json = boost::json;
using tcp = boost::asio::ip::tcp;

ResponseData SendRequest(const RequestData &request_data) {
  auto const host = "localhost";
  auto const port = "8080";
  auto const target = "/";
  int version = 11;

  net::io_context ioc;

  tcp::resolver resolver(ioc);
  beast::tcp_stream stream(ioc);

  auto const results = resolver.resolve(host, port);
  stream.connect(results);

  http::request<http::string_body> request{http::verb::get, target, version};
  request.set(http::field::host, host);
  request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  request.body() = json::serialize(json::value_from(request_data));
  request.prepare_payload();

  http::write(stream, request);
  beast::flat_buffer buffer;

  http::response<http::string_body> response;
  http::read(stream, buffer, response);

  auto response_data =
      json::value_to<ResponseData>(json::parse(response.body()));

  beast::error_code ec;
  stream.socket().shutdown(tcp::socket::shutdown_both, ec);

  if (ec && ec != beast::errc::not_connected) {
    throw beast::system_error{ec};
  }

  return response_data;
}
