#include <http_parser.h>
#include <iostream>
#include <sstream>
#include <uvw-http/http-client.hpp>
#include <uvw/async.hpp>
#include <uvw/tcp.hpp>
#include <cstring>

namespace uvw {

namespace http {

namespace internal {
class HttpClientPrivate {

public:
  HttpClientPrivate(HttpClient *cl, std::shared_ptr<uvw::Loop> l,
                    const HttpRequestOptions &r)
      : loop(l), req(r), handle(loop->resource<uvw::TcpHandle>()), client(cl) {

    http_parser_init(&parser, HTTP_RESPONSE);
    http_parser_settings_init(&settings);

    parser.data = this;
    settings.on_header_field = on_header_field;
    settings.on_header_value = on_header_value;
    settings.on_body = on_body;
    settings.on_message_begin = on_message_begin;
    settings.on_message_complete = on_message_complete;
    settings.on_headers_complete = on_headers_complete;
  }
  ~HttpClientPrivate() {}

  std::string create_request() {
    std::stringstream stream;

    switch (req.method) {
    case Get:
      stream << "GET";
      break;
    case Post:
      stream << "POST";
      break;
    case Put:
      stream << "PUT";
      break;
    case Patch:
      stream << "PATCH";
      break;
    case Head:
      stream << "HEAD";
      break;
    case Delete:
      stream << "DELETE";
      break;
    case Option:
      stream << "OPTION";
      break;
    default:
      throw "Invalid method";
    }

    stream << " " << req.url.path() << " HTTP/1.1\r\n";

    for (auto kv : req.header) {
      stream << kv.first << ": " << kv.second << "\r\n";
    }

    if ((req.method == Post || req.method == Put || req.method == Patch) &&
        req.data.size() > 0) {
      stream << "Content-Length: " << req.data.size() << "\r\n";
    }

    stream << "\r\n";

    if ((req.method == Post || req.method == Put || req.method == Patch) &&
        req.data.size() > 0) {
      stream << req.data;
    }

    return stream.str();
  }

private:
  static int on_headers_complete(http_parser *parser) {
    auto req = static_cast<HttpClientPrivate *>(parser->data);
    req->client->publish(HeaderEvent(parser->status_code, req->header));
    return 0;
  }

  static int on_message_begin(http_parser *parser) { return 0; }

  static int on_message_complete(http_parser *parser) {
    auto req = static_cast<HttpClientPrivate *>(parser->data);
    req->handle->close();
    return 0;
  }

  static int on_header_field(http_parser *parser, const char *data,
                             size_t len) {
    auto req = static_cast<HttpClientPrivate *>(parser->data);
    req->current_field = std::string(data, len);
    return 0;
  }

  static int on_header_value(http_parser *parser, const char *data,
                             size_t len) {
    auto req = static_cast<HttpClientPrivate *>(parser->data);
    req->header[req->current_field] = std::string(data, len);
    return 0;
  }

  static int on_body(http_parser *parser, const char *data, size_t len) {
    auto req = static_cast<HttpClientPrivate *>(parser->data);
    auto chunk = new char[len];
    std::memcpy(chunk, data, len);
    req->client->publish(DataEvent(std::unique_ptr<char[]>(chunk), len));
    return 0;
  }

public:
  std::shared_ptr<uvw::Loop> loop;
  HttpRequestOptions req;
  std::shared_ptr<uvw::TcpHandle> handle;
  http_parser parser;
  http_parser_settings settings;
  HttpClient *client;
  // response
  Header header;
  std::string current_field;
};

} // namespace internal

HttpClient::HttpClient(std::shared_ptr<uvw::Loop> loop,
                       const HttpRequestOptions &req)
    : d(new internal::HttpClientPrivate(this, std::move(loop), req)) {}

void HttpClient::connect() {

  d->handle->once<uvw::ConnectEvent>([this](auto &event, auto &tcp) {
    auto request = this->d->create_request();
    tcp.write((char *)request.data(), request.size());

  });

  d->handle->once<uvw::ErrorEvent>(
      [this](auto &event, auto &tcp) { this->publish(event); });

  d->handle->once<uvw::WriteEvent>([](auto &event, auto &tcp) { tcp.read(); });

  d->handle->on<uvw::DataEvent>([this](auto &event, auto &tcp) {
    auto nparsed = http_parser_execute(&d->parser, &d->settings,
                                       event.data.get(), event.length);

    if (nparsed != event.length) {
      tcp.close();
    }

  });

  d->handle->once<uvw::CloseEvent>(
      [this](auto &e, auto &) { this->publish(e); });

  d->handle->connect(d->req.url.host(), d->req.url.port());
}

void HttpClient::close() { d->handle->close(); }

uvw::Loop *HttpClient::loop() const {
  return d->loop.get();
}

HttpClient::~HttpClient() {
  d->handle->close();
}

void HttpClient::deleteLater() {
  auto handle = d->loop->resource<uvw::AsyncHandle>();

  handle->on<uvw::AsyncEvent>([this](const auto &, auto &hndl) {
    delete this;
    hndl.close();
  });

  handle->send();
}

std::shared_ptr<HttpClient> HttpClient::get(const std::string &url,
                                            const Header &header) {}
std::shared_ptr<HttpClient> HttpClient::post(const std::string &url,
                                             const Header &header,
                                             const std::string body) {}

} // namespace http

} // namespace uvw