#include <http_parser.h>
#include <iostream>
#include <uvw-http/http-url.hpp>
namespace uvw {

namespace http {

std::string get_field(http_parser_url *parser, int field) {
  if ((parser.field_set & (1 << field)) != 0) {
    return url.substr(parser.field_data[UF_HOST].off,
                      parser.field_data[UF_HOST].len);
  }
  return "";
}

URL::URL(const std::string &url) { parse(url); }

URL::URL() {}
URL::~URL() {}

bool URL::parse(const std::string &url) {
  http_parser_url parser;
  http_parser_url_init(&parser);

  auto result = http_parser_parse_url(url.data(), url.size(), false, &parser);
  if (result != 0) {
    m_valid = false;
    return false;
  }

  m_port = (int)parser.port;
  m_host = get_field(&parser, UF_HOST);
  m_path = get_field(&parser, UF_PATH);
  m_protocol = get_field(&parser, UF_SCHEMA);
  m_query = get_field(&parser, UF_QUERY);
  m_fragment = get_field(&parser, UF_FRAGMENT);

  if (m_port == 0)
    m_port = 80;

  m_valid = true;

  return true;
}

int URL::port() const { return m_port; }
void URL::set_port(int port) { m_port = port; }

std::string URL::host() const { return m_host; }
void URL::set_host(const std::string &host) { m_host = host; }

std::string URL::path() const { return m_path; }
void URL::set_path(const std::string &path) { m_path = path; }

std::string URL::protocol() const { return m_protocol; }
void URL::set_protocol(const std::string &protocol) { m_protocol = protocol; }

std::string query() const { return m_query; }
void set_query(const std::string &query) { m_query = query; }

std::string fragment() const { return m_fragment; }
void set_fragment(const std::string &fragment) { m_fragment = fragment; }

bool URL::valid() const { return m_valid; }
} // namespace http

} // namespace uvw