#pragma once

#include "address.hpp"
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

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
  int Connect(Address &addr) const;
  /**
   * @brief: Bind socket.
   * @param addr: server address.
   */
  int Bind(Address &addr) const;
  /**
   * @brief: Accept socket.
   * @param addr: server address.
   * @param plen: pointer of address length.
   */
  Socket Accept(Address &addr) const;
  /**
   * @brief: Close socket.
   */
  int Listen(int max_connect_queue) const;
  /**
   * @brief: Close socket.
   */
  int Close();
  /**
   * @brief: Read data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param complete: if must need recv len data
   * @return: read length.
   */
  int Read(std::string &buf, size_t len, bool complete = false) const;
  /**
   * @brief: Read data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param complete: if must need recv len data
   * @return: read length.
   */
  int Read(void *buf, size_t len, bool complete = false) const;
  /**
   * @brief: Read data from socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param address: udp read from
   * @return: read length.
   */
  int ReadUdp(std::string &buf, size_t len, Address &addr) const;
  /**
   * @brief: Read data from udp socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param address: udp read from
   * @return: read length.
   */
  int ReadUdp(void *buf, size_t len, Address &addr) const;
  /**
   * @brief: Write data to socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @return: write length.
   */
  int Write(const std::string &buf) const;
  /**
   * @brief: Write data to socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @return: write length.
   */
  int Write(const void *buf, size_t len) const;
  /**
   * @brief: Write data to udp socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param addr: udp write to
   * @return: write length.
   */
  int WriteUdp(const std::string &buf, Address &addr) const;
  /**
   * @brief: Write data to udp socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @param addr: udp write to
   * @return: write length.
   */
  int WriteUdp(const void *buf, size_t len, Address &addr) const;
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
   * @brief: get addr from socket
   */
  int GetAddr(Address &addr) const;
  /**
   * @brief: get system error string
   */
  static inline std::string GetSysErr() { return strerror(errno); }

public:
  inline int fd() const { return fd_; }
  inline Status status() const { return status_; }

public:
  /**
   * @brief: Compare operator.
   */
  bool operator==(const Socket &rhs) const { return fd_ == rhs.fd_; }

private:
  int fd_ = -1;
  Status status_ = kUninit;
};

} // namespace cppnet
