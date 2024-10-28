
#include "http_req.hpp"
#include "../../utils/const.hpp"
#include "../../utils/string.hpp"
#include <vector>

namespace cppnet {

int HttpReq::Parse(const std::string &origin_req) {
  if (origin_req.empty()) {
    err_msg_ = "[logicerr]:empty http request";
    return kLogicErr;
  }
  // find \r\n\r\n
  const std::string kCRLF = "\r\n";
  auto header_end_pos = origin_req.find(kCRLF + kCRLF);
  if (header_end_pos == std::string::npos) {
    err_msg_ = "[logicerr]:can't find endof header";
    return kLogicErr;
  }
  // find first \r\n
  auto line_end_pos = origin_req.find(kCRLF);
  if (line_end_pos == std::string::npos) {
    err_msg_ = "[logicerr]:can't find endof line";
    return kLogicErr;
  }
  // parse header
  if (!header_.Parse(
          origin_req.substr(line_end_pos + kCRLF.size(), header_end_pos))) {
    err_msg_ = header_.err_msg();
    return kLogicErr;
  }
  // set body
  body_ = origin_req.substr(header_end_pos + kCRLF.size() + kCRLF.size());
  // split first line with space
  auto first_line = origin_req.substr(0, line_end_pos);
  std::vector<std::string> single_line;
  StringUtil::Split(first_line, " ", single_line);
  if (single_line.size() != 3) {
    err_msg_ = "[logicerr]:invalid http request";
    return kLogicErr;
  }
  method_ = HttpMethodUtil::ConvertToMethod(single_line[0]);
  if (method_ == HttpMethod::UNKNOWN) {
    err_msg_ = "[logicerr]:invalid http method";
    return kLogicErr;
  }
  auto ret = route_.Parse(single_line[1]);
  if (ret != kSuccess) {
    err_msg_ = route_.err_msg();
    return kLogicErr;
  }
  version_ = HttpVersionUtil::ConvertToVersion(single_line[2]);
  if (version_ == HttpVersion::UNKNOWN) {
    err_msg_ = "[logicerr]:invalid http version";
    return kLogicErr;
  }

  return kSuccess;
}

int HttpReq::Build(std::string &req_str) {
  if (method_ == HttpMethod::UNKNOWN || version_ == HttpVersion::UNKNOWN) {
    err_msg_ = "[logicerr]:invalid http request";
    return kLogicErr;
  }
  if (header_.Get("Host").empty()) {
    header_.Add("Host", "localhost");
  }
  header_.SetContentLength(body_.size());

  req_str = HttpMethodUtil::ConvertToStr(method_) + " " + route_.ToString() +
            " " + HttpVersionUtil::ConvertToStr(version_) + "\r\n";
  req_str += header_.ToString();
  req_str += body_;
  return kSuccess;
}

void HttpReq::Clear() {
  method_ = HttpMethod::UNKNOWN;
  version_ = HttpVersion::UNKNOWN;
  header_.Clear();
  route_.Clear();
  body_.clear();
  err_msg_.clear();
}

void HttpReq::GET(const std::string &url, const KVMappings &params,
                  const KVMappings &extra_header) {
  method_ = HttpMethod::GET;
  version_ = HttpVersion::HTTP_1_1;
  route_.SetPath(url);
  for (auto &it : params) {
    route_.AddParam(it.first, it.second);
  }
  for (auto &it : extra_header) {
    header_.Add(it.first, it.second);
  }
}

void HttpReq::POST(const std::string &url, const std::string &body,
                   const KVMappings &extra_header) {
  method_ = HttpMethod::POST;
  version_ = HttpVersion::HTTP_1_1;
  route_.SetPath(url);
  body_ = body;
  for (auto &it : extra_header) {
    header_.Add(it.first, it.second);
  }
}

} // namespace cppnet
