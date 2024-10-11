#include "io_multiplexing_factory.hpp"

#ifdef __linux__
#include "epoll.hpp"
#elif __APPLE__
#include "kqueue.hpp"
#elif _WIN32
#include "select.hpp"
#endif

namespace cppnet {

std::shared_ptr<IOMultiplexingBase> IOMultiplexingFactory::CreateDefault() {
#ifdef __linux__
  // linux use epoll
  return std::make_shared<Epoll>();
#elif __APPLE__
  // macos use kqueue
  return std::make_shared<KQueue>();
#elif _WIN32
  // windows use select
  return std::make_shared<Select>();
#endif
}

} // namespace cppnet
