#include <http_parser.h>
#include <iostream>
#include <uvw-http/http-url.hpp>
namespace uvw {

namespace http {

URL::URL(const std::string &url) {
  http_parser_url parser;
  http_parser_url_init(&parser);

  auto result = http_parser_parse_url(url.data(), url.size(), false, &parser);
  if (result != 0) {
    m_valid = false;
    return;
  }

  if ((parser.field_set & (1 << UF_HOST)) != 0) {
    std::cout << url.substr(parser.field_data[UF_HOST].off,
                            parser.field_data[UF_HOST].len)
              << std::endl;
  }
}

URL::URL() {}
URL::~URL() {}

int URL::port() const { return m_port; }
void URL::set_port(int port) { m_port = port; }

std::string URL::host() const { return m_host; }
void URL::set_host(const std::string &host) { m_host = host; }

std::string URL::path() const { return m_path; }
void URL::set_path(const std::string &path) { m_path = path; }

std::string URL::protocol() const { return m_protocol; }
void URL::set_protocol(const std::string &protocol) { m_protocol = protocol; }

bool URL::valid() const { return m_valid; }
} // namespace http

} // namespace uvw