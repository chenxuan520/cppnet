#pragma once

#include <functional>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>
namespace cppnet {

using RecvCallBack = std::function<void(const std::string &data, int &id)>;
using TimerCallback = std ::function<int(std::unordered_map<int, std::string> &,
                                         std::unordered_map<int, int> &)>;

class TcpServer {
public:
  TcpServer() = default;
  TcpServer(const std::string &ip, uint16_t port);
  virtual ~TcpServer();
  /**
   * @brief: Init tcp server. Create epfd, bind serverfd, set serverfd non block
   * and register serverfd to epoll.
   */
  void Init();
  /**
   * @brief: Closes tcp server. Close epfd and serverfd.
   */
  void Stop();
  /**
   * @brief: Tcp server send msg to peer.
   */
  int SendData(int fd, std::string msg);
  /**
   * @brief: Register callback function when recv peer data.
   */
  void RegisterRecvCallback(RecvCallBack recv_callback);
  /**
   * @brief: Register callback function when timer ticks.
   */
  void RegisterTimerCallback(TimerCallback timer_callback);
  /**
   * @brief: Unregister callback function of recv.
   */
  void ResetRecvCallback();
  /**
   * @brief: Unregister callback function of handle_timer.
   */
  void ResetTimerCallback();
  /**
   * @brief: Epoll event loop.
   */
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

private:
  // Dot seperated ip address. eg: 127.0.0.1
  std::string ip_;
  // port
  uint16_t port_{0};
  // epoll file descriptor
  int epfd_{-1};
  // server file descriptor
  int listenfd_{-1};
  // time file descriptor
  int timerfd_{-1};
  // epoll loop flag, if false then exit
  bool loop_flag_{true};
  // callback function when recv data
  RecvCallBack recv_callbak_{nullptr};
  // callback function when timer ticks
  TimerCallback handle_timer_callbak_{nullptr};
};

} // namespace cppnet
