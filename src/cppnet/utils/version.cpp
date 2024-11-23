#include "version.hpp"

namespace cppnet {

std::string Version::GetStr() {
#ifndef CPPNET_VERSION
  return "unknown";
#else
  return CPPNET_VERSION;
#endif
}

std::string Version::GetDate() {
#ifndef CPPNET_VERSION_DATE
  return "0000-00-00";
#else
  return CPPNET_VERSION_DATE;
#endif
}

} // namespace cppnet
