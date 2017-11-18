#include <uvw-http/http-request.hpp>

namespace uvw {

namespace http {

bool CaseInsensitiveCompare::operator()(const std::string &a,
                                        const std::string &b) const noexcept {
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(),
                                      [](unsigned char ac, unsigned char bc) {
                                        return std::tolower(ac) <
                                               std::tolower(bc);
                                      });
}

HttpRequestOptions::HttpRequestOptions(Method m, const URL &u, const Header &h)
    : method(m), url(u), header(h) {}

} // namespace http

} // namespace uvw