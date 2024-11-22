#ifdef CPPNET_OPENSSL
#include "openssl/err.h"
#include "openssl/ssl.h"

#include "../utils/const.hpp"
#include "ssl_socket.hpp"

namespace cppnet {

SSLSocket::SSLSocket(SSL *ssl, const Socket &soc) {
  if (ssl == nullptr) {
    err_msg_ = "[logicerr]:ssl is nullptr";
    return;
  }
  if (soc.status() != kInit) {
    err_msg_ = "[logicerr]:socket fd is invalid";
    return;
  }
  fd_ = soc.fd();
  ssl_ = ssl;
  SSL_set_fd(ssl_, fd_);
  status_ = kInit;
}

int SSLSocket::Close() {
  if (status_ != kInit) {
    err_msg_ = "[logicerr]:socket status is invalid";
    return kLogicErr;
  }
  Socket::Close();
  CloseSSL();
  return kSuccess;
}

int SSLSocket::CloseSSL() {
  if (ssl_ != nullptr) {
    SSL_shutdown(ssl_);
    SSL_free(ssl_);
    ssl_ = nullptr;
  }
  return kSuccess;
}

int SSLSocket::Connect(Address &addr) {
  if (ssl_ == nullptr || status_ != kInit) {
    err_msg_ = "[logicerr]:ssl is nullptr or socket fd is invalid";
    return kLogicErr;
  }
  auto rc = ::connect(fd_, addr.GetSockAddr(), *addr.GetAddrLen());
  if (rc < 0) {
    err_msg_ = "[syserr]:" + std::string(strerror(errno));
    return kSysErr;
  }
  rc = SSL_connect(ssl_);
  if (rc <= 0) {
    err_msg_ = std::string("[syserr]:ssl connect failed") +
               ERR_error_string(ERR_get_error(), nullptr);
    return kSysErr;
  }
  return kSuccess;
}

int SSLSocket::IORead(void *buf, size_t len, int flag) {
  if (flag == MSG_WAITALL) {
    auto rc = 0;
    auto total_len = 0;
    while (total_len < len) {
      rc = SSL_read(ssl_, (char *)buf + total_len, len - total_len);
      if (rc <= 0) {
        err_msg_ = std::string("[syserr]:ssl complete read failed") +
                   ERR_error_string(ERR_get_error(), nullptr);
        return kSysErr;
      }
      total_len += rc;
    }
    return total_len;
  } else {
    return SSL_read(ssl_, buf, len);
  }
}

int SSLSocket::IOWrite(const void *buf, size_t len, int flag) {
  return SSL_write(ssl_, buf, len);
}

} // namespace cppnet

#endif
