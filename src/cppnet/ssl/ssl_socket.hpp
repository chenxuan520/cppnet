#pragma once

#include "utils/const.hpp"
#include <memory>
#ifdef CPPNET_OPENSSL

#include "../socket/socket.hpp"
#include <openssl/ssl.h>
#include <string>
namespace cppnet {

class SSLSocket : public Socket {
public:
  SSLSocket(SSL *ssl, SSL_CTX *ssl_ctx, const Socket &soc);
  SSLSocket(const SSLSocket &) = delete;
  /**
   * @brief: connect socket
   */
  int Connect(Address &addr) override;
  /**
   * @brief: Accept socket ssl.
   */
  std::shared_ptr<SSLSocket> AcceptSSL(Address &addr);
  /**
   * @brief: close ssl
   */
  int Close() override;

public:
  inline std::string err_msg() const { return err_msg_; }

protected:
  /**
   * @brief: Accept socket, return err, use AcceptSSL
   */
  Socket Accept(Address &addr) override {
    err_msg_ = "[loginerr]:ssl socket can not accept,use AcceptSSL";
    return kLogicErr;
  }

protected:
  inline int IORead(void *buf, size_t len) override {
    return SSL_read(ssl_, buf, len);
  }

  inline int IOWrite(const void *buf, size_t len) override {
    return SSL_write(ssl_, buf, len);
  }

private:
  SSL *ssl_ = nullptr;
  SSL_CTX *ssl_ctx_ = nullptr;
  std::string err_msg_;
};

} // namespace cppnet

#endif
