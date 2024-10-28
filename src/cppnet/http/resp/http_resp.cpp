#include "http_resp.hpp"
#include "../../utils/const.hpp"
#include "../../utils/string.hpp"
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
  const std::string delimiter = "\r\n";
  auto pos = origin_resp.find(delimiter);
  auto header_end_pos = origin_resp.find(delimiter + delimiter);
  if (pos == std::string::npos) {
    err_msg_ = "header format error";
    return kLogicErr;
  }
  std::string first_line = origin_resp.substr(0, pos);
  std::string header = origin_resp.substr(
      pos + delimiter.size(), header_end_pos - pos - delimiter.size());
  body_ = origin_resp.substr(origin_resp.find(delimiter + delimiter) +
                             2 * delimiter.size());

  // parse status_code and version
  std::vector<std::string> first_line_vec;
  StringUtil::Split(first_line, " ", first_line_vec);
  if (first_line_vec.size() != 3) {
    err_msg_ = "status code format error";
    return kLogicErr;
  }
  status_code_ = HttpStatusCodeUtil::ConvertToCode(first_line_vec[2]);
  version_ = HttpVersionUtil::ConvertToVersion(first_line_vec[0]);

  // parse header
  auto rc = header_.Parse(header);
  if (rc != kSuccess) {
    err_msg_ = header_.err_msg() + "\n" + header;
    return rc;
  }

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

void HttpResp::NotFound() {
  status_code_ = HttpStatusCode::NOT_FOUND;
  body_ = "<html><body><h1>404 Not Found</h1></body></html>";
  header_.SetContentType(HttpHeader::ContentType::kTextHtml);
  header_.SetHost("cppnet");
}

void HttpResp::Redirect(const std::string &location, bool forever) {
  if (forever) {
    status_code_ = HttpStatusCode::MOVED_PERMANENTLY;
    body_ = "<html><body><h1>301 Moved Permanently</h1></body></html>";
  } else {
    status_code_ = HttpStatusCode::FOUND;
    body_ = "<html><body><h1>302 Found</h1></body></html>";
  }
  header_.SetContentType(HttpHeader::ContentType::kTextHtml);
  header_.SetHost("cppnet");
  header_.Add("Location", location);
}

void HttpResp::SuccessWithBinary(const std::string &body) {
  status_code_ = HttpStatusCode::OK;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kApplicationOctetStream);
  header_.SetHost("cppnet");
}

void HttpResp::SuccessWithJson(const std::string &body) {
  status_code_ = HttpStatusCode::OK;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kApplicationJson);
  header_.SetHost("cppnet");
}

void HttpResp::Json(HttpStatusCode status_code, const std::string &body) {
  status_code_ = status_code;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kApplicationJson);
  header_.SetHost("cppnet");
}

void HttpResp::Text(HttpStatusCode status_code, const std::string &body) {
  status_code_ = status_code;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kTextPlain);
  header_.SetHost("cppnet");
}

} // namespace cppnet
