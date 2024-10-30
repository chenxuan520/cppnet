#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace cppnet {

class ThreadPool {
public:
  /**
   * @brief: Task of ThreadPool
   */
  struct Task {
    std::function<void()> task_func_;
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
  void Init(unsigned thread_num = 10);
  /**
   * @brief: Resize number of thread
   * @param thread_num: The number of threads
   */
  void Resize(unsigned thread_num);
  /**
   * @brief: Add task
   * @param task: Task
   */
  int AddTask(Task task);
  /**
   * @brief: Stop
   */
  void Stop();
  /**
   * @brief: Get busy and task number
   * @param busy: busy number
   * @param task: task number
   */
  void GetBusyAndTaskNum(unsigned &busy, unsigned &task);

public:
  inline void MutexLock() { user_lock_.lock(); }
  inline void MutexUnlock() { user_lock_.unlock(); }
  inline void MutexTryLock() { user_lock_.try_lock(); }

private:
  static void Worker(void *arg);

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
