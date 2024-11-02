#include "multi_logger.hpp"

namespace cppnet {

void MultiLogger::Init(std::initializer_list<std::shared_ptr<Logger>> loggers) {
  for (auto &logger : loggers) {
    loggers_.push_back(logger);
  }
}

void MultiLogger::Debug(const std::string &msg) {
  for (auto &logger : loggers_) {
    logger->Debug(msg);
  }
}

void MultiLogger::Info(const std::string &msg) {
  for (auto &logger : loggers_) {
    logger->Info(msg);
  }
}

void MultiLogger::Warn(const std::string &msg) {
  for (auto &logger : loggers_) {
    logger->Warn(msg);
  }
}

void MultiLogger::Error(const std::string &msg) {
  for (auto &logger : loggers_) {
    logger->Error(msg);
  }
}

void MultiLogger::Fatal(const std::string &msg) {
  for (auto &logger : loggers_) {
    logger->Fatal(msg);
  }
}

} // namespace cppnet
