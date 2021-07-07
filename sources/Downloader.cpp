// Copyright 2020 qDerezzed
#include "Downloader.h"

Downloader::Downloader(std::string host, std::string port)
    : host_(std::move(host)), port_(std::move(port)) {

  load_root_certificates(sslContext);

  if (!SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str())) {
    boost::system::error_code ec{static_cast<int>(::ERR_get_error()),
                                 boost::asio::error::get_ssl_category()};
    throw boost::system::system_error{ec};
  }

  resolverResults = resolver.resolve(host_, port_);
}

std::string Downloader::download(const std::string &url, int httpVersion) {

  boost::asio::connect(stream.next_layer(), resolverResults.begin(),
                       resolverResults.end());
  stream.handshake(ssl::stream_base::client);

  boost::beast::http::request<boost::beast::http::string_body> request{
      boost::beast::http::verb::get, url, httpVersion};
  request.set(boost::beast::http::field::host, host_);
  request.set(boost::beast::http::field::user_agent,
              BOOST_BEAST_VERSION_STRING);

  boost::beast::flat_buffer buffer;
  boost::beast::http::response<boost::beast::http::dynamic_body> response;

  boost::beast::http::write(stream, request);
  boost::beast::http::read(stream, buffer, response);

  return boost::beast::buffers_to_string(response.body().data());
}
