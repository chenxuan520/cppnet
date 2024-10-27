#pragma once

#include "test.h"
#include "utils/trie.hpp"
#include <atomic>
#include <memory>

using namespace std;
using namespace cppnet;

TEST(Trie, Test) {
  Trie<int> trie;
  trie.Set("hello", std::make_shared<int>(1));
  trie.Set("world", std::make_shared<int>(2));
  trie.Set("hello.world", std::make_shared<int>(3));
  trie.Set("hello.world.cppnet", std::make_shared<int>(4));
  trie.Set("hello.world.cppnet.cppnet", std::make_shared<int>(5));

  // test set and get
  auto ptr = trie.Get("hello");
  MUST_TRUE(ptr != nullptr, "Get data failed");
  MUST_TRUE(*ptr == 1, "Get wrong data");

  ptr = trie.Get("world");
  MUST_TRUE(ptr != nullptr, "Get data failed");
  MUST_TRUE(*ptr == 2, "Get wrong data");

  ptr = trie.Get("hello.world");
  MUST_TRUE(ptr != nullptr, "Get data failed");
  MUST_TRUE(*ptr == 3, "Get wrong data");

  ptr = trie.Get("hello.world.cppnet.cppnet.cppnet");
  MUST_TRUE(ptr == nullptr, "Get wrong data");

  ptr = trie.Get("hello.world.cppnet.cppnet.cppnet.cppnet");
  MUST_TRUE(ptr == nullptr, "");

  // test search
  atomic<int> count = 0;
  trie.Search("hello.world", [&](shared_ptr<int> data, bool last) -> bool {
    count++;
    return true;
  });
  MUST_TRUE(count == 2, "Search failed");

  count = 0;
  trie.Search("hello.world.cppnet",
              [&](shared_ptr<int> data, bool last) -> bool {
                count++;
                return true;
              });
  MUST_TRUE(count == 3, "Search failed");

  count = 0;
  trie.Search("hello.world.cppnet.cppnet",
              [&](shared_ptr<int> data, bool last) -> bool {
                count++;
                return true;
              });
  MUST_TRUE(count == 4, "Search failed");

  count = 0;
  trie.Search("hello.world.cppnet.cppnet.cppnet",
              [&](shared_ptr<int> data, bool last) -> bool {
                count++;
                return true;
              });
  MUST_TRUE(count == 4, "Search failed");

  count = 0;
  trie.Search("helo;", [&](shared_ptr<int> data, bool last) -> bool {
    count++;
    return true;
  });
  MUST_TRUE(count == 0, "count is " << count);

  // test
  trie.Reset();
  MUST_TRUE(trie.Get("hello") == nullptr, "");
  MUST_TRUE(trie.Get("world") == nullptr, "");
  MUST_TRUE(trie.Get("hello.world") == nullptr, "");
  MUST_TRUE(trie.Get("hello.world.cppnet") == nullptr, "");
}
