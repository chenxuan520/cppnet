#pragma once

#include "test.h"
#include "utils/host.hpp"

using namespace cppnet;

TEST(Host, IPAndHostName) {
  auto ip = Host::GetLocalIP();
  auto hostname = Host::GetLocalName();
  DEBUG(ip);
  DEBUG(hostname);
}
