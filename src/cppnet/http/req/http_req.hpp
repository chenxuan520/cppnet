#pragma once
#include "../header/http_header.hpp"
#include "../version/http_version.hpp"
#include "method/http_method.hpp"
#include "route/http_route.hpp"

namespace cppnet {

class HttpReq {
public:
  HttpReq() = default;
  /**
   * @brief: parse http request
   * @param origin_req: origin http request
   * @return: 0: success, -1: invalid http request
   */
  int Parse(const std::string &origin_req);

public:
  HttpHeader &header() { return header_; }
  HttpRoute &route() { return route_; }
  HttpMethod &method() { return method_; }
  HttpVersion &version() { return version_; }
  std::string &body() { return body_; }
  std::string err_msg() { return err_msg_; }

private:
  HttpHeader header_;
  HttpRoute route_;
  HttpMethod method_;
  HttpVersion version_;
  std::string body_;
  std::string err_msg_;
};

} // namespace cppnet
