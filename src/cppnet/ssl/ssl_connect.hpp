#pragma once
#ifdef CPPNET_OPENSSL
#include "../socket/socket.hpp"
#include <openssl/ssl.h>

namespace cppnet {

class SSLConnect {
public:
  SSLConnect(SSL *ssl) : ssl_(ssl){};
  virtual ~SSLConnect();
  int BindSoc(const Socket &soc) const;
  int Read(void *buf, size_t len, bool complete = false) const;
  int Read(std::string &buf, size_t len, bool complete = false) const;
  int Write(const void *buf, size_t len) const;
  int Write(const std::string &buf) const;
  void Close();

private:
  SSL *ssl_ = nullptr;
};

} // namespace cppnet
#endif
