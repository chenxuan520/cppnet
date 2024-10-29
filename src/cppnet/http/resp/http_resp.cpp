#include "http_resp.hpp"
#include "../../utils/const.hpp"
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
  auto first_space_pos = first_line.find(" ");
  if (first_space_pos == std::string::npos) {
    err_msg_ = "first line format error";
    return kLogicErr;
  }
  auto second_space_pos = first_line.find(" ", first_space_pos + 1);
  if (second_space_pos == std::string::npos) {
    err_msg_ = "first line format error";
    return kLogicErr;
  }
  first_line_vec.push_back(first_line.substr(0, first_space_pos));
  first_line_vec.push_back(first_line.substr(
      first_space_pos + 1, second_space_pos - first_space_pos - 1));
  first_line_vec.push_back(first_line.substr(second_space_pos + 1));

  int code = 0;
  try {
    code = std::stoi(first_line_vec[1]);
  } catch (std::exception &e) {
    err_msg_ = "status code format error";
    return kLogicErr;
  }
  status_code_ = HttpStatusCodeUtil::ConvertToCode(code);
  version_ = HttpVersionUtil::ConvertToVersion(first_line_vec[0]);

  // parse header
  auto rc = header_.Parse(header);
  if (rc != kSuccess) {
    err_msg_ = header_.err_msg() + "\n" + header;
    return rc;
  }

  return kSuccess;
}

int HttpResp::Build(std::string &resp_str) {
  if (status_code_ == HttpStatusCode::UNKNOWN) {
    err_msg_ = "status code is unknown";
    return kLogicErr;
  }
  resp_str.clear();

  // add version
  resp_str += HttpVersionUtil::ConvertToStr(version_) + " ";

  // add status
  auto status_str = HttpStatusCodeUtil::ConvertToStr(status_code_);
  resp_str += std::to_string((int)status_code_) + " " + status_str + "\r\n";

  // set header size key
  header_.SetContentLength(body_.size());

  // add header
  resp_str += header_.ToString();

  // add body
  resp_str += body_;

  return kSuccess;
}

void HttpResp::Clear() {
  status_code_ = HttpStatusCode::UNKNOWN;
  header_.Clear();
  body_.clear();
}

void HttpResp::NotFound() {
  status_code_ = HttpStatusCode::NOT_FOUND;
  body_ = "<html><body><h1>404 Not Found</h1></body></html>";
  header_.SetContentType(HttpHeader::ContentType::kTextHtml);
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
  header_.Add("Location", location);
}

void HttpResp::SuccessWithBinary(const std::string &body) {
  status_code_ = HttpStatusCode::OK;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kApplicationOctetStream);
}

void HttpResp::SuccessWithJson(const std::string &body) {
  status_code_ = HttpStatusCode::OK;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kApplicationJson);
}

void HttpResp::Success(HttpHeader::ContentType content_type,
                       const std::string &body) {
  if (!body.empty()) {
    body_ = body;
  }
  status_code_ = HttpStatusCode::OK;
  header_.SetContentType(content_type);
}

void HttpResp::Json(HttpStatusCode status_code, const std::string &body) {
  status_code_ = status_code;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kApplicationJson);
}

void HttpResp::Text(HttpStatusCode status_code, const std::string &body) {
  status_code_ = status_code;
  body_ = body;
  header_.SetContentType(HttpHeader::ContentType::kTextPlain);
}

} // namespace cppnet
