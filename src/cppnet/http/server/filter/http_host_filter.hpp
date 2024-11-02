#pragma once

#include "http_filter.hpp"
#include <regex>
#include <string>
namespace cppnet {

class HttpHostFilter : public HttpFilter {
public:
  HttpHostFilter() = default;

  inline void Init(const std::string &regex_host) { host_ = regex_host; }

  inline virtual bool IsMatchFilter(HttpReq &req) override {
    // regex compare
    return std::regex_match(req.header().GetHost(), host_);
  }

private:
  std::regex host_;
};

} // namespace cppnet
