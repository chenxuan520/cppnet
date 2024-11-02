#pragma once

#include <string>
namespace cppnet {

class Logger {
public:
  enum class Level { DEBUG = 0, INFO = 1, WARN = 2, ERROR = 3, FATAL = 4 };

public:
  Logger() = default;
  virtual void Debug(const std::string &msg) {}
  virtual void Info(const std::string &msg) {}
  virtual void Warn(const std::string &msg) {}
  virtual void Error(const std::string &msg) {}
  virtual void Fatal(const std::string &msg) {}
  void set_level(Level level) { level_ = level; }

protected:
  Level level_ = Level::DEBUG;
};

} // namespace cppnet
