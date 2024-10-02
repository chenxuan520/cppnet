#include "old_server.h"
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

static std::unordered_map<int, int> fd2PlayerId;
static std::unordered_map<int, std::string> fd2msg;

int EpollTcpServer::CreateSocket() {
  int listenfd = ::socket(AF_INET, SOCK_STREAM, 0);
  if (listenfd < 0) {
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
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

int EpollTcpServer::CreateTimer() {
  int timerfd = timerfd_create(CLOCK_REALTIME, 0);
  if (timerfd <= 0) {
    return -1;
  }

  ResetTimer(timerfd);
  return timerfd;
}

bool EpollTcpServer::ResetTimer(int timerfd) {
  struct itimerspec ts;
  ts.it_value.tv_sec = kTimerInitTime / kMicroSecond;
  ts.it_value.tv_nsec = kTimerInitTime % kMicroSecond * 1e6;
  ts.it_interval.tv_sec = kTimerIntervalTime / kMicroSecond;
  ts.it_interval.tv_nsec = kTimerIntervalTime % kMicroSecond * 1e6;

  timerfd_settime(timerfd, 0, &ts, NULL);
  return true;
}

int EpollTcpServer::SetSocketNonblock(int fd) {

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

int EpollTcpServer::Listen(int fd) {
  int retval = ::listen(fd, kMaxConnecNum);
  if (retval < 0) {
    return -1;
  }

  return 0;
}

int EpollTcpServer::CreateEpoll() {
  int epfd = epoll_create1(0);
  if (epfd < 0) {
    return -1;
  }

  return epfd;
}

int EpollTcpServer::DeleteEpollEvents(int efd, int fd) {
  struct epoll_event ev;
  memset(&ev, 0, sizeof(epoll_event));

  int retval = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
  if (retval < 0) {
    return -1;
  }

  return 0;
}

int EpollTcpServer::UpdateEpollEvents(int efd, int op, int fd, int events) {

  struct epoll_event ev;
  memset(&ev, 0, sizeof(epoll_event));

  ev.events = events;
  // TODO(gpl): handler param
  ev.data.fd = fd;

  int retval = epoll_ctl(efd, op, fd, &ev);
  if (retval < 0) {
    return -1;
  }

  return 0;
}

bool EpollTcpServer::Init() {
  epfd_ = CreateEpoll();
  if (epfd_ < 0) {
    return false;
  }

  timerfd_ = CreateTimer();
  if (timerfd_ < 0) {
    return false;
  }

  listenfd_ = CreateSocket();
  if (listenfd_ < 0) {
    return false;
  }

  int retval = SetSocketNonblock(listenfd_);
  if (retval < 0) {
    return false;
  }

  retval = Listen(listenfd_);
  if (retval < 0) {
    return false;
  }

  retval =
      UpdateEpollEvents(epfd_, EPOLL_CTL_ADD, listenfd_, EPOLLIN | EPOLLET);
  if (retval < 0) {
    ::close(epfd_);
    return false;
  }

  retval = UpdateEpollEvents(epfd_, EPOLL_CTL_ADD, timerfd_, EPOLLET | EPOLLIN);
  if (retval < 0) {
    ::close(epfd_);
    return false;
  }

  return true;
}

bool EpollTcpServer::Stop() {
  loop_flag_ = false;
  ::close(listenfd_);
  ::close(timerfd_);
  ::close(epfd_);
  ResetRecvCallback();
  ResetOnHandleTimerCallback();
  return true;
}

bool EpollTcpServer::Reset() {

  // clean old connect
  for (auto [fd, _] : fd2PlayerId) {
    CloseFd(fd);
  }

  fd2PlayerId.clear();
  fd2msg.clear();

  // reset game
  if (!game_reset_callbak_()) {
    return false;
  }

  if (!ResetTimer(timerfd_)) {
    return false;
  }

  return true;
}

int EpollTcpServer::SendData(int fd, std::string msg) {
  if (fd < 0) {
    return -1;
  }

  uint64_t length = msg.size();
  int retval = ::write(fd, &length, sizeof(length));
  if (retval < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return -1;
    }
    CloseFd(fd);
  }

  retval = ::write(fd, msg.data(), msg.size());
  if (retval < 0) {
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return -1;
    }
    CloseFd(fd);
  }

  return retval;
}

int EpollTcpServer::CloseFd(int fd) {
  if (fd < 0) {
    return -1;
  }

  DeleteEpollEvents(epfd_, fd);
  ::close(fd);
  return 0;
}

void EpollTcpServer::RegisterOnRecvCallback(callback_recv_t callback) {
  if (callback == nullptr) {
  }
  recv_callbak_ = callback;
  return;
}

void EpollTcpServer::RegisterOnHandleTimerCallback(
    callback_handle_timer_t callback) { // nyw add
  if (callback == nullptr) {
  }
  handle_timer_callbak_ = callback;
  return;
}

void EpollTcpServer::RegisterOnGameResetCallback(
    callback_game_reset_t callback) {
  if (callback == nullptr) {
  }
  game_reset_callbak_ = callback;
  return;
}

void EpollTcpServer::ResetRecvCallback() {
  recv_callbak_ = nullptr;
  return;
}

void EpollTcpServer::ResetOnGameResetCallback() {
  game_reset_callbak_ = nullptr;
  return;
}

void EpollTcpServer::ResetOnHandleTimerCallback() { // nyw add
  handle_timer_callbak_ = nullptr;
  return;
}

void EpollTcpServer::HandleAccept() {

  while (true) {

    struct sockaddr_in in_addr;
    socklen_t in_len = sizeof(in_addr);

    int cli_fd = accept(listenfd_, (struct sockaddr *)&in_addr, &in_len);
    if (cli_fd < 0) {
      if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
        break;
      } else {
        continue;
      }
    }

    sockaddr_in peer_addr;
    socklen_t peer_len = sizeof(peer_addr);
    int retval = getpeername(cli_fd, (sockaddr *)&peer_addr, &peer_len);
    if (retval < 0) {
      continue;
    }

    retval = SetSocketNonblock(cli_fd);
    if (retval < 0) {
      ::close(cli_fd);
      continue;
    }

    retval = UpdateEpollEvents(epfd_, EPOLL_CTL_ADD, cli_fd,
                               EPOLLIN | EPOLLET | EPOLLRDHUP);
    if (retval < 0) {
      ::close(cli_fd);
      continue;
    }
  }
  return;
}

