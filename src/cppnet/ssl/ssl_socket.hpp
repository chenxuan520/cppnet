#pragma once

#ifdef CPPNET_OPENSSL

#include "../socket/socket.hpp"
#include <openssl/ssl.h>
#include <string>
namespace cppnet {

class SSLSocket : public Socket {
public:
  SSLSocket(SSL *ssl) : ssl_(ssl){};
  /**
   * @brief: Bind socket.
   * @param soc: socket to bind.
   */
  int BindSoc(const Socket &soc);
  /**
   * @brief: close ssl
   */
  void CloseSSL();

protected:
  inline int IORead(void *buf, size_t len) const override {
    return SSL_read(ssl_, buf, len);
  }

  inline int IOWrite(const void *buf, size_t len) const override {
    return SSL_write(ssl_, buf, len);
  }

private:
  SSL *ssl_ = nullptr;
  std::string err_msg_;
};

} // namespace cppnet

#endif
