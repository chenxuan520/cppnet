#pragma once
#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace cppnet {

template <class T = void *> class ThreadPool {
public: // a struct for you to add task
  struct Task {
    std::function<void(T)> task_;
    T arg_;
  };

private:
  std::mutex que_lock_;
  std::mutex user_lock_;
  std::condition_variable cond_;
  std::queue<Task> task_que_;
  unsigned thread_num_;
  std::atomic<unsigned> busy_now_;
  std::atomic<bool> is_continue_;
  std::vector<std::thread *> arr;
  const char *error;

public:
  ThreadPool(unsigned threadNum = 10) {
    error = NULL;
    this->thread_num_ = threadNum;
    for (unsigned i = 0; i < threadNum; i++)
      arr.push_back(new std::thread(worker, this));
    is_continue_ = true;
    busy_now_ = 0;
  }
  ~ThreadPool() {
    stopPool();
    for (unsigned i = 0; i < arr.size(); i++) {
      arr[i]->join();
      delete arr[i];
    }
  }
  inline void mutexLock() { user_lock_.lock(); }
  inline void mutexUnlock() { user_lock_.unlock(); }
  void addTask(Task task) {
    std::unique_lock<std::mutex> guard(this->que_lock_);
    this->task_que_.push(task);
    this->cond_.notify_one();
  }
  void stopPool() {
    is_continue_ = false;
    this->cond_.notify_all();
  }
  void getBusyAndTask(unsigned &busy, unsigned &task) {
    std::unique_lock<std::mutex> guard(this->que_lock_);
    task = this->task_que_.size();
    busy = this->busy_now_;
  }

private:
  static void worker(void *arg) {
    ThreadPool &pool = *(ThreadPool *)arg;
    Task task = {0, 0};
    while (1) {
      {
        std::unique_lock<std::mutex> guard(pool.que_lock_);
        if (pool.is_continue_ && pool.task_que_.size() == 0)
          pool.cond_.wait(guard, [&]() -> bool {
            return pool.task_que_.size() > 0 || pool.is_continue_ == false;
          });
        if (pool.is_continue_ == false)
          return;
        if (pool.task_que_.size() == 0)
          continue;
        task = pool.task_que_.front();
        pool.task_que_.pop();
      }
      pool.busy_now_++;
      if (task.task_ != NULL)
        task.task_(task.arg_);
      pool.busy_now_--;
    }
  }
};

} // namespace cppnet
