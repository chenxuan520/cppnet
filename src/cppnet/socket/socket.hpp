#pragma once

#include "address.hpp"
#include <cstring>
#include <unistd.h>

#ifndef _WIN32
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif

namespace cppnet {

class Socket {
public:
  enum Status {
    kUninit = 0,
    kInit = 1,
    kClosed = 2,
  };

public:
  Socket() = default;
  Socket(int fd);

  /**
   * @brief: Init socket.
   * @return: 0 if success, -1 if failed.
   */
  int Init();
  /**
   * @brief: Init socket with udp
   * @return: 0 if success, -1 if failed.
   */
  int InitUdp();
  /**
   * @brief: try to connect to server.
   * @param addr: server address.
   * @param len: address length.
   */
  virtual int Connect(Address &addr);
  /**
   * @brief: Bind socket.
   * @param addr: server address.
   */
  virtual int Bind(Address &addr);
  /**
   * @brief: Accept socket.
   * @param addr: server address.
   */
  virtual Socket Accept(Address &addr);
  /**
   * @brief: Close socket.
   */
  virtual int Listen(int max_connect_queue);
  /**
   * @brief: Close socket.
   */
  virtual int Close();
  /**
   * @brief: Read data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param complete: if must need recv len data
   * @return: read length.
   */
  int Read(std::string &buf, size_t len, bool complete = false);
  /**
   * @brief: Read data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param complete: if must need recv len data
   * @return: read length.
   */
  int Read(void *buf, size_t len, bool complete = false);
  /**
   * @brief: Read data from socket until delim(include delim).
   * @param buf: buffer to store data.
   * @param delim: delimiter
   * @return: read length.
   */
  int ReadUntil(std::string &buf, const std::string &delim);
  /**
   * @brief: Read data from socket but not remove data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @return: read length.
   */
  int ReadPeek(std::string &buf, size_t len);
  /**
   * @brief: Read data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param address: udp read from
   * @return: read length.
   */
  int ReadUdp(std::string &buf, size_t len, Address &addr);
  /**
   * @brief: Read data from udp socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param address: udp read from
   * @return: read length.
   */
  int ReadUdp(void *buf, size_t len, Address &addr);
  /**
   * @brief: Write data to socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @return: write length.
   */
  int Write(const std::string &buf);
  /**
   * @brief: Write data to socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @return: write length.
   */
  int Write(const void *buf, size_t len);
  /**
   * @brief: Write data to udp socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param addr: udp write to
   * @return: write length.
   */
  int WriteUdp(const std::string &buf, Address &addr);
  /**
   * @brief: Write data to udp socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param addr: udp write to
   * @return: write length.
   */
  int WriteUdp(const void *buf, size_t len, Address &addr);
  /**
   * @brief: Set socket nonblock.
   */
  int SetNoBlock() const;
  /**
   * @brief: Set socket block
   */
  int SetBlock() const;
  /**
   * @brief: set socket reuse addr
   */
  int SetReuseAddr() const;
  /**
   * @brief: set socket read time out
   * @param timeout_sec: timeout second
   * @param timeout_usec: timeout microsecond, 1s=1e6us
   */
  int SetReadTimeout(int timeout_sec, int timeout_usec) const;
  /**
   * @brief: set socket write time out
   * @param timeout_sec: timeout second
   * @param timeout_usec: timeout nano second
   */
  int SetWriteTimeout(int timeout_sec, int timeout_usec) const;
  /**
   * @brief: set socket opt
   * @param level: socket level
   * @param optname: socket option name
   * @param optval: socket option value
   * @param optlen: socket option length
   * @return: 0 if success,other failed.
   */
  int SetSockOpt(int level, int optname, const void *optval,
                 size_t optlen) const;
  /**
   * @brief: get addr from socket
   */
  int GetAddr(Address &addr) const;
  /**
   * @brief: get addr from socket
   */
  Address GetAddr() const;
  /**
   * @brief: get system error string
   */
  static inline std::string err_msg() { return strerror(errno); }
  /**
   * @brief: get system error number
   */
  static inline int err_no() { return errno; }

public:
  inline int fd() const { return fd_; }
  inline Status status() const { return status_; }

public:
  /**
   * @brief: Compare operator.
   */
  bool operator==(const Socket &rhs) const { return fd_ == rhs.fd_; }

protected:
  virtual inline int IORead(void *buf, size_t len, int flags = 0) {
#ifndef _WIN32
    return ::recv(fd_, buf, len, flags);
#else
    return ::recv(fd_, (char *)buf, len, flags);
#endif
  }

  virtual inline int IOWrite(const void *buf, size_t len, int flags = 0) {
#ifndef _WIN32
    return ::send(fd_, buf, len, flags);
#else
    return ::send(fd_, (char *)buf, len, flags);
#endif
  }

protected:
  int fd_ = -1;
  Status status_ = kUninit;
};

} // namespace cppnet
