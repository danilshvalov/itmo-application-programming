#include "certificates.hpp"

namespace {

std::string LoadFileContent(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    throw std::runtime_error("Cannot open file: " + filename);
  }
  return std::string{
      std::istreambuf_iterator<char>(file),
      std::istreambuf_iterator<char>(),
  };
}

}  // namespace

void LoadCertificate(boost::asio::ssl::context& ctx,
                     const std::string& cert_filename,
                     const std::string& key_filename,
                     const std::string& dh_filename,
                     const std::string& password) {
  const std::string cert = LoadFileContent(cert_filename);
  const std::string key = LoadFileContent(key_filename);
  const std::string dh = LoadFileContent(dh_filename);

  ctx.set_password_callback(
      [=](std::size_t, boost::asio::ssl::context_base::password_purpose) {
        return password;
      });

  ctx.set_options(boost::asio::ssl::context::default_workarounds |
                  boost::asio::ssl::context::no_sslv2 |
                  boost::asio::ssl::context::single_dh_use);

  ctx.use_certificate_chain(boost::asio::buffer(cert.data(), cert.size()));

  ctx.use_private_key(boost::asio::buffer(key.data(), key.size()),
                      boost::asio::ssl::context::file_format::pem);

  ctx.use_tmp_dh(boost::asio::buffer(dh.data(), dh.size()));
}
