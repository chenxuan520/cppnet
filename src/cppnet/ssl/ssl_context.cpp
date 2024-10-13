#ifdef CPPNET_OPENSSL
#include "ssl_context.hpp"
#include "../utils/const.hpp"

#include <memory>
#include <openssl/err.h>
#include <string>

namespace cppnet {

SSLContext::~SSLContext() { Close(); }

int SSLContext::Init(SSL_CTX *ctx) {
  if (ctx == nullptr) {
    err_msg_ = "[loginerr]:ssl context is null";
    return kLogicErr;
  }
  ssl_ctx_ = ctx;
  return kSuccess;
}

int SSLContext::Init(const std::string &cert_data, const std::string &key_data,
                     const std::string &password) {
  if (ssl_ctx_ != nullptr) {
    err_msg_ = "[loginerr]:ssl context is not null";
    return kLogicErr;
  }
  if (cert_data.empty() || key_data.empty()) {
    err_msg_ = "[loginerr]:cert or key data is empty";
    return kLogicErr;
  }

  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == nullptr) {
    err_msg_ = "[syserr]:create ssl context failed";
    return kSysErr;
  }

  BIO *bio = BIO_new_mem_buf((void *)cert_data.c_str(), cert_data.size());
  if (bio == nullptr) {
    err_msg_ = "[syserr]:create bio failed";
    return kSysErr;
  }

  X509 *cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
  if (cert == nullptr) {
    err_msg_ = "[syserr]:read certificate failed";
    return kSysErr;
  }

  if (SSL_CTX_use_certificate(ctx, cert) <= 0) {
    err_msg_ = "[syserr]:use certificate failed";
    return kSysErr;
  }

  if (!password.empty()) {
    SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)password.c_str());
  }
  return kSuccess;
}

int SSLContext::InitFile(const std::string &cert_path,
                         const std::string &key_path,
                         const std::string &password) {
  if (ssl_ctx_ != nullptr) {
    err_msg_ = "[loginerr]:ssl context is not null";
    return kLogicErr;
  }
  if (cert_path.empty() || key_path.empty()) {
    err_msg_ = "[loginerr]:cert or key path is empty";
    return kLogicErr;
  }

  SSL_CTX *ctx = SSL_CTX_new(TLS_server_method());
  if (ctx == nullptr) {
    err_msg_ = "[syserr]:create ssl context failed";
    return kSysErr;
  }

  if (SSL_CTX_use_certificate_file(ctx, cert_path.c_str(), SSL_FILETYPE_PEM) <=
      0) {
    err_msg_ = "[syserr]:use certificate file failed";
    return kSysErr;
  }

  if (!password.empty()) {
    SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)password.c_str());
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, key_path.c_str(), SSL_FILETYPE_PEM) <=
      0) {
    err_msg_ = "[syserr]:use private key file failed";
    return kSysErr;
  }

  return kSuccess;
}

std::shared_ptr<SSLSocket> SSLContext::CreateSSLSocket() {
  if (ssl_ctx_ == nullptr) {
    err_msg_ = "[loginerr]:ssl context is null";
    return nullptr;
  }

  SSL *ssl = SSL_new(ssl_ctx_);
  if (ssl == nullptr) {
    err_msg_ = "[syserr]:create ssl failed";
    return nullptr;
  }

  return std::make_shared<SSLSocket>(ssl);
}

} // namespace cppnet

#endif
