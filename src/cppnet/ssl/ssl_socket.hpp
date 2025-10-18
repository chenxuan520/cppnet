#pragma once

#include <memory>
#ifdef CPPNET_OPENSSL

#include "../socket/socket.hpp"
#include <mutex>
#include <openssl/ssl.h>
#include <string>
namespace cppnet {

class SSLSocket : public Socket {
public:
  /*
   * @brief: ssl socket mode
   * @note:
   *  kQuickly: quick connect(not supported as a socket simultaneous scene)
   *  kSafely: safely connect for thread(use mutex for simultaneous scene)
   */
  enum class Mode { kQuickly = 0, kSafely = 1 };

public:
  SSLSocket(SSL *ssl, const Socket &soc, Mode mode = Mode::kQuickly);
  ~SSLSocket() = default;
  // forbiden copy
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
   * @note: use close func will auto run close_ssl
   */
  int CloseSSL();

public:
  inline std::string err_msg() const { return err_msg_; }
  inline void set_mode(Mode mode) { mode_ = mode; }

protected:
  int IORead(void *buf, size_t len, int flag = 0) override;
  int IOWrite(const void *buf, size_t len, int flag = 0) override;

private:
  SSL *ssl_ = nullptr;
  std::string err_msg_;
  Mode mode_ = Mode::kQuickly;
  std::shared_ptr<std::mutex> pmutex_ = nullptr;
};

} // namespace cppnet

#endif
