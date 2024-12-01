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

int SSLContext::InitCli() {
  if (ssl_ctx_ != nullptr) {
    err_msg_ = "[loginerr]:ssl context is not null";
    return kLogicErr;
  }

  const SSL_METHOD *meth = SSLv23_client_method();
  if (meth == nullptr) {
    err_msg_ = "[syserr]:create ssl method failed";
    return kSysErr;
  }
  SSL_CTX *ctx = SSL_CTX_new(meth);
  if (ctx == nullptr) {
    err_msg_ = "[syserr]:create ssl context failed";
    return kSysErr;
  }

  return Init(ctx);
}

int SSLContext::InitSvr(const std::string &cert_data,
                        const std::string &key_data,
                        const std::string &password) {
  if (ssl_ctx_ != nullptr) {
    err_msg_ = "[loginerr]:ssl context is not null";
    return kLogicErr;
  }
  if (cert_data.empty() || key_data.empty()) {
    err_msg_ = "[loginerr]:cert or key data is empty";
    return kLogicErr;
  }

  SSL_load_error_strings();
  SSL_library_init();
  OPENSSL_init_ssl(
      OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);

  SSL_CTX *ctx = SSL_CTX_new(TLS_method());
  if (ctx == nullptr) {
    err_msg_ = "[syserr]:create ssl context failed ";
    err_msg_ += ERR_error_string(ERR_get_error(), nullptr);
    ERR_clear_error();
    return kSysErr;
  }

  SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
  SSL_CTX_set_options(ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
                               SSL_OP_NO_COMPRESSION |
                               SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);

  BIO *bio = BIO_new_mem_buf((void *)cert_data.c_str(), cert_data.size());
  if (bio == nullptr) {
    err_msg_ = "[syserr]:create bio failed";
    err_msg_ += ERR_error_string(ERR_get_error(), nullptr);
    ERR_clear_error();
    return kSysErr;
  }

  X509 *cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
  BIO_free(bio);
  if (cert == nullptr) {
    err_msg_ = "[syserr]:read certificate failed";
    err_msg_ += ERR_error_string(ERR_get_error(), nullptr);
    ERR_clear_error();
    return kSysErr;
  }

  if (SSL_CTX_use_certificate(ctx, cert) <= 0) {
    err_msg_ = "[syserr]:use certificate failed";
    err_msg_ += ERR_error_string(ERR_get_error(), nullptr);
    ERR_clear_error();
    return kSysErr;
  }

  if (!password.empty()) {
    SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)password.c_str());
  }
  X509_free(cert);
  return Init(ctx);
}

int SSLContext::InitSvrFile(const std::string &cert_path,
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

  SSL_load_error_strings();
  SSL_library_init();
  OPENSSL_init_ssl(
      OPENSSL_INIT_LOAD_SSL_STRINGS | OPENSSL_INIT_LOAD_CRYPTO_STRINGS, NULL);

  SSL_CTX *ctx = SSL_CTX_new(TLS_method());
  if (ctx == nullptr) {
    err_msg_ = "[syserr]:create ssl context failed";
    return kSysErr;
  }

  SSL_CTX_set_mode(ctx, SSL_MODE_AUTO_RETRY);
  SSL_CTX_set_options(ctx, SSL_OP_ALL | SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 |
                               SSL_OP_NO_COMPRESSION |
                               SSL_OP_NO_SESSION_RESUMPTION_ON_RENEGOTIATION);

  if (SSL_CTX_use_certificate_chain_file(ctx, cert_path.c_str()) != 1) {
    err_msg_ = std::string("[syserr]:use certificate file failed ") +
               ERR_error_string(ERR_get_error(), nullptr);
    ERR_clear_error();
    return kSysErr;
  }

  if (!password.empty()) {
    SSL_CTX_set_default_passwd_cb_userdata(ctx, (void *)password.c_str());
  }

  if (SSL_CTX_use_PrivateKey_file(ctx, key_path.c_str(), SSL_FILETYPE_PEM) !=
      1) {
    err_msg_ = std::string("[syserr]:use private key file failed ") +
               ERR_error_string(ERR_get_error(), nullptr);
    ERR_clear_error();
    return kSysErr;
  }

  return Init(ctx);
}

std::shared_ptr<SSLSocket> SSLContext::CreateSSLSocket(const Socket &soc) {
  if (ssl_ctx_ == nullptr) {
    err_msg_ = "[loginerr]:ssl context is null";
    return nullptr;
  }

  SSL *ssl = SSL_new(ssl_ctx_);
  if (ssl == nullptr) {
    err_msg_ = "[syserr]:create ssl failed";
    return nullptr;
  }

  return std::make_shared<SSLSocket>(ssl, soc);
}

std::shared_ptr<SSLSocket> SSLContext::CreateSSLSocket() {
  if (ssl_ctx_ == nullptr) {
    err_msg_ = "[loginerr]:ssl context is null";
    return nullptr;
  }

  Socket soc;
  auto rc = soc.Init();
  if (rc != kSuccess) {
    err_msg_ = soc.err_msg();
    return nullptr;
  }
  return CreateSSLSocket(soc);
}

std::shared_ptr<SSLSocket> SSLContext::AcceptSSL(const Socket &soc) {
  auto new_ssl = SSL_new(ssl_ctx_);
  if (new_ssl == nullptr) {
    err_msg_ = "[syserr]:create ssl failed";
    return nullptr;
  }
  SSL_set_fd(new_ssl, soc.fd());
  auto rc = SSL_accept(new_ssl);
  if (rc < 0) {
    err_msg_ = std::string("[syserr]:ssl accept failed ") +
               ERR_error_string(ERR_get_error(), nullptr);
    SSL_free(new_ssl);
    ERR_clear_error();
    return nullptr;
  }
  return std::make_shared<SSLSocket>(new_ssl, soc);
}

int SSLContext::Close() {
  if (ssl_ctx_ != nullptr) {
    SSL_CTX_free(ssl_ctx_);
    ssl_ctx_ = nullptr;
  }
  return kSuccess;
}

} // namespace cppnet

#endif
