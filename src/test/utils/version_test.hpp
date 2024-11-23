#include "test.h"

#include "utils/version.hpp"

using namespace cppnet;

TEST(Version, Get) {
  std::string version = Version::GetStr();
  DEBUG("version: " << version);
  MUST_TRUE(version != "unknown", "version is unknown");

  std::string date = Version::GetDate();
  DEBUG("date: " << date);
  MUST_TRUE(date != "0000-00-00", "date is 0000-00-00");
}
