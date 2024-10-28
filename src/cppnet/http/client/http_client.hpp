#pragma once

#include "../../socket/socket.hpp"
#include "../req/http_req.hpp"
#include "../resp/http_resp.hpp"

namespace cppnet {

class HttpClient {
public:
  HttpClient() = default;
  ~HttpClient() { Close(); }
  /**
   * @brief: init client
   * @param addr: server address
   * @return: 0: success, -1: failed
   */
  int Init(Address &addr);
  /**
   * @brief: send request
   * @param req: http request
   * @param resp: http response
   * @return: 0: success, -1: failed
   */
  int Send(HttpReq &req, HttpResp &resp);

  /**
   * @brief: close client
   */
  void Close() { soc_.Close(); }

public:
  Socket &socket() { return soc_; }
  std::string err_msg() { return err_msg_; }

private:
  Socket soc_;
  std::string err_msg_;
};

} // namespace cppnet
