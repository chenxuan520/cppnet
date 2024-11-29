#pragma once

#include <any>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace cppnet {

// Because of template,so cannot use cpp file
class Trie {
public:
  Trie();

  ~Trie();

  /**
   * @brief: Set key and value
   * @param: key: key
   * @param: data: data
   * @return: true if success
   */
  template <class T> void Set(const std::string &key, std::shared_ptr<T> data) {
    SetNode(key, data);
  }
  /**
   * @brief: Get value by key
   * @param: key: key
   * @return: value
   */
  template <class T> std::shared_ptr<T> Get(const std::string &key) const {
    auto node = GetNode(key);
    if (node == nullptr) {
      return nullptr;
    }
    return std::any_cast<std::shared_ptr<T>>(node->data_);
  }
  /**
   * @brief: Search value by key
   * @param: key: key
   * @param: func: function callback
   * @return: value
   */
  template <class T>
  void Search(const std::string &key,
              std::function<bool(std::shared_ptr<T>, bool)> callback) const {
    SearchNode(key, [&](const Node *node, bool is_last) -> bool {
      return callback(std::any_cast<std::shared_ptr<T>>(node->data_), is_last);
    });
  }

  /**
   * @brief: Check key
   * @param: key: key
   * @return: true if key exists
   */
  bool Check(const std::string &key) const;

  /**
   * @brief: Clean all data,Reset trie
   */
  void Reset();

public:
  struct Node {
    std::string key_;
    std::vector<Node *> next_{128, nullptr};
    bool stop_ = false;
    std::any data_ = nullptr;
  };

  /**
   * @brief: same to Set, Get, Search,but operate on node instead of data, not
   * recommend use it directly
   */
  void SetNode(const std::string &key, std::any data);
  const Node *GetNode(const std::string &key) const;
  void SearchNode(const std::string &key,
                  std::function<bool(const Node *, bool)> callback) const;

private:
  void CleanMemory(Node *root);
  void ForkNode(Node *node, int key_pos);

private:
  Node *root_;
};

} // namespace cppnet
