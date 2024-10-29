#pragma once

#include "../../socket/socket.hpp"
#include "../req/http_req.hpp"
#include "../resp/http_resp.hpp"

#ifdef CPPNET_OPENSSL
#include "../../ssl/ssl_context.hpp"
#include <memory>
#endif

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
  void Close();

#ifdef CPPNET_OPENSSL
public:
  /**
   * @brief: init ssl client
   * @param addr: server address
   * @param ssl_context: ssl context
   * @return: 0: success, -1: failed
   */
  int InitSSL(Address &addr);

private:
  std::shared_ptr<SSLContext> ssl_context_;
#endif

public:
  Socket &socket() { return *soc_; }
  std::string err_msg() { return err_msg_; }

private:
  std::shared_ptr<Socket> soc_ = nullptr;
  std::string err_msg_;
};

} // namespace cppnet
