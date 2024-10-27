
#include "http_version.hpp"

namespace cppnet {

std::string HttpVersionUtil::ConvertToStr(const HttpVersion &version) {
  switch (version) {
  case HttpVersion::HTTP_1_0:
    return "HTTP/1.0";
  case HttpVersion::HTTP_1_1:
    return "HTTP/1.1";
  case HttpVersion::HTTP_2_0:
    return "HTTP/2.0";
  default:
    return "UNKNOWN";
  }
}

HttpVersion HttpVersionUtil::ConvertToVersion(const std::string &version) {
  if (version == "HTTP/1.0")
    return HttpVersion::HTTP_1_0;
  else if (version == "HTTP/1.1")
    return HttpVersion::HTTP_1_1;
  else if (version == "HTTP/2.0")
    return HttpVersion::HTTP_2_0;
  else
    return HttpVersion::UNKNOWN;
}

} // namespace cppnet
