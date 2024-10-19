#include "http_resp.hpp"
#include "../../utils/const.hpp"
#include "../../utils/string.hpp"
#include "../status_code/http_status_code.hpp"
#include "../version/http_version.hpp"
#include <string>
#include <vector>

namespace cppnet {

int HttpResp::Parse(const std::string &origin_resp) {
  if (origin_resp.empty()) {
    err_msg_ = "empty http response";
    return kLogicErr;
  }

  // split with \r\n
  std::vector<std::string> single_resp;
  const std::string delimiter = "\r\n";
  StringUtil::Split(origin_resp, delimiter, single_resp);
  if (single_resp.size() < 3) {
    err_msg_ = "invalid http response";
    return kLogicErr;
  }
  // parse status_code and version
  size_t pos = single_resp[0].find(" ");
  if (pos == std::string::npos) {
    err_msg_ = "header format error";
    return kLogicErr;
  }
  std::string version_str = single_resp[0].substr(0, pos);
  version_ = HttpVersionUtil::ConvertToVersion(version_str);
  if (version_ == HttpVersion::UNKNOWN) {
    err_msg_ = "invalid http version";
    return kLogicErr;
  }
  std::string status_str = single_resp[0].substr(pos + 1);
  status_code_ = HttpStatusCodeUtil::ConvertToCode(status_str);
  if (status_code_ == HttpStatusCode::UNKNOWN) {
    err_msg_ = "invalid http status code";
    return kLogicErr;
  }
  // parse header
  // TODO

  // set body
  body_ = origin_resp;

  return kSuccess;
}

int HttpResp::Build(std::string &resp) {
  if (status_code_ == HttpStatusCode::UNKNOWN) {
    err_msg_ = "status code is unknown";
    return kLogicErr;
  }
  resp.clear();

  // add version
  resp += HttpVersionUtil::ConvertToStr(version_) + " ";

  // add status
  auto status_str = HttpStatusCodeUtil::ConvertToStr(status_code_);
  resp += std::to_string((int)status_code_) + " " + status_str + "\r\n";

  // set header size key
  header_.Add("Content-Length", std::to_string(body_.size()));

  // add header
  resp += header_.ToString();

  // add body
  resp += body_;

  return kSuccess;
}

} // namespace cppnet
