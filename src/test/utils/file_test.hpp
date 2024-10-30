#include "test.h"
#include "utils/file.hpp"

using namespace std;
using namespace cppnet;

TEST(File, ReadWrite) {
  string path = "./test.txt";
  string data = "hello world";
  int rc = File::Write(path, data);
  MUST_TRUE(rc == 0, "write file fail");
  DEFER_DEFAULT {
    remove(path.c_str());
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
