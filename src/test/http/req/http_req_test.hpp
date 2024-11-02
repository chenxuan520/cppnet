#include "http/req/http_req.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

TEST(HttpReq, Parse) {
  string req =
      "POST /api/users/123?uid=123&txt=%E5%95%8A%E5%95%8A&token=456 "
      "HTTP/1.1\r\n"
      "Host: example.com\r\n"
      "Content-Type: application/json\r\n"
      "Cookie: BAIDUID_BFESS=2F6CC156A9ED4578232A0CE56EA6D92C:FG=1; "
      "BIDUPSID=2F1CC156A9ED4576222A0CE56EA6D92C; PSTM=1740629752; "
      "H_PS_PSSID=60272_60853_609749_61027_61024; BD_HOME=8; BD_UPN=121363; "
      "BA_HECTOR=8l8k242425ag8gslagah2g8g3pp7eq1jibhoh1v; "
      "ZFY=WsFmSErjKZZubvHTlSfpplvzMTR4GBwu1saGRamI5Arac:C\r\n"
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
            std::to_string(http_req.header().GetContentLength()) + " " +
                http_req.err_msg());
  MUST_TRUE(http_req.body() == "{'name':'Alice','age':25}",
            http_req.body() << " " << http_req.body());
  MUST_TRUE(http_req.header().GetCookieVal("BIDUPSID") ==
                "2F1CC156A9ED4576222A0CE56EA6D92C",
            "cookie not exist " + http_req.header().GetCookieVal("BIDUPSID"));
  MUST_TRUE(http_req.header().GetCookieVal("unexist") == "", "cookie exist");
}
