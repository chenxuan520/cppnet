#include "http/client/http_client.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

TEST(HttpClient, Send) {
  HttpClient client;
  Address addr;
  addr.InitWithDomain("www.androidftp.top", 80);
  auto rc = client.Init(addr);
  MUST_TRUE(rc == 0, client.err_msg());
  HttpReq req;
  req.GET("/");
  HttpResp resp;
  rc = client.Send(req, resp);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  std::string resp_str;
  rc = resp.Build(resp_str);
  MUST_TRUE(rc == 0, resp.err_msg());
}

#ifdef CPPNET_OPENSSL
TEST(HttpClient, Https) {
  HttpClient client;
  Address addr;
  addr.InitWithDomain("www.baidu.com", 443);
  auto rc = client.InitSSL(addr);
  MUST_TRUE(rc == 0, client.err_msg());
  HttpReq req;
  req.GET("/");
  req.header().SetHost("www.baidu.com");
  HttpResp resp;
  rc = client.Send(req, resp);
  string resp_str;
  rc = resp.Build(resp_str);
  MUST_TRUE(rc == 0, client.err_msg());
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()) + "\r\n" +
                resp_str);
  MUST_TRUE(rc == 0, resp.err_msg());
  DEBUG(resp_str);
}
#endif
