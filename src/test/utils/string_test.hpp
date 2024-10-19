#pragma once

#include "test.h"
#include "utils/string.hpp"

using namespace cppnet;

TEST(StringUtil, Split) {
  std::vector<std::string> arr;
  StringUtil::Split("a,b,c", ",", arr);
  ASSERT_EQ(arr.size(), 3);
  ASSERT_EQ(arr[0], "a");
  ASSERT_EQ(arr[1], "b");
  ASSERT_EQ(arr[2], "c");

  arr.clear();
  StringUtil::Split("a,b,c", ",", arr, true);
  ASSERT_EQ(arr.size(), 3);
  ASSERT_EQ(arr[0], "a,");
  ASSERT_EQ(arr[1], "b,");
  ASSERT_EQ(arr[2], "c");

  arr.clear();
  StringUtil::Split("a,b,c", ",b", arr);
  ASSERT_EQ(arr.size(), 2);
  ASSERT_EQ(arr[0], "a");
  ASSERT_EQ(arr[1], ",c");

  arr.clear();
  StringUtil::Split("a,b,c", "c,b", arr);
  ASSERT_EQ(arr.size(), 1);
  ASSERT_EQ(arr[0], "a,b,c");
}
