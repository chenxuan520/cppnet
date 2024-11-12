#pragma once

#include "io_multiplexing_base.hpp"
#include <unistd.h>
namespace cppnet {

class Select : public IOMultiplexingBase {
public:
  Select();
  ~Select();
  int Init() override;
  int MonitorSoc(const Socket &fd) override;
  int RemoveSoc(const Socket &fd) override;
  int Loop(NotifyCallBack callback) override;
  void Close() override;

private:
  fd_set monitor_set_;
  int max_fd_ = 0;
};

} // namespace cppnet
