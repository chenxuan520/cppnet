#pragma once

#include "../../../utils/string.hpp"
#include "http_filter.hpp"
#include <regex>
#include <string>
namespace cppnet {

class HttpHostFilter : public HttpFilter {
public:
  HttpHostFilter() = default;

  inline void Init(const std::string &regex_host) { host_ = regex_host; }
  inline void InitWildCard(const std::string &wild_card) {
    wild_card_ = wild_card;
  }

  inline virtual bool IsMatchFilter(HttpReq &req) override {
    // regex compare
    return std::regex_match(req.header().GetHost(), host_) ||
           StringUtil::WildCardMatch(wild_card_, req.header().GetHost());
  }

private:
  std::regex host_;
  std::string wild_card_;
};

} // namespace cppnet
