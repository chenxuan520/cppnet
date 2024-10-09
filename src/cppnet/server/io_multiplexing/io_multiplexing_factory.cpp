#include "io_multiplexing_factory.hpp"
#include "epoll.hpp"

namespace cppnet {

std::shared_ptr<IOMultiplexingBase> IOMultiplexingFactory::CreateDefault() {
#ifdef __linux__
  // linux use epoll
  return std::make_shared<Epoll>();
#elif __APPLE__
  // macos use kqueue
  return nullptr;
#elif _WIN32
  // windows use select
  return nullptr;
#endif
}

} // namespace cppnet
