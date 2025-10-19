#pragma once
#include "../utils/threadpool.hpp"
#include "logger.hpp"
#include <atomic>
#include <memory>

namespace cppnet {

/**
 * @note: this class not Thread-safe, do not use in multithreaded situations
 */
class FileLogger : public Logger {
public:
  /*
   * @brief: Initialize the file logger.
   * @param file_path: The file path.
   * @return: 0 if success, -1 if failed.
   */
  int Init(const std::string &file_path);
  /*
   * @brief: stop the file logger.
   */
  void Stop();
  ~FileLogger() { Stop(); }
  virtual void Debug(const std::string &msg) override;
  virtual void Info(const std::string &msg) override;
  virtual void Warn(const std::string &msg) override;
  virtual void Error(const std::string &msg) override;
  virtual void Fatal(const std::string &msg) override;

public:
  /*
   * @brief: Set the number of buffers.
   * @param buf_num: The number of buffers.
   */
  void set_buf_num(int buf_num) { buf_num_ = buf_num; }
  /*
   * @brief: Set the size of buffer.
   * @param buf_size: The size of buffer.
   */
  void set_buf_size(int buf_size) { buf_size_ = buf_size; }
  /*
   * @brief: Set the flush interval in seconds.
   * @param flush_interval_sec: The flush interval in seconds.
   */
  void set_flush_interval_sec(int flush_interval_sec) {
    flush_interval_sec_ = flush_interval_sec;
  }
  int buf_num() const { return buf_num_; }
  int buf_size() const { return buf_size_; }
  int flush_interval_sec() const { return flush_interval_sec_; }

private:
  struct Buffer {
    std::string data{""};
    std::atomic<bool> need_flush{false};
    int64_t last_flush_time{0};
    std::shared_ptr<Buffer> next{nullptr};
  };

  static void FlushBuffer(std::string file_path,
                          std::shared_ptr<Buffer> buffer);
  void CheckFlush();

private:
  std::string file_path_;
  int buf_num_ = 4;
  int buf_size_ = 1024;
  int flush_interval_sec_ = 3;
  ThreadPool thread_pool_;
  std::shared_ptr<Buffer> buffers_list_ = nullptr;
  std::shared_ptr<Buffer> cur_buffer_ = nullptr;
};

} // namespace cppnet
