#include "http/server/filter/http_host_filter.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

TEST(Filter, Host) {
  HttpHostFilter filter;
  filter.InitRegex(".*");
  HttpReq req;
  req.header().SetHost("www.google.com");

  MUST_TRUE(filter.IsMatchFilter(req), "Host filter failed");

  filter.InitRegex("(\\*|[a-zA-Z0-9\\-\\.]+)\\.google\\.com");
  MUST_TRUE(filter.IsMatchFilter(req), "Host filter failed");

  req.header().SetHost("www.google.com.cn");
  MUST_TRUE(!filter.IsMatchFilter(req), "Host filter failed");

  req.header().SetHost("www.google.com");
  filter.InitRegex(".*\\.google\\.com");
  MUST_TRUE(filter.IsMatchFilter(req), "Host filter failed");

  req.header().SetHost("test.google.com");
  MUST_TRUE(filter.IsMatchFilter(req), "Host filter failed");

  req.header().SetHost("www.google.com.cn");
  MUST_TRUE(!filter.IsMatchFilter(req), "Host filter failed");
}
