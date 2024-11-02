#include "test.h"
#include "utils/file.hpp"
#include <string>

using namespace std;
using namespace cppnet;

TEST(File, ReadWrite) {
  string path = "./test.txt";
  string data = "hello world";
  int rc = File::Write(path, data);
  MUST_TRUE(rc == 0, "write file fail");
  DEFER_DEFAULT {
    File::Remove(path);
    MUST_TRUE(!File::Exist(path), "file not remove");
  };

  string read_data;
  rc = File::Read(path, read_data);
  MUST_TRUE(rc == 0, "read file fail");
  DEBUG("read_data: " << read_data);
  MUST_TRUE(data == read_data, "data not equal");

  // exist
  MUST_TRUE(File::Exist(path), "file not exist");
  MUST_TRUE(!File::Exist("."), "dir exist");
  MUST_TRUE(!File::Exist("./"), "dir exist");
}

TEST(File, Suffix) {
  string path = "./test.txt";
  string suffix = File::Suffix(path);
  MUST_TRUE(suffix == "txt", "suffix not equal " + suffix);

  path = "./test";
  suffix = File::Suffix(path);
  MUST_TRUE(suffix == "", "suffix not equal " + suffix);

  path = "./test/test.txt";
  suffix = File::Suffix(path);
  MUST_TRUE(suffix == "txt", "suffix not equal" + suffix);

  path = "./test/test";
  suffix = File::Suffix(path);
  MUST_TRUE(suffix == "", "suffix not equal " + suffix);

  path = "test.txt";
  suffix = File::Suffix(path);
  MUST_TRUE(suffix == "txt", "suffix not equal " + suffix);

  path = "test";
  suffix = File::Suffix(path);
  MUST_TRUE(suffix == "", "suffix not equal " + suffix);
}

TEST(File, Create) {
  string path = "test.txt";
  int rc = File::Create(path);
  MUST_TRUE(rc == 0, "create file fail");
  DEFER_DEFAULT {
    File::Remove(path);
    MUST_TRUE(!File::Exist(path), "file not remove");
  };

  rc = File::Create(path);
  MUST_TRUE(rc == 0, "create file fail");
  rc = File::Create(path);
  MUST_TRUE(rc == 0, "create file fail");
}

TEST(File, Append) {
  string path = "test.txt";
  string data = "hello world";
  int rc = File::Append(path, data);
  MUST_TRUE(rc == 0, "append file fail");
  DEFER_DEFAULT {
    File::Remove(path);
    MUST_TRUE(!File::Exist(path), "file not remove");
  };

  string read_data;
  rc = File::Read(path, read_data);
  MUST_TRUE(rc == 0, "read file fail");
  DEBUG("read_data: " << read_data);
  MUST_TRUE(data == read_data, "data not equal");

  string append_data = "hello world";
  rc = File::Append(path, append_data);
  MUST_TRUE(rc == 0, "append file fail");

  read_data.clear();
  rc = File::Read(path, read_data);
  MUST_TRUE(rc == 0, "read file fail");
  DEBUG("read_data: " << read_data);
  MUST_TRUE(data + append_data == read_data, "data not equal");
}

TEST(File, LineCount) {
  string path = "test.txt";
  int line_count = File::LineCount(path);
  MUST_TRUE(line_count == 0, "line count not equal");

  string data = "hello world";
  File::Write(path, data);

  DEFER_DEFAULT {
    File::Remove(path);
    MUST_TRUE(!File::Exist(path), "file not remove");
  };

  line_count = File::LineCount(path);
  MUST_TRUE(line_count == 1, "line count not equal, " + to_string(line_count));

  File::Append(path, "\n" + data);
  line_count = File::LineCount(path);
  MUST_TRUE(line_count == 2, "line count not equal, " + to_string(line_count));

  File::Append(path, "\n");
  line_count = File::LineCount(path);
  string content;
  auto rc = File::Read(path, content);
  MUST_TRUE(rc == 0, "read file fail");
  DEBUG(content);

  MUST_TRUE(line_count == 2, "line count not equal, " + to_string(line_count));
}
