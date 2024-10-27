#pragma once

#include "../../req/http_req.hpp"

namespace cppnet {

class HttpFilter {
public:
  HttpFilter() = default;
  virtual ~HttpFilter() = default;

  /**
   * @brief: check if filter match this req
   * @param req: http request
   * @return: true: match filter, false: not match filter
   */
  virtual bool IsMatchFilter(HttpReq &req) = 0;
};

} // namespace cppnet
