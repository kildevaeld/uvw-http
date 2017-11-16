#pragma once
#include <map>
#include <string>

namespace uvw {

namespace http {

enum Method { Get = 1, Post, Put, Patch, Delete, Option, Head };

struct CaseInsensitiveCompare {
  bool operator()(const std::string &a, const std::string &b) const noexcept;
};

using Header = std::map<std::string, std::string, CaseInsensitiveCompare>;

struct HttpRequestOptions {
  Method method = Get;
  std::string path;
  unsigned int port;
  std::string hostname;
  Header header;
  std::string data;
};

} // namespace http

} // namespace uvw