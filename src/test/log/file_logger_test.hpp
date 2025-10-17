#include "log/file_logger.hpp"
#include "test.h"
#include "utils/file.hpp"
#include <string>

using namespace cppnet;
using namespace std;

TEST(FileLogger, Debug) {
  const std::string file_path = "file_test_debug.log";
  const std::string write_msg = "test";

  auto logger = std::make_shared<FileLogger>();
  auto rc = logger->Init(file_path);
  ASSERT_EQ(rc, 0);

  DEFER_DEFAULT {
    File::Remove(file_path);
    ASSERT_EQ(File::Exist(file_path), false);
  };

  DEFER_DEFAULT {
    logger->Stop();
    string file_content = "";
    auto rc = File::Read(file_path, file_content);
    ASSERT_EQ(rc, 0);

    DEBUG(file_content);
  };

  logger->set_level(Logger::Level::kDebug);
  logger->Debug(write_msg);
}

TEST(FileLogger, FirstWrite) {
  const std::string file_path = "file_test_first_write.log";
  const std::string write_msg = "test";

  auto logger = std::make_shared<FileLogger>();
  logger->set_buf_size(1024);
  logger->set_buf_num(1);

  auto rc = logger->Init(file_path);
  ASSERT_EQ(rc, 0);

  DEFER_DEFAULT {
    File::Remove(file_path);
    ASSERT_EQ(File::Exist(file_path), false);
  };

  const int write_time = 1000;
  for (int i = 0; i < write_time; i++) {
    logger->Debug(write_msg + to_string(i));
    logger->Info(write_msg + to_string(i));
    logger->Warn(write_msg + to_string(i));
    logger->Error(write_msg + to_string(i));
    logger->Fatal(write_msg + to_string(i));
  }

  logger->Stop();
  DEBUG("buf_num " << logger->buf_num());

  auto count = File::LineCount(file_path);
  MUST_TRUE(count == write_time * 5,
            to_string(count) + " != " + to_string(write_time * 5));
}

BENCHMARK(FileLogger, WriteRate) {
#ifndef __linux__
  // not use mac for benchmark
  SKIP()
#endif
  const std::string file_path = "file_benct_test.log";
  const std::string write_msg = "[test]:test-for-benchmark";
  auto logger = std::make_shared<FileLogger>();
  logger->Init(file_path);

  DEFER_DEFAULT { File::Remove(file_path); };

  BENCHFUNC([&]() { logger->Debug(write_msg); });
}
