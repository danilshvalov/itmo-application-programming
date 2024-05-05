#pragma once

#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>

#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>

/*
    The certificate was generated from bash on Ubuntu (OpenSSL 1.1.1f) using:

    openssl dhparam -out dh.pem 2048
    openssl req -newkey rsa:2048 -nodes -keyout key.pem -x509 -days 10000 -out
   cert.pem -subj "/C=US/ST=CA/L=Los Angeles/O=Beast/CN=www.example.com"
*/

void LoadCertificate(boost::asio::ssl::context& ctx,
                     const std::string& cert_filename,
                     const std::string& key_filename,
                     const std::string& dh_filename,
                     const std::string& password);
