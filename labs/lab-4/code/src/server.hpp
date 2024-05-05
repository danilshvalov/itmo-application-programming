#include <atomic>

#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/ssl.hpp>

class Server {
 public:
  Server();

  void Start(const boost::asio::ip::address& address, uint16_t port,
             const std::string& cert_filename, const std::string& key_filename,
             const std::string& dh_filename, const std::string& password);

  void Stop();

 private:
  boost::asio::io_context ioc_{1};
  boost::asio::ssl::context ctx_;
  std::atomic_flag running_ = false;
};
