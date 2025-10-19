#include "file_logger.hpp"
#include "../utils/const.hpp"
#include "../utils/file.hpp"
#include "utils/date.hpp"

namespace cppnet {

void FileLogger::Stop() {
  // stop first avoid multi-thread write
  thread_pool_.Stop();

  if (cur_buffer_ == nullptr) {
    return;
  }

  // flush all buffers
  cur_buffer_ = buffers_list_;
  for (int i = 0; i < buf_num_; ++i) {
    if (!cur_buffer_->data.empty()) {
      FlushBuffer(file_path_, cur_buffer_);
    }
    cur_buffer_ = cur_buffer_->next;
  }
  // for delete shared_ptr memory
  cur_buffer_ = nullptr;
  buffers_list_->next = nullptr;
}

int FileLogger::Init(const std::string &file_path) {
  file_path_ = file_path;
  // is exist file_path_, check write permission
  // if not exist, create file
  auto rc = File::Create(file_path_);
  if (rc != kSuccess) {
    return rc;
  }
  rc = File::CanWrite(file_path_);
  if (!rc) {
    return rc;
  }
  thread_pool_.Init(1);
  // init buffers
  buffers_list_ = std::make_shared<Buffer>();
  cur_buffer_ = buffers_list_;
  cur_buffer_->data.reserve(buf_size_);
  cur_buffer_->last_flush_time = time(nullptr);

  for (int i = 1; i < buf_num_; ++i) {
    cur_buffer_->next = std::make_shared<Buffer>();
    cur_buffer_ = cur_buffer_->next;
    cur_buffer_->data.reserve(buf_size_);
    cur_buffer_->last_flush_time = time(nullptr);
  }
  cur_buffer_->next = buffers_list_;
  cur_buffer_ = buffers_list_;
  return kSuccess;
}

void FileLogger::Debug(const std::string &msg) {
  if (level_ > Logger::Level::kDebug || cur_buffer_ == nullptr) {
    return;
  }
  cur_buffer_->data += "[DEBUG]" + Date::GetNow() + "|" + msg + "\n";
  CheckFlush();
}

void FileLogger::Info(const std::string &msg) {
  if (level_ > Logger::Level::kInfo || cur_buffer_ == nullptr) {
    return;
  }
  cur_buffer_->data += "[INFO]" + Date::GetNow() + "|" + msg + "\n";
  CheckFlush();
}

void FileLogger::Warn(const std::string &msg) {
  if (level_ > Logger::Level::kWarn || cur_buffer_ == nullptr) {
    return;
  }
  cur_buffer_->data += "[WARN]" + Date::GetNow() + "|" + msg + "\n";
  CheckFlush();
}

void FileLogger::Error(const std::string &msg) {
  if (level_ > Logger::Level::kError || cur_buffer_ == nullptr) {
    return;
  }
  cur_buffer_->data += "[ERROR]" + Date::GetNow() + "|" + msg + "\n";
  CheckFlush();
}

void FileLogger::Fatal(const std::string &msg) {
  if (level_ > Logger::Level::kFatal || cur_buffer_ == nullptr) {
    return;
  }
  cur_buffer_->data += "[FATAL]|" + msg + "\n";
  CheckFlush();
}

void FileLogger::CheckFlush() {
  if (cur_buffer_->data.size() > buf_size_ ||
      (cur_buffer_->last_flush_time + flush_interval_sec_ < time(nullptr) &&
       cur_buffer_->data != "")) {
    cur_buffer_->need_flush = true;
    auto dir_buf = cur_buffer_;

    if (cur_buffer_->next->need_flush) {
      // all buffers are full, create new buffer
      auto next_buf = std::make_shared<Buffer>();
      next_buf->next = cur_buffer_->next;
      cur_buffer_->next = next_buf;
      cur_buffer_ = next_buf;
      cur_buffer_->data.reserve(buf_size_);
      cur_buffer_->last_flush_time = time(nullptr);
      buf_num_++;
    } else {
      cur_buffer_ = cur_buffer_->next;
    }

    // flush the dir buffer
    auto task = [this, dir_buf]() { FlushBuffer(file_path_, dir_buf); };
    thread_pool_.AddTask(ThreadPool::Task{task});
  }
}

void FileLogger::FlushBuffer(std::string file_path,
                             std::shared_ptr<Buffer> buffer) {
  if (buffer->data.empty()) {
    return;
  }
  auto rc = File::Append(file_path, buffer->data);
  if (rc != kSuccess) {
    return;
  }
  buffer->data.clear();
  buffer->need_flush = false;
  buffer->last_flush_time = time(nullptr);
}

} // namespace cppnet
