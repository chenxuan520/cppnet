
#include "tcp_server.hpp"

#include <algorithm>
#include <arpa/inet.h>
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cassert>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <unordered_map>

namespace cppnet {

TcpServer::~TcpServer() { Stop(); }

TcpServer::TcpServer(const std::string &ip, uint16_t port)
    : ip_(ip), port_(port) {}

void TcpServer::Init() {
  listenfd_ = CreateSocket();
  if (listenfd_ < 0) {
    exit(-1);
  }

  if (Listen(listenfd_) < 0) {
    exit(-1);
  }

  epfd_ = CreateEpoll();
  if (epfd_ < 0) {
    exit(-1);
  }

  if (SetSocketNonblock(listenfd_) < 0) {
    exit(-1);
  }

  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = listenfd_;
  if (epoll_ctl(epfd_, EPOLL_CTL_ADD, listenfd_, &ev) < 0) {
    exit(-1);
  }
}

int TcpServer::CreateSocket() {
  int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_);
  addr.sin_addr.s_addr = inet_addr(ip_.c_str());

  // set reuse addr to avoid time wait delay
  int reuse_addr_on = 1;
  if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse_addr_on,
                 sizeof(reuse_addr_on)) < 0) {
    ::close(listenfd);
    exit(-1);
  }

  if (::bind(listenfd, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
    ::close(listenfd);
    exit(-1);
  }

  return listenfd;
}

int TcpServer::CreateEpoll() {
  int epfd = epoll_create1(0);
  if (epfd < 0) {
    return -1;
  }

  return epfd;
}

int TcpServer::SetSocketNonblock(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags < 0) {
    return -1;
  }

  int retval = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  if (retval < 0) {
    return -1;
  }

  return 0;
}

} // namespace cppnet
