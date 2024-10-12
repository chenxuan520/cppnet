#pragma once
#include <memory>
#ifdef CPPNET_OPENSSL
#include "ssl_connect.hpp"
#include <openssl/ssl.h>

namespace cppnet {

class SSLContext {
public:
  SSLContext();
  ~SSLContext();
  int Init(const std::string &cert_path, const std::string &key_path,
           const std::string &password = "");
  std::shared_ptr<SSLConnect> CreateSSLConnect() const;

public:
  const std::string &err_msg() const { return err_msg_; }

private:
  SSL_CTX *ssl_ctx_ = nullptr;
  std::string err_msg_;
};

} // namespace cppnet

#endif
