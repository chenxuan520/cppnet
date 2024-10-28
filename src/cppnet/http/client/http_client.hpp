#pragma once

#include "../../socket/socket.hpp"
#include "../req/http_req.hpp"
#include "../resp/http_resp.hpp"

namespace cppnet {

class HttpClient {
public:
  HttpClient() = default;
  ~HttpClient();
  /**
   * @brief: init client
   * @param addr: server address
   * @return: 0: success, -1: failed
   */
  int Init(Address &addr);
  /**
   * @brief: send request
   * @return: 0: success, -1: failed
   */
  int Send();

public:
  HttpResp &resp() { return resp_; }
  HttpReq &req() { return req_; }
  Socket &socket() { return soc_; }

private:
  HttpResp resp_;
  HttpReq req_;
  Socket soc_;
};

} // namespace cppnet
