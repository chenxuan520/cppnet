#include "test.h"

#include "utils/date.hpp"

using namespace cppnet;

TEST(Date, GetNow) {
  auto now = Date::GetNow();
  MUST_TRUE(now.find("20") != std::string::npos, "now is unknown " + now);
  DEBUG(now);
}
