#pragma once

#include "../../req/method/http_method.hpp"
#include "http_filter.hpp"
namespace cppnet {

class HttpMethodFilter : public HttpFilter {
public:
  HttpMethodFilter() = default;
  void Init(const HttpMethod method) { method_ = method; }

  inline virtual bool IsMatchFilter(HttpReq &req) override {
    return req.method() == method_;
  }

private:
  HttpMethod method_;
};

} // namespace cppnet
