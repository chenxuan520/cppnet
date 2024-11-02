#include "threadpool.hpp"
#include "const.hpp"

namespace cppnet {

void ThreadPool::Init(unsigned thread_num) {
  threads_.clear();
  this->thread_num_ = thread_num;
  for (unsigned i = 0; i < thread_num; i++) {
    threads_.push_back(std::move(std::thread(Worker, this)));
  }
}

void ThreadPool::Resize(unsigned thread_num) {
  if (thread_num_ == thread_num || thread_num == 0) {
    return;
  }
  Stop();
  thread_num_ = thread_num;
  threads_.clear();
  Init(thread_num);
}

int ThreadPool::AddTask(Task task) {
  if (!is_continue_ || thread_num_ == 0) {
    err_msg_ = "[logicerr]:threadpoll is not init";
    return kLogicErr;
  }
  std::unique_lock<std::mutex> guard(que_lock_);
  task_que_.push(task);
  cond_.notify_one();
  return kSuccess;
}

void ThreadPool::Stop() {
  if (!is_continue_) {
    return;
  }
  is_continue_ = false;
  this->cond_.notify_all();
  for (unsigned i = 0; i < threads_.size(); i++) {
    threads_[i].join();
  }
}

void ThreadPool::GetBusyAndTaskNum(unsigned &busy, unsigned &task) {
  std::unique_lock<std::mutex> guard(this->que_lock_);
  task = this->task_que_.size();
  busy = this->busy_now_;
}

void ThreadPool::Worker(void *arg) {
  ThreadPool &pool = *(ThreadPool *)arg;
  Task task = {nullptr};
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
    if (task.task_func != NULL) {
      task.task_func();
    }
    pool.busy_now_--;
  }
}

} // namespace cppnet
