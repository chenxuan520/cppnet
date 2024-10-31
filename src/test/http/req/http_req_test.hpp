#include "http/req/http_req.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

TEST(HttpReq, Parse) {
  string req = "POST /api/users/123?uid=123&txt=%E5%95%8A%E5%95%8A&token=456 "
               "HTTP/1.1\r\n"
               "Host: example.com\r\n"
               "Content-Type: application/json\r\n"
               "Content-Length: 25\r\n"
               "\r\n"
               "{'name':'Alice','age':25}";
  HttpReq http_req;
  auto rc = http_req.Parse(req);
  MUST_TRUE(rc == 0, http_req.err_msg());
  MUST_TRUE(http_req.method() == HttpMethod::POST,
            "get wrong method" +
                HttpMethodUtil::ConvertToStr(http_req.method()));
  MUST_TRUE(http_req.route().GetPath() == "/api/users/123",
            http_req.route().GetPath());
  MUST_TRUE(http_req.route().GetParam("uid") == "123",
            http_req.route().GetParam("uid"))
  MUST_TRUE(http_req.route().GetParam("token") == "456",
            http_req.route().GetParam("token"))
  MUST_TRUE(http_req.route().GetParam("txt") == "啊啊",
            http_req.route().GetParam("txt"))
  MUST_TRUE(http_req.route().GetParam("unexist") == "",
            http_req.route().GetParam("unexist"))
  MUST_TRUE(http_req.header().GetContentLength() == 25,
            http_req.header().GetContentLength());
  MUST_TRUE(http_req.body() == "{'name':'Alice','age':25}",
            http_req.body() << " " << http_req.body());
}
