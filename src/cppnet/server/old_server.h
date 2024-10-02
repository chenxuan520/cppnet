#pragma once

#include <arpa/inet.h>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <netinet/in.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <thread>
#include <unordered_map>
#include <vector>

namespace cppnet {

using callback_recv_t = std::function<void(const std::string &data, int &id)>;
using callback_handle_timer_t = std ::function<int(
    std::unordered_map<int, std::string> &, std::unordered_map<int, int> &)>;
using callback_game_reset_t = std::function<bool()>;

/**
 * @brief: Tcp server with epoll io mux.
 */
class EpollTcpServer {
public:
  EpollTcpServer() = default;
  /**
   * @param ip: ip addr
   * @param port: port
   */
  EpollTcpServer(std::string ip, uint16_t port) : ip_(ip), port(port) {}

  EpollTcpServer(const EpollTcpServer &other) = delete;
  EpollTcpServer &operator=(const EpollTcpServer &other) = delete;
  EpollTcpServer(EpollTcpServer &&other) = delete;
  EpollTcpServer &operator=(EpollTcpServer &&other) = delete;
  ~EpollTcpServer() { Stop(); };

  /**
   * @brief: Init tcp server. Create epfd, bind serverfd, set serverfd non
   * block and register serverfd to epoll.
   */
  bool Init();
  /**
   * @brief: Closes tcp server. Close epfd and serverfd.
   */
  bool Stop();
  /**
   * @brief: Tcp server send msg to peer.
   */
  int SendData(int fd, std::string msg);
  /**
   * @brief: Register callback function when recv peer data.
   */
  void RegisterOnRecvCallback(callback_recv_t callback);
  /**
   * @brief: Register callback function when timer ticks.
   */
  void RegisterOnHandleTimerCallback(callback_handle_timer_t callback);
  /**
   * @brief: Unregister callback function of recv.
   */
  void ResetRecvCallback();
  /**
   * @brief: Unregister callback function of handle_timer.
   */
  void ResetOnHandleTimerCallback(); // nyw add
  /**
   * @brief: Epoll event loop.
   */

  void RegisterOnGameResetCallback(callback_game_reset_t callback);
  void ResetOnGameResetCallback();

  void EpollLoop();

protected:
  int CreateEpoll();
  int CreateSocket();
  int CreateTimer();
  int SetSocketNonblock(int fd);
  int Listen(int fd);
  int UpdateEpollEvents(int efd, int op, int fd, int events);
  int DeleteEpollEvents(int efd, int fd);
  int CloseFd(int fd);

  void HandleAccept();
  void HandleRead(int fd);
  void HandleWrite(int fd);
  bool Reset();
  bool ResetTimer(int timerfd);
  // NOTE(nyw):这里用作每回合刷新game的状态，应该在api.h中实现，使server和game解耦，这里简单采用回调函数代替
  // void handle_timer(int fd);

  int kMaxConnecNum;
  int kMaxEventNum;
  int kEpollTimeout;
  int kTimerInitTime;
  int kTimerIntervalTime;
  const int kMicroSecond = 1000;

private:
  // Dot seperated ip address. eg: 127.0.0.1
  std::string ip_;
  // port
  uint16_t port{0};
  // epoll file descriptor
  int epfd_{-1};
  // server file descriptor
  int listenfd_{-1};
  // time file descriptor
  int timerfd_{-1};
  // one loop per thread
  std::shared_ptr<std::thread> th_{nullptr};
  // epoll loop flag, if false then exit
  bool loop_flag_{true};
  // callback function when recv data
  callback_recv_t recv_callbak_{nullptr};
  // callback function when timer ticks
  callback_handle_timer_t handle_timer_callbak_{nullptr};
  // callback function for game reset
  callback_game_reset_t game_reset_callbak_{nullptr};
};

typedef std::shared_ptr<EpollTcpServer> EpollTcpServerPtr;

} // namespace cppnet
