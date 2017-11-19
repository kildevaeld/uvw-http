#pragma once
#include "http-request.hpp"
#include <memory>
#include <uvw/emitter.hpp>
#include <uvw/loop.hpp>
#include <uvw/stream.hpp>

namespace uvw {

namespace http {

namespace internal {
class HttpClientPrivate;
}

struct HeaderEvent {
  HeaderEvent(unsigned int status, const Header &h)
      : header(h), status_code(status) {}
  Header header;
  unsigned int status_code;
};

class HttpClient : public Emitter<HttpClient> {

public:
  HttpClient(std::shared_ptr<uvw::Loop> loop, const HttpRequestOptions &req);
  HttpClient(const HttpClient &other) = delete;
  ~HttpClient();

  void connect();
  void close();


  uvw::Loop *loop() const;

  void deleteLater();

  static std::shared_ptr<HttpClient> get(const std::string &url,
                                         const Header &header = Header());
  static std::shared_ptr<HttpClient>
  post(const std::string &url, const Header &header = Header(),
       const std::string body = std::string(""));

private:
  friend class internal::HttpClientPrivate;
  std::unique_ptr<internal::HttpClientPrivate> d;
};

} // namespace http

} // namespace uvw