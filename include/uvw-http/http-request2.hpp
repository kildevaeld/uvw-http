#pragma once
#include <uvw/emitter.hpp>
#include <uvw/loop.hpp>

namespace uvw {

namespace http {

class HttpRequest : public BaseHandle, Emitter < HttpRequest {};

} // namespace http

} // namespace uvw