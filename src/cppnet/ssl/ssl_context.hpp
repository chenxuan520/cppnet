#pragma once
#ifdef CPPNET_OPENSSL
#include <openssl/ssl.h>

namespace cppnet {

class SSLContext {
public:
private:
  SSL_CTX *ssl_ctx_;
};

#endif

} // namespace cppnet
