#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace cppnet {

// Because of template,so cannot use cpp file
template <class T> class Trie {
public:
  using SearchCallback = std::function<bool(std::shared_ptr<T>, bool)>;

  Trie() { root_ = new Node; }

  ~Trie() {
    CleanMemory(root_);
    root_ = nullptr;
  }

  /**
   * @brief: Set key and value
   * @param: key: key
   * @param: data: data
   * @return: true if success
   */
  void Set(const std::string &key, std::shared_ptr<T> data) {
    Node *temp = root_;
    for (unsigned i = 0; i < key.size(); i++) {
      if (temp->next_[key[i]] != nullptr) {
        temp = temp->next_[key[i]];
      } else {
        temp->next_[key[i]] = new Node;
        temp = temp->next_[key[i]];
      }
    }
    temp->stop_ = true;
    temp->data_ = data;
  }
  /**
   * @brief: Get value by key
   * @param: key: key
   * @return: value
   */
  std::shared_ptr<T> Get(const std::string &key) const {
    Node *temp = root_;
    for (unsigned i = 0; i < key.size(); i++) {
      if (temp->next_[key[i]] == nullptr) {
        return nullptr;
      } else {
        temp = temp->next_[key[i]];
      }
    }
    if (temp->stop_ == false) {
      return nullptr;
    }
    return temp->data_;
  }
  /**
   * @brief: Search value by key
   * @param: key: key
   * @param: func: function callback
   * @return: value
   */
  void Search(const std::string &key, SearchCallback callback) const {
    Node *temp = root_;

    if (temp->stop_) {
      auto is_continue = callback(temp->data_, key.empty());
      if (!is_continue) {
        return;
      }
    }

    for (unsigned i = 0; i < key.size(); i++) {
      if (temp->next_[key[i]] == nullptr) {
        return;
      } else {
        temp = temp->next_[key[i]];
        if (temp->stop_) {
          auto is_continue = callback(temp->data_, i == key.size() - 1);
          if (!is_continue) {
            break;
          }
        }
      }
    }
    return;
  }

  /**
   * @brief: Check key
   * @param: key: key
   * @return: true if key exists
   */
  bool Check(const std::string &key) const {
    Node *temp = root_;
    for (unsigned i = 0; i < key.size(); i++) {
      if (temp->next_[key[i]] == nullptr) {
        return false;
      } else {
        temp = temp->next;
      }
    }
    if (temp->stop_ == false) {
      return false;
    }
    return true;
  }

  /**
   * @brief: Clean all data,Reset trie
   */
  void Reset() {
    CleanMemory(root_);
    root_ = new Node;
  }

private:
  struct Node {
    std::vector<Node *> next_{128, nullptr};
    bool stop_ = false;
    std::shared_ptr<T> data_ = nullptr;
  };

private:
  void CleanMemory(Node *root) {
    if (root == nullptr) {
      return;
    }

    for (unsigned i = 0; i < root->next_.size(); i++) {
      if (root->next_[i] != nullptr) {
        CleanMemory(root->next_[i]);
      }
    }
    delete root;
    root = nullptr;
  }

private:
  Node *root_;
};

} // namespace cppnet
