#pragma once

#ifdef CPPNET_OPENSSL

#include "../socket/socket.hpp"
#include <openssl/ssl.h>
#include <string>
namespace cppnet {

class SSLSocket : public Socket {
public:
  SSLSocket(SSL *ssl, const Socket &soc);
  ~SSLSocket() = default;
  SSLSocket(const SSLSocket &) = delete;
  SSLSocket &operator=(const SSLSocket &) = delete;
  /**
   * @brief: connect socket
   */
  int Connect(Address &addr) override;
  /**
   * @brief: close ssl and socket
   */
  int Close() override;
  /**
   * @brief: close ssl
   */
  int CloseSSL();

public:
  inline std::string err_msg() const { return err_msg_; }

protected:
  inline int IORead(void *buf, size_t len, int flag = 0) override {
    return SSL_read(ssl_, buf, len);
  }

  inline int IOWrite(const void *buf, size_t len, int flag = 0) override {
    return SSL_write(ssl_, buf, len);
  }

private:
  SSL *ssl_ = nullptr;
  std::string err_msg_;
};

} // namespace cppnet

#endif
