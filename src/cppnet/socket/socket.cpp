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

int Socket::InitUdp() {
  fd_ = ::socket(AF_INET, SOCK_DGRAM, 0);
  if (fd_ < 0) {
    return -1;
  }
  status_ = kInit;
  return 0;
}

Socket Socket::Accept(Address &addr) {
  if (status_ != kInit) {
    return Socket(-1);
  }
  auto accept_fd = ::accept(fd_, addr.GetSockAddr(), addr.GetAddrLen());
  return Socket(accept_fd);
}

int Socket::Listen(int max_connect_queue) {
  if (status_ != kInit) {
    return -1;
  }
  return ::listen(fd_, max_connect_queue);
}

int Socket::Bind(Address &addr) {
  if (status_ != kInit) {
    return -1;
  }
  return ::bind(fd_, addr.GetSockAddr(), *addr.GetAddrLen());
}

int Socket::Connect(Address &addr) {
  if (status_ != kInit) {
    return -1;
  }
  return ::connect(fd_, addr.GetSockAddr(), *addr.GetAddrLen());
}

int Socket::Close() {
  if (status_ != kInit) {
    return -1;
  }
  status_ = kClosed;
  return ::close(fd_);
}

int Socket::SetNoBlock() const {
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
}

int Socket::SetBlock() const {
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd_, F_SETFL, flags & ~O_NONBLOCK);
}

int Socket::Read(std::string &buf, size_t len, bool complete) {
  if (status_ != kInit) {
    return -1;
  }
  char *data = new char[len + 1];
  memset(data, 0, len + 1);
  auto recv_len = Read(data, len, complete);
  buf = data;
  delete[] data;
  return recv_len;
}

int Socket::Read(void *buf, size_t len, bool complete) {
  if (status_ != kInit) {
    return -1;
  }
  auto recv_len = 0;
  if (complete) {
    while (recv_len < len) {
      auto rc = IORead((char *)buf + recv_len, len - recv_len);
      if (rc <= 0) {
        break;
      }
      recv_len += rc;
    }
    return recv_len;
  } else {
    return IORead(buf, len);
  }
}

int Socket::ReadUdp(std::string &buf, size_t len, Address &addr) {
  if (status_ != kInit) {
    return -1;
  }
  int addr_len = sizeof(sockaddr);
  char *data = new char[len + 1];
  memset(data, 0, len + 1);
  auto rc = ReadUdp(data, len, addr);
  buf = data;
  delete[] data;
  return rc;
}

int Socket::ReadUdp(void *buf, size_t len, Address &addr) {
  if (status_ != kInit) {
    return -1;
  }
  int addr_len = sizeof(sockaddr);
  return ::recvfrom(fd_, buf, len, 0, addr.GetSockAddr(),
                    (socklen_t *)&addr_len);
}

int Socket::Write(const std::string &buf) {
  return Write(buf.c_str(), buf.size());
}

int Socket::Write(const void *buf, size_t len) {
  if (status_ != kInit) {
    return -1;
  }
  return IOWrite(buf, len);
}

int Socket::WriteUdp(const std::string &buf, Address &addr) {
  return WriteUdp(buf.c_str(), buf.size(), addr);
}

int Socket::WriteUdp(const void *buf, size_t len, Address &addr) {
  if (status_ != kInit) {
    return -1;
  }
  return ::sendto(fd_, buf, len, 0, addr.GetSockAddr(), sizeof(sockaddr));
}

int Socket::SetReuseAddr() const {
  int reuse_addr_on = 1;
  return ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_on,
                      sizeof(reuse_addr_on));
}

int Socket::SetSockOpt(int level, int optname, const void *optval,
                       size_t optlen) const {
  return ::setsockopt(fd_, level, optname, optval, optlen);
}

int Socket::GetAddr(Address &addr) const {
  if (status_ != kInit) {
    return -1;
  }
  socklen_t addr_len = sizeof(sockaddr);
  return getpeername(fd_, addr.GetSockAddr(), addr.GetAddrLen());
}

} // namespace cppnet
