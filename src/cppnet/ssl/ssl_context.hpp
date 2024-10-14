#pragma once
#include "socket/socket.hpp"
#ifdef CPPNET_OPENSSL

#include "ssl_socket.hpp"
#include <memory>
#include <openssl/ssl.h>

namespace cppnet {

class SSLContext {
public:
  SSLContext() = default;
  ~SSLContext();
  // forbiden copy
  SSLContext(const SSLContext &) = delete;
  SSLContext &operator=(const SSLContext &) = delete;
  /**
   * @brief: init ssl context
   * @param: ctx: ssl context
   */
  int Init(SSL_CTX *ctx);
  /**
   * @brief: init ssl client context
   */
  int InitCli();
  /**
   * @brief: init ssl context
   * @param cert_data: cert data
   * @param key_data: key data
   * @param password: password
   */
  int Init(const std::string &cert_data, const std::string &key_data,
           const std::string &password = "");
  /**
   * @brief: init ssl context
   * @param cert_path: cert path file
   * @param key_path: key path file
   * @param password: password
   */
  int InitFile(const std::string &cert_path, const std::string &key_path,
               const std::string &password = "");
  /**
   * @brief: create ssl socket
   */
  std::shared_ptr<SSLSocket> CreateSSLSocket(const Socket &soc);
  std::shared_ptr<SSLSocket> CreateSSLSocket();
  /**
   * @brief: close ssl context
   */
  int Close();

public:
  const std::string &err_msg() const { return err_msg_; }

private:
  SSL_CTX *ssl_ctx_ = nullptr;
  std::string err_msg_;
};

} // namespace cppnet

#endif
