#include "http_route.hpp"
#include "../../utils/const.hpp"
#include "../../utils/string.hpp"
#include <vector>

namespace cppnet {

int HttpRoute::Parse(const std::string &origin_path) {
  if (origin_path.empty()) {
    err_msg_ = "path is empty";
    return kLogicErr;
  }

  int param_start = origin_path.find('?');
  if (param_start == std::string::npos) {
    path_ = origin_path;
    return kSuccess;
  }

  // split with &
  std::string origin_param = origin_path.substr(param_start + 1);
  std::vector<std::string> single_param;
  const std::string delimiter = "&";
  StringUtil::Split(origin_param, delimiter, single_param);
  // parse param
  for (auto &it : single_param) {
    size_t pos = it.find("=");
    if (pos == std::string::npos) {
      err_msg_ = "param format error";
      return kLogicErr;
    }
    std::string key = it.substr(0, pos);
    std::string value = it.substr(pos + 1);
    params_[key] = value;
  }
  path_ = origin_path.substr(0, param_start);

  return kSuccess;
}

std::string HttpRoute::GetParam(const std::string &key) const {
  auto it = params_.find(key);
  if (it != params_.end())
    return it->second;
  else
    return "";
}

std::string HttpRoute::ToString() const {
  std::string ret = path_;
  if (!params_.empty()) {
    ret += "?";
    for (auto &it : params_) {
      ret += it.first + "=" + it.second + "&";
    }
    ret.pop_back();
  }
  return ret;
}

} // namespace cppnet
