#include "test.h"
#include "utils/threadpool.hpp"
#include <atomic>
#include <unistd.h>

using namespace cppnet;

TEST(ThreadPool, Run) {
  const int loop = 100;

  ThreadPool<int> pool;
  pool.Init();

  std::atomic<int> sum;
  sum = 0;
  int target = 0;

  auto task_func = [&](int num) -> void { sum += num; };

  for (int i = 0; i < loop; i++) {
    target += i;
    auto rc = pool.AddTask({task_func, i});
    MUST_EQUAL(rc, 0);
  }

  // sleep for wait task down
  usleep(10000);
  MUST_TRUE(target == sum, "sum is:" << sum << ", target is:" << target);
}