void EpollTcpServer::HandleRead(int fd) {

  if (fd < 0) {
    return;
  }

  uint64_t length = 0;
  if (::read(fd, &length, sizeof(length)) < sizeof(length)) {
    return;
  }

  if (length >= 65535) {
    return;
  }

  char buffer[length];

  bzero(buffer, sizeof(buffer));

  int offset = 0;
  int size = -1;

  while (offset < length) {
    while ((size = ::read(fd, buffer + offset, length - offset)) > 0) {
      offset += size;
    }
  }

  if (size == -1) {
    // finished reading bytes
    if (errno == EAGAIN || errno == EWOULDBLOCK) {
      return;
    }
    CloseFd(fd);
    return;
  }

  if (size == 0 && offset != length) {
    CloseFd(fd);
    return;
  }

  std::string msg(buffer, length);

  if (recv_callbak_) {
    if (fd2PlayerId.count(fd)) {
      recv_callbak_(msg, fd2PlayerId[fd]);
    } else {
      int player_id = -1; // nyw add to specify new InitReq
      recv_callbak_(msg, player_id);
      if (player_id != -1) // adding player succeed
      {
        fd2PlayerId.insert({fd, player_id});
      } else {
      }
    }
  }

  return;
}

void EpollTcpServer::HandleWrite(int fd) {
  // TODO(gpl): handle write
  return;
}

void EpollTcpServer::EpollLoop() {
  struct epoll_event ready_events[kMaxEventNum];
  bzero(ready_events, sizeof(ready_events));

  while (loop_flag_) {
    int num = epoll_wait(epfd_, (epoll_event *)&ready_events, kMaxEventNum,
                         kEpollTimeout);

    for (int i = 0; i < num; ++i) {
      int fd = ready_events[i].data.fd;
      int events = ready_events[i].events;

      if ((events & EPOLLERR) || (events & EPOLLHUP)) {
        CloseFd(fd);
      } else if (events & EPOLLRDHUP) {
        // Stream socket peer closed connection, or shut down writing half of
        // connection.
        CloseFd(fd);
      } else if (events & EPOLLIN) {
        if (fd == listenfd_) {
          HandleAccept();
        } else if (fd == timerfd_) {
          int64_t data;
          int retval = ::read(timerfd_, &data, sizeof(data));
          // MODIFIED(nyw): use handle timer call back function
          retval = handle_timer_callbak_(fd2msg, fd2PlayerId);
          for (auto &[fd, msg] : fd2msg) {
            SendData(fd, msg);
          }
          if (retval == 1) // game over
          {
            Reset();
          }
        } else {
          HandleRead(fd);
        }
      } else if (events & EPOLLOUT) {
        HandleWrite(fd);
      } else {
      }
    }
  }

  return;
}

} // namespace cppnet
