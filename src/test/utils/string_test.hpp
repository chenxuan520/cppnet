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

TEST(StringUtil, UrlCode) {
  std::string str = "/a/b/c?a=1&b=%E5%95%8A%E5%95%8A&c=3";
  DEBUG(str);
  std::string res = StringUtil::UrlDecode(str);
  DEBUG(res);

  std::string res2 = StringUtil::UrlEncode(res);
  DEBUG(res2);

  ASSERT_EQ(res2, str);
}

TEST(StringUtil, WildCardMatch) {
  MUST_TRUE(
      StringUtil::WildCardMatch("*.chenxuanweb.top", "www.chenxuanweb.top"),
      "not match");

  MUST_TRUE(StringUtil::WildCardMatch("*chenxuanweb.top", "chenxuanweb.top"),
            "not match");

  MUST_TRUE(
      StringUtil::WildCardMatch("*.chenxuanweb.top", "blog.chenxuanweb.top"),
      "not match");

  MUST_TRUE(!StringUtil::WildCardMatch("*.chenxuanweb", "www.chenxuanweb.top"),
            "not match");
}
