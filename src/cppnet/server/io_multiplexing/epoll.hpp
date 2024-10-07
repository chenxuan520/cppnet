#pragma once
#include "io_multiplexing_base.hpp"

namespace cppnet {

class Epoll : public IOMultiplexingBase {
public:
  enum TriggerType {
    kEdgeTrigger,
    kLevelTrigger,
  };

public:
  Epoll() : IOMultiplexingBase() {}
  ~Epoll() {}
  int Init() override;
  int MonitorSoc(const Socket &fd) override;
  int RemoveSoc(const Socket &fd) override;
  int Loop(NotifyCallBack callback) override;
  void Close() override;

public:
  static void SetTriggerType(TriggerType type) { trigger_type_ = type; }

private:
  Socket CreateEpoll();
  int UpdateEpollEvents(int epfd, int op, int fd, int event);

private:
  Socket epoll_fd_;

private:
  static TriggerType trigger_type_;
};

} // namespace cppnet
