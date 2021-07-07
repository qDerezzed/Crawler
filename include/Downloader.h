// Copyright 2020 qDerezzed
#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <string>

#include "RootCertificates.h"

class Downloader {
 public:
  Downloader(std::string host, std::string port);

  [[nodiscard]] std::string getHost() const { return host_; }

  [[nodiscard]] std::string getPort() const { return port_; }

  std::string download(const std::string &url, int httpVersion = 10);

 private:
  std::string host_;
  std::string port_;

  boost::asio::io_context ioContext;
  boost::asio::ssl::context sslContext{
      boost::asio::ssl::context::sslv23_client};

  boost::asio::ip::tcp::resolver resolver{ioContext};
  boost::asio::ssl::stream<boost::asio::ip::tcp::socket> stream{ioContext,
                                                                sslContext};

  boost::asio::ip::tcp::resolver::results_type resolverResults;
};
