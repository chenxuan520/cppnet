#pragma once

#include "address.hpp"
#include <sys/socket.h>
#include <unistd.h>
namespace cppnet {

class Socket {
public:
  enum Status {
    kUninit = 0,
    kInit = 1,
    kClosed = 3,
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
  Socket Accept(Address &addr, socklen_t *plen) const;
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
   * @return: read length.
   */
  int Read(std::string &buf, size_t len) const;
  /**
   * @brief: Write data to socket.
   * @param buf: buffer to store data.
   * @param len: buffer length.
   * @return: write length.
   */
  int Write(const std::string &buf) const;
  /**
   * @brief: Set socket nonblock.
   */
  int SetNonBlock() const;
  /**
   * set socket reuse addr
   */
  int SetReuseAddr() const;

public:
  inline int fd() const { return fd_; }
  inline Status status() const { return status_; }

private:
  int fd_ = -1;
  Status status_ = kUninit;
};

} // namespace cppnet
