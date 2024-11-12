#pragma once

#include "logger.hpp"
#include <iostream>
#include <string>
namespace cppnet {

class StdLogger : public Logger {
public:
  StdLogger() = default;
  virtual void Debug(const std::string &msg) override {
    if (level_ > Logger::Level::kDebug) {
      return;
    }
    std::cout << "[DEBUG]:" << msg << std::endl;
  }
  virtual void Info(const std::string &msg) override {
    if (level_ > Logger::Level::kInfo) {
      return;
    }
    std::cout << "[INFO]:" << msg << std::endl;
  }
  virtual void Warn(const std::string &msg) override {
    if (level_ > Logger::Level::kWarn) {
      return;
    }
    std::cout << "[WARN]:" << msg << std::endl;
  }
  virtual void Error(const std::string &msg) override {
    if (level_ > Logger::Level::kError) {
      return;
    }
    std::cerr << "[ERROR]:" << msg << std::endl;
  }
  virtual void Fatal(const std::string &msg) override {
    std::cerr << "[FATAL]:" << msg << std::endl;
  }
};

} // namespace cppnet
