#include "openssl/err.h"
#include "openssl/ssl.h"
#ifdef CPPNET_OPENSSL

#include "ssl_socket.hpp"
#include "utils/const.hpp"

namespace cppnet {

SSLSocket::SSLSocket(SSL *ssl, SSL_CTX *ssl_ctx, const Socket &soc) {
  if (ssl == nullptr) {
    err_msg_ = "[loginerr]:ssl is nullptr";
    return;
  }
  if (ssl_ctx == nullptr) {
    err_msg_ = "[loginerr]:ssl context is null";
    return;
  }
  if (soc.status() != kInit) {
    err_msg_ = "[loginerr]:socket fd is invalid";
    return;
  }
  fd_ = soc.fd();
  ssl_ = ssl;
  ssl_ctx_ = ssl_ctx;
  SSL_set_fd(ssl_, fd_);
  status_ = kInit;
}

int SSLSocket::Close() {
  if (ssl_ != nullptr) {
    ::close(fd_);
    SSL_shutdown(ssl_);
    SSL_free(ssl_);
    ssl_ = nullptr;
  }
  return kSuccess;
}

int SSLSocket::Connect(Address &addr) {
  if (ssl_ == nullptr || status_ != kInit) {
    err_msg_ = "[loginerr]:ssl is nullptr or socket fd is invalid";
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

} // namespace cppnet

#endif
