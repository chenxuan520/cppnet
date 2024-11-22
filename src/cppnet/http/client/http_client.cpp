#include "http_client.hpp"
#include "../../utils/const.hpp"
#include <memory>

namespace cppnet {

int HttpClient::Init(Address &addr) {
  if (addr.GetPort() == 0) {
    // default 80 port
    addr = {addr.GetIP(), 80};
  }
  soc_ = std::make_shared<Socket>();
  auto rc = soc_->Init();
  rc = soc_->Connect(addr);
  if (rc != kSuccess) {
    err_msg_ = "[soc.Connect]:" + soc_->err_msg();
    return rc;
  }
  return 0;
}

int HttpClient::Send(HttpReq &req, HttpResp &resp) {
  if (soc_ == nullptr) {
    err_msg_ = "[logicerr]:socket is null";
    return kLogicErr;
  }
  // step1: build request
  std::string req_str;
  auto rc = req.Build(req_str);
  if (rc != kSuccess) {
    err_msg_ = "[req.Build]:" + req.err_msg();
    return rc;
  }
  // step2: send request
  auto len = soc_->Write(req_str);
  if (len <= 0) {
    err_msg_ = "[soc.Write]:" + soc_->err_msg();
    return rc;
  }
  // step3: read response
  std::string resp_str;
  const std::string kCRLF = "\r\n";
  len = soc_->ReadUntil(resp_str, kCRLF + kCRLF);
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
    auto rec_len = soc_->Read(body, len, true);
    if (rec_len != len) {
      err_msg_ = "[logicerr]:invalid http response";
      return kLogicErr;
    }
  }
  resp.body() = body;
  return 0;
}

void HttpClient::Close() {
  if (soc_ != nullptr) {
    soc_->Close();
#ifdef CPPNET_OPENSSL
    if (ssl_context_ != nullptr) {
      ssl_context_->Close();
      // make to ssl_socket ptr and close
      auto ssl_soc = std::dynamic_pointer_cast<SSLSocket>(soc_);
      ssl_soc->CloseSSL();
    }
#endif
  }
}

#ifdef CPPNET_OPENSSL
int HttpClient::InitSSL(Address &addr) {
  if (addr.GetPort() == 0) {
    // default 443 port
    addr = {addr.GetIP(), 443};
  }
  ssl_context_ = std::make_shared<SSLContext>();
  auto rc = ssl_context_->InitCli();
  soc_ = ssl_context_->CreateSSLSocket();
  if (soc_ == nullptr) {
    err_msg_ = "[ssl_soc.CreateSSLSocket]:" + ssl_context_->err_msg();
    return kLogicErr;
  }
  rc = soc_->Connect(addr);
  if (rc != kSuccess) {
    err_msg_ = "[ssl_soc.Connect]:" + soc_->err_msg();
    return rc;
  }
  return 0;
}
#endif

} // namespace cppnet
