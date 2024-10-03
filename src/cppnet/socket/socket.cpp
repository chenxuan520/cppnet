#include "socket.hpp"
#include <cstring>
#include <fcntl.h>
#include <sys/socket.h>

namespace cppnet {

Socket::Socket(int fd) : fd_(fd) {
  if (fd_ < 0) {
    return;
  }
  status_ = kInit;
}

int Socket::Init() {
  fd_ = ::socket(AF_INET, SOCK_STREAM, 0);
  if (fd_ < 0) {
    return -1;
  }
  status_ = kInit;
  return 0;
}

Socket Socket::Accept(Address &addr, socklen_t *plen) const {
  if (status_ != kInit) {
    return Socket(-1);
  }
  auto accept_fd = ::accept(fd_, addr.GetSockAddr(), plen);
  return Socket(accept_fd);
}

int Socket::Listen(int max_connect_queue) const {
  if (status_ != kInit) {
    return -1;
  }
  return ::listen(fd_, max_connect_queue);
}

int Socket::Bind(Address &addr) const {
  if (status_ != kInit) {
    return -1;
  }
  return ::bind(fd_, addr.GetSockAddr(), sizeof(sockaddr));
}

int Socket::Connect(Address &addr) const {
  if (status_ != kInit) {
    return -1;
  }
  return ::connect(fd_, addr.GetSockAddr(), sizeof(sockaddr));
}

int Socket::Close() {
  if (status_ != kInit) {
    return -1;
  }
  status_ = kClosed;
  return ::close(fd_);
}

int Socket::SetNonBlock() const {
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
}

int Socket::Read(std::string &buf, size_t len) const {
  if (status_ != kInit) {
    return -1;
  }
  char *data = new char[len + 1];
  memset(data, 0, len + 1);
  auto rc = ::read(fd_, data, len);
  buf = data;
  delete[] data;
  return rc;
}

int Socket::Read(void *buf, size_t len) const {
  if (status_ != kInit) {
    return -1;
  }
  return ::read(fd_, buf, len);
}

int Socket::Write(const std::string &buf) const {
  if (status_ != kInit) {
    return -1;
  }
  return ::write(fd_, buf.c_str(), buf.size());
}

int Socket::Write(const void *buf, size_t len) const {
  if (status_ != kInit) {
    return -1;
  }
  return ::write(fd_, buf, len);
}

int Socket::SetReuseAddr() const {
  int reuse_addr_on = 1;
  return ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_on,
                      sizeof(reuse_addr_on));
}

} // namespace cppnet
