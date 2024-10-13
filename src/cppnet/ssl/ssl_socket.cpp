#ifdef CPPNET_OPENSSL

#include "ssl_socket.hpp"
#include "utils/const.hpp"

namespace cppnet {

int SSLSocket::BindSoc(const Socket &soc) {
  if (ssl_ == nullptr) {
    err_msg_ = "[loginerr]:ssl is nullptr";
    return kLogicErr;
  }
  fd_ = soc.fd();
  SSL_set_fd(ssl_, fd_);
  SSL_accept(ssl_);
  return kSuccess;
}

void SSLSocket::CloseSSL() {
  if (ssl_ != nullptr) {
    SSL_free(ssl_);
    ssl_ = nullptr;
  }
}

} // namespace cppnet

#endif
