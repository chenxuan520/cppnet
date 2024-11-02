#pragma once

#include "logger.hpp"
#include <iostream>
#include <string>
namespace cppnet {

class StdLogger : public Logger {
public:
  StdLogger() = default;
  virtual void Debug(const std::string &msg) override {
    if (level_ > Logger::Level::DEBUG) {
      return;
    }
    std::cout << "[DEBUG]:" << msg << std::endl;
  }
  virtual void Info(const std::string &msg) override {
    if (level_ > Logger::Level::INFO) {
      return;
    }
    std::cout << "[INFO]:" << msg << std::endl;
  }
  virtual void Warn(const std::string &msg) override {
    if (level_ > Logger::Level::WARN) {
      return;
    }
    std::cout << "[WARN]:" << msg << std::endl;
  }
  virtual void Error(const std::string &msg) override {
    if (level_ > Logger::Level::ERROR) {
      return;
    }
    std::cerr << "[ERROR]:" << msg << std::endl;
  }
  virtual void Fatal(const std::string &msg) override {
    std::cerr << "[FATAL]:" << msg << std::endl;
  }
};

} // namespace cppnet
