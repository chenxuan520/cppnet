#include "http/resp/http_resp.hpp"
#include "test.h"

using namespace cppnet;
using namespace std;

TEST(HttpResp, Parse) {
  HttpResp resp;
  auto rc = resp.Parse(
      "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: "
      "21\r\n\r\n{'message':'Success'}");
  MUST_TRUE(rc == 0, resp.err_msg());

  MUST_TRUE(resp.version() == HttpVersion::HTTP_1_1,
            "get wrong version " +
                HttpVersionUtil::ConvertToStr(resp.version()));
  MUST_TRUE(resp.status_code() == HttpStatusCode::OK,
            "get wrong status code " +
                HttpStatusCodeUtil::ConvertToStr(resp.status_code()));
  MUST_TRUE(resp.body() == "{'message':'Success'}",
            "get wrong body " + resp.body());
}
