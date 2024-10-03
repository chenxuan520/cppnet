
#include "tcp_server.hpp"
#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <signal.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

namespace cppnet {

TcpServer::TcpServer(const std::string &ip, uint16_t port) : addr_(ip, port) {}

TcpServer::TcpServer(Address &addr) : addr_(addr) {}

TcpServer::~TcpServer() { Stop(); }

Socket TcpServer::CreateEpoll() {
  int epfd = epoll_create1(0);
  if (epfd < 0) {
    return -1;
  }
  return epfd;
}

Socket TcpServer::CreateSocket() {
  Socket listenfd;
  auto rc = listenfd.Init();
  if (rc < 0) {
    return -1;
  }

  // set reuse addr to avoid time wait delay
  rc = listenfd.SetReuseAddr();
  if (rc < 0) {
    return -1;
  }

  rc = listenfd.Bind(addr_);
  if (rc < 0) {
    return -1;
  }

  return listenfd;
}

void TcpServer::Register(EventCallBack cb) { event_callback_ = cb; }

int TcpServer::Listen(int fd) {
  if (::listen(fd, max_connect_queue_) < 0) {
    return -1;
  }
  return 0;
}

int TcpServer::UpdateEpollEvents(int efd, int op, int fd, int events) {
  struct epoll_event ev;
  ev.events = events;
  ev.data.fd = fd;
  return epoll_ctl(efd, op, fd, &ev);
}

int TcpServer::DeleteEpollEvents(int efd, int fd) {
  return epoll_ctl(efd, EPOLL_CTL_DEL, fd, nullptr);
}

int TcpServer::CloseFd(int fd) {
  auto rc = DeleteEpollEvents(epfd_.fd(), fd);
  if (rc != 0) {
    return rc;
  }
  return ::close(fd);
}

void TcpServer::HandleAccept() {
  Address addr;
  socklen_t in_len = sizeof(in_addr);

  auto new_socket = listenfd_.Accept(addr, &in_len);
  if (new_socket.fd() < 0) {
    return;
  }

  auto rc = UpdateEpollEvents(epfd_.fd(), EPOLL_CTL_ADD, new_socket.fd(),
                              EPOLLIN | EPOLLET | EPOLLRDHUP);
  if (rc < 0) {
    new_socket.Close();
    return;
  }

  if (event_callback_) {
    event_callback_(kEventAccept, *this, new_socket);
  }
}

void TcpServer::HandleRead(int fd) {
  if (event_callback_) {
    event_callback_(kEventRead, *this, fd);
  }
}

void TcpServer::HandleWrite(int fd) {}

void TcpServer::HandleLeave(int fd) {
  if (event_callback_) {
    event_callback_(kEventLeave, *this, fd);
  }
  CloseFd(fd);
}

void TcpServer::EpollLoop() {
  if (epfd_.status() != Socket::kInit || listenfd_.status() != Socket::kInit) {
    return;
  }

  struct epoll_event events[max_event_num_];
  while (loop_flag_) {
    int nfds = epoll_wait(epfd_.fd(), events, max_event_num_, epoll_timeout_);
    if (nfds < 0) {
      if (errno == EINTR) {
        continue;
      }
      break;
    }

    for (int i = 0; i < nfds; ++i) {
      if (events[i].data.fd == listenfd_.fd()) {
        HandleAccept();
      } else if (events[i].events & EPOLLRDHUP || events[i].events & EPOLLERR ||
                 events[i].events & EPOLLHUP) {
        // must deal leave event before read event, because read event may be
        // triggered after leave event
        HandleLeave(events[i].data.fd);
      } else if (events[i].events & EPOLLIN) {
        HandleRead(events[i].data.fd);
      } else if (events[i].events & EPOLLOUT) {
        HandleWrite(events[i].data.fd);
      } else {
        // unknown event,dismiss
      }
    }
  }
}

void TcpServer::Stop() {
  loop_flag_ = false;
  if (epfd_.status() == Socket::kInit) {
    epfd_.Close();
  }
  if (listenfd_.status() == Socket::kInit) {
    listenfd_.Close();
  }
}

int TcpServer::Init() {
  listenfd_ = CreateSocket();
  if (listenfd_.status() != Socket::kInit) {
    return -1;
  }

  auto retval = listenfd_.SetNonBlock();
  if (retval < 0) {
    listenfd_.Close();
    return retval;
  }

  retval = listenfd_.Listen(max_connect_queue_);
  if (retval < 0) {
    listenfd_.Close();
    return retval;
  }

  epfd_ = CreateEpoll();
  if (epfd_.status() != Socket::kInit) {
    listenfd_.Close();
    return -1;
  }

  retval = UpdateEpollEvents(epfd_.fd(), EPOLL_CTL_ADD, listenfd_.fd(),
                             EPOLLIN | EPOLLET | EPOLLRDHUP);
  if (retval < 0) {
    listenfd_.Close();
    epfd_.Close();
    return retval;
  }
  return 0;
}

} // namespace cppnet
