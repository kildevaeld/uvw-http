#pragma once
#include "http-url.hpp"
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

public:
  HttpRequestOptions() {}
  HttpRequestOptions(Method m, const URL &u, const Header &h = Header())
      : method(m), url(u), header(h) {}

  Method method = Get;
  URL url;
  Header header;
  std::string data;
};

} // namespace http

} // namespace uvw