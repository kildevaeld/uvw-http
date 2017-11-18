#include <iostream>
#include <uvw-http/http-client.hpp>
#include <uvw-http/http-url.hpp>
using namespace uvw::http;

int main() {
  auto loop = uvw::Loop::getDefault();

  HttpRequestOptions req(Post, URL("http://localhost:5000/auth/login"));
  req.header["Content-Type"] = "application/json";
  req.data = "{\"email\":\"test@gmail.com\",\"password\":\"password\"}";

  auto client = new HttpClient(loop, req);

  client->once<uvw::ErrorEvent>([](const auto &event, auto &) {
    std::cout << event.what() << std::endl;
  });

  client->on<uvw::DataEvent>([](const auto &event, auto &) {
    std::cout << std::string(event.data.get(), event.length) << std::endl;
  });

  client->on<uvw::CloseEvent>(
      [](const auto &event, auto &) { std::cout << "Close" << std::endl; });

  client->connect();

  loop->run();
  /*Header header;
  header["Content-Type"] = "application/json";

  auto request = new uvw::http::HttpClient(
      loop,
      uvw::http::HttpRequestOptions{
          .method = Post,
          .path = "/auth/login",
          .port = 5000,
          .hostname = "127.0.0.1",
          .header = header,
          .data = "{\"email\":\"test@gmail.com\", \"password\":\"password\"}"});

  request->once<HeaderEvent>([](auto &event, auto &reader) {
    std::cout << "Header event: " << event.status_code << std::endl;
    for (auto &kv : event.header) {
      std::cout << kv.first << " " << kv.second << "\n";
    }
    std::cout << std::endl;
  });

  request->once<uvw::DataEvent>([](auto &event, auto &reader) {z
    std::cout << std::string(event.data.get(), event.length) << std::endl;
  });

  request->connect();

  loop->run();*/

  return 0;
}