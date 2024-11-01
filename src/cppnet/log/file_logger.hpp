#pragma once
#include "logger.hpp"

namespace cppnet {

class FileLogger : public Logger {
public:
  int Init(const std::string &file_path);
  // TODO: finish it //
  virtual void Debug(const std::string &msg) override;
  virtual void Info(const std::string &msg) override;
  virtual void Warn(const std::string &msg) override;
  virtual void Error(const std::string &msg) override;

private:
  std::string file_path_;
};

} // namespace cppnet
