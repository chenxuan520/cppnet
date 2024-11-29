#pragma once

#include "test.h"
#include "utils/trie.hpp"
#include <atomic>
#include <memory>

using namespace std;
using namespace cppnet;

TEST(Trie, Test) {
  Trie trie;
  trie.Set("hello", std::make_shared<int>(1));
  trie.Set("world", std::make_shared<int>(2));
  trie.Set("hello.world", std::make_shared<int>(3));
  trie.Set("hello.world.cppnet", std::make_shared<int>(4));
  trie.Set("hello.world.cppnet.cppnet", std::make_shared<int>(5));
  DEBUG(0);

  // test set and get
  auto ptr = trie.Get<int>("hello");
  MUST_TRUE(ptr != nullptr, "Get data failed");
  MUST_TRUE(*ptr == 1, "Get wrong data");
  DEBUG(1);

  ptr = trie.Get<int>("world");
  MUST_TRUE(ptr != nullptr, "Get data failed");
  MUST_TRUE(*ptr == 2, "Get wrong data");

  ptr = trie.Get<int>("hello.world");
  MUST_TRUE(ptr != nullptr, "Get data failed");
  MUST_TRUE(trie.Check("hello.world"), "Get wrong data");
  MUST_TRUE(*ptr == 3, "Get wrong data");

  ptr = trie.Get<int>("hello.world.cppnet.cppnet.cppnet");
  MUST_TRUE(ptr == nullptr, "Get wrong data");

  ptr = trie.Get<int>("hello.world.cppnet.cppnet.cppnet.cppnet");
  MUST_TRUE(ptr == nullptr, "");

  // test search
  atomic<int> count = 0;
  trie.Search<int>("hello.world", [&](shared_ptr<int> data, bool last) -> bool {
    count++;
    return true;
  });
  MUST_TRUE(count == 2, "Search failed");

  count = 0;
  trie.Search<int>("hello.world.cppnet",
                   [&](shared_ptr<int> data, bool last) -> bool {
                     count++;
                     return true;
                   });
  MUST_TRUE(count == 3, "Search failed");

  count = 0;
  trie.Search<int>("hello.world.cppnet.cppnet",
                   [&](shared_ptr<int> data, bool last) -> bool {
                     count++;
                     return true;
                   });
  MUST_TRUE(count == 4, "Search failed");

  count = 0;
  trie.Search<int>("hello.world.cppnet.cppnet.cppnet",
                   [&](shared_ptr<int> data, bool last) -> bool {
                     count++;
                     return true;
                   });
  MUST_TRUE(count == 4, "Search failed");

  count = 0;
  trie.Search<int>("helo;", [&](shared_ptr<int> data, bool last) -> bool {
    count++;
    return true;
  });
  MUST_TRUE(count == 0, "count is " << count);

  // test
  trie.Reset();
  MUST_TRUE(trie.Get<int>("hello") == nullptr, "");
  MUST_TRUE(trie.Get<int>("world") == nullptr, "");
  MUST_TRUE(trie.Get<int>("hello.world") == nullptr, "");
  MUST_TRUE(trie.Get<int>("hello.world.cppnet") == nullptr, "");
}

TEST(Trie, OriginApi) {
  Trie trie;
  trie.SetNode("/temp", 1);
  auto result = trie.GetNode("/temp");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 1, "");

  result = trie.GetNode("/temp/");
  MUST_TRUE(result == nullptr, "");

  trie.SetNode("/temp/api", 2);
  result = trie.GetNode("/temp/api");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 2, "");

  trie.SetNode("/tem", 6);
  result = trie.GetNode("/tem");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 6, "");

  result = trie.GetNode("/temp/api");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 2, "");

  trie.SetNode("/temp/apc", 3);
  result = trie.GetNode("/temp/apc");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 3, "");
  result = trie.GetNode("/temp/api");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 2, "");

  trie.SetNode("", 10);
  result = trie.GetNode("");
  MUST_TRUE(result != nullptr, "");
  MUST_TRUE(std::any_cast<int>(result->data_) == 10, "");

  auto count = 0;
  trie.SearchNode("/temp/apcd",
                  [&](const Trie::Node *node, bool is_end) -> bool {
                    DEBUG("node " << node->key_ << " is_end " << is_end);
                    count++;
                    if (is_end) {
                      ERROR("is_end");
                    }
                    return true;
                  });
  MUST_TRUE(count == 4, "error count of " << count);

  count = 0;
  trie.SearchNode("/temp", [&](const Trie::Node *node, bool is_end) -> bool {
    DEBUG("node " << node->key_ << " is_end " << is_end);
    count++;
    return false;
  });
  MUST_TRUE(count == 1, "error count of " << count);
}
