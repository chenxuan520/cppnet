#pragma once
#include "utils/const.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace cppnet {

// Because of template,so cannot use cpp file
template <class T = void *> class ThreadPool {
public:
  /**
   * @brief: Task of ThreadPool
   */
  struct Task {
    std::function<void(T)> task_func_;
    T task_arg_;
  };

public:
  /**
   * @brief: Constructor
   * @param thread_num: The number of threads
   */
  ThreadPool(unsigned thread_num) { Init(thread_num); }
  ThreadPool() = default;
  ~ThreadPool() { Stop(); }
  /**
   * @brief: Init
   * @param thread_num: The number of threads
   */
  void Init(unsigned thread_num = 10) {
    threads_.clear();
    this->thread_num_ = thread_num;
    for (unsigned i = 0; i < thread_num; i++) {
      threads_.push_back(std::move(std::thread(Worker, this)));
    }
  }
  /**
   * @brief: Resize number of thread
   * @param thread_num: The number of threads
   */
  void Resize(unsigned thread_num) {
    if (thread_num_ == thread_num || thread_num == 0) {
      return;
    }
    Stop();
    thread_num_ = thread_num;
    threads_.clear();
    Init(thread_num);
  }
  /**
   * @brief: Add task
   * @param task: Task
   */
  int AddTask(Task task) {
    if (!is_continue_ || thread_num_ == 0) {
      err_msg_ = "[logicerr]:threadpoll is not init";
      return kLogicErr;
    }
    std::unique_lock<std::mutex> guard(que_lock_);
    task_que_.push(task);
    cond_.notify_one();
    return kSuccess;
  }
  /**
   * @brief: Stop
   */
  void Stop() {
    if (!is_continue_) {
      return;
    }
    is_continue_ = false;
    this->cond_.notify_all();
    for (unsigned i = 0; i < threads_.size(); i++) {
      threads_[i].join();
    }
  }
  /**
   * @brief: Get busy and task number
   * @param busy: busy number
   * @param task: task number
   */
  void GetBusyAndTaskNum(unsigned &busy, unsigned &task) {
    std::unique_lock<std::mutex> guard(this->que_lock_);
    task = this->task_que_.size();
    busy = this->busy_now_;
  }

public:
  inline void MutexLock() { user_lock_.lock(); }
  inline void MutexUnlock() { user_lock_.unlock(); }
  inline void MutexTryLock() { user_lock_.try_lock(); }

private:
  static void Worker(void *arg) {
    ThreadPool &pool = *(ThreadPool *)arg;
    Task task = {0, 0};
    while (1) {
      {
        std::unique_lock<std::mutex> guard(pool.que_lock_);
        if (pool.is_continue_ && pool.task_que_.size() == 0) {
          pool.cond_.wait(guard, [&]() -> bool {
            return pool.task_que_.size() > 0 || pool.is_continue_ == false;
          });
        }
        if (pool.is_continue_ == false) {
          return;
        }
        if (pool.task_que_.size() == 0) {
          continue;
        }
        task = pool.task_que_.front();
        pool.task_que_.pop();
      }

      pool.busy_now_++;
      if (task.task_func_ != NULL) {
        task.task_func_(task.task_arg_);
      }
      pool.busy_now_--;
    }
  }

private:
  std::mutex que_lock_;
  std::mutex user_lock_;
  std::condition_variable cond_;
  std::queue<Task> task_que_;
  unsigned thread_num_{0};
  std::atomic<unsigned> busy_now_{0};
  std::atomic<bool> is_continue_{true};
  std::vector<std::thread> threads_;
  std::string err_msg_;
};

} // namespace cppnet
