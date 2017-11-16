#pragma once
#include <string>

namespace uvw {

namespace http {

class URL {

public:
  URL(const std::string &url);
  URL();
  ~URL();

  int port() const;
  void set_port(int port);

  std::string host() const;
  void set_host(const std::string &host);

  std::string path() const;
  void set_path(const std::string &path);

  std::string protocol() const;
  void set_protocol(const std::string &protocol);

  bool valid() const;

private:
  std::string m_host;
  int m_port;
  std::string m_path;
  std::string m_protocol;
  bool m_valid = false;
};

} // namespace http

} // namespace uvw