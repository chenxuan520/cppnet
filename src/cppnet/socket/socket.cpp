#include "socket.hpp"
#include <cstring>
#include <fcntl.h>
#ifndef _WIN32
#include <sys/socket.h>
#else
#include <winsock2.h>
#endif

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
#ifndef _WIN32
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd_, F_SETFL, flags | O_NONBLOCK);
#else
  u_long nonBlocking = 1; // 1 表示非阻塞
  int result = ioctlsocket(fd_, FIONBIO, &nonBlocking);
  if (result != 0) {
      // 处理错误
      return result;
  }
  return 0;
#endif
}


int Socket::SetBlock() const {
#ifndef _WIN32
  int flags = fcntl(fd_, F_GETFL, 0);
  if (flags == -1) {
    return -1;
  }
  return fcntl(fd_, F_SETFL, flags & ~O_NONBLOCK);
#else
  u_long nonBlocking = 0; // 0 表示阻塞
  int result = ioctlsocket(fd_,FIONBIO, &nonBlocking);
  if (result != 0) {
      return result;
  }
  return 0;
#endif
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
    return IORead(buf, len, MSG_WAITALL);
  } else {
    return IORead(buf, len);
  }
}

int Socket::ReadPeek(std::string &buf, size_t len) {
  if (status_ != kInit) {
    return -1;
  }
  char *data = new char[len + 1];
  memset(data, 0, len + 1);
  auto recv_len = IORead(data, len, MSG_PEEK);
  buf = data;
  delete[] data;
  return recv_len;
}

int Socket::ReadUntil(std::string &buf, const std::string &delim) {
  if (status_ != kInit || delim.empty()) {
    return -1;
  }
  auto delim_len = delim.size();
  char ch = 0;
  while (ch != delim[delim_len - 1] ||
         buf.substr(buf.size() - delim.size()) != delim) {
    auto rc = IORead(&ch, 1);
    if (rc <= 0) {
      return rc;
    }
    buf += ch;
  }
  return buf.size();
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
#ifndef _WIN32
  return ::recvfrom(fd_, buf, len, 0, addr.GetSockAddr(),
                    (socklen_t *)&addr_len);
#else
  return ::recvfrom(fd_, (char*)buf, len, 0, addr.GetSockAddr(),
                    (socklen_t *)&addr_len);
#endif
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
#ifndef _WIN32
  return ::sendto(fd_, buf, len, 0, addr.GetSockAddr(), sizeof(sockaddr));
#else
  return ::sendto(fd_, (char*)buf, len, 0, addr.GetSockAddr(), sizeof(sockaddr));
#endif
}

int Socket::SetReuseAddr() const {
  int reuse_addr_on = 1;
  return Socket::SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &reuse_addr_on,
                            sizeof(reuse_addr_on));
}

int Socket::SetReadTimeout(int timeout_sec, int timeout_usec) const {
  struct timeval timeout;
  timeout.tv_sec = timeout_sec;
  timeout.tv_usec = timeout_usec;
  return Socket::SetSockOpt(SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
}

int Socket::SetWriteTimeout(int timeout_sec, int timeout_usec) const {
  struct timeval timeout;
  timeout.tv_sec = timeout_sec;
  timeout.tv_usec = timeout_usec;
  return Socket::SetSockOpt(SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
}

int Socket::SetSockOpt(int level, int optname, const void *optval,
                       size_t optlen) const {
#ifndef _WIN32
  return ::setsockopt(fd_, level, optname, optval, optlen);
#else
  return ::setsockopt(fd_, level, optname, (const char*)optval, optlen);
#endif
}

int Socket::GetAddr(Address &addr) const {
  if (status_ != kInit) {
    return -1;
  }
  return addr.GetPeerAddr(fd_, addr);
}

Address Socket::GetAddr() const {
  Address addr;
  GetAddr(addr);
  return addr;
}

} // namespace cppnet
