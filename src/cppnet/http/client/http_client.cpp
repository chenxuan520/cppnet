#include "http_client.hpp"
#include "../../utils/const.hpp"

namespace cppnet {

int HttpClient::Init(Address &addr) {
  auto rc = soc_.Init();
  rc = soc_.Connect(addr);
  if (rc != kSuccess) {
    err_msg_ = "[soc.Connect]:" + soc_.err_msg();
    return rc;
  }
  return 0;
}

int HttpClient::Send(HttpReq &req, HttpResp &resp) {
  // step1: build request
  std::string req_str;
  auto rc = req.Build(req_str);
  if (rc != kSuccess) {
    err_msg_ = "[req.Build]:" + req.err_msg();
    return rc;
  }
  // step2: send request
  auto len = soc_.Write(req_str);
  if (len <= 0) {
    err_msg_ = "[soc.Write]:" + soc_.err_msg();
    return rc;
  }
  // step3: read response
  std::string resp_str;
  const std::string kCRLF = "\r\n";
  len = soc_.ReadUntil(resp_str, kCRLF + kCRLF);
  if (len <= 0) {
    err_msg_ = "[logicerr]:empty http response";
    return kLogicErr;
  }

  rc = resp.Parse(resp_str);
  if (rc != kSuccess) {
    err_msg_ = "[resp.Parse]:" + resp.err_msg();
    return rc;
  }

  len = resp.header().GetContentLength();
  std::string body;
  if (len > 0) {
    auto rec_len = soc_.Read(body, len, true);
    if (rec_len != len) {
      err_msg_ = "[logicerr]:invalid http response";
      return kLogicErr;
    }
  }
  resp.body() = body;
  return 0;
}

} // namespace cppnet
