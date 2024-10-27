#pragma once

#include "http_filter.hpp"
#include <string>
namespace cppnet {

class HttpHostFilter : public HttpFilter {
public:
  HttpHostFilter() = default;

  inline void Init(const std::string &host) { host_ = host; }

  inline virtual bool IsMatchFilter(HttpReq &req) override {
    return req.header().GetHost() == host_;
  }

private:
  std::string host_;
};

} // namespace cppnet
