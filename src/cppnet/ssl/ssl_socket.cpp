#include "openssl/err.h"
#include "openssl/ssl.h"
#ifdef CPPNET_OPENSSL

#include "ssl_socket.hpp"
#include "utils/const.hpp"

namespace cppnet {

SSLSocket::SSLSocket(SSL *ssl, const Socket &soc) {
  if (ssl == nullptr) {
    err_msg_ = "[loginerr]:ssl is nullptr";
    return;
  }
  if (soc.status() != kInit) {
    err_msg_ = "[loginerr]:socket fd is invalid";
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
