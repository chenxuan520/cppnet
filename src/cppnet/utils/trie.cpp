#include "trie.hpp"

namespace cppnet {

Trie::Trie() { root_ = new Node; }

Trie::~Trie() {
  CleanMemory(root_);
  root_ = nullptr;
}

void Trie::CleanMemory(Node *root) {
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

void Trie::Reset() {
  CleanMemory(root_);
  root_ = new Node;
}

bool Trie::Check(const std::string &key) const {
  auto node = GetNode(key);
  if (node == nullptr) {
    return false;
  }
  return true;
}

void Trie::ForkNode(Node *node, int key_pos) {
  if (node == nullptr) {
    return;
  }
  if (key_pos <= 0 || key_pos >= node->key_.size()) {
    return;
  }

  auto new_node = new Node(*node);
  new_node->key_ = node->key_.substr(key_pos);

  // clean node
  node->data_ = nullptr;
  node->stop_ = false;
  for (unsigned i = 0; i < node->next_.size(); i++) {
    node->next_[i] = nullptr;
  }
  node->next_[node->key_[key_pos]] = new_node;
  node->key_ = node->key_.substr(0, key_pos);
}

void Trie::SetNode(const std::string &key, std::any data) {
  auto now = root_;
  auto pos = 0;

  if (key.empty()) {
    now->data_ = data;
    now->stop_ = true;
    return;
  }

  if (now->next_[key[0]] == nullptr) {
    now->next_[key[0]] = new Node();
  }
  now = now->next_[key[0]];

  if (now->key_.empty()) {
    now->key_ = key;
    now->data_ = data;
    now->stop_ = true;
    return;
  }

  for (unsigned i = 0; i < key.size(); i++) {
    if (pos < now->key_.size()) {
      if (now->key_[pos] == key[i]) {
        pos++;
      } else {
        ForkNode(now, pos);
        // for avoid i++
        i--;
      }
    } else {
      if (now->next_[key[i]] == nullptr) {
        auto tmp = new Node();
        now->next_[key[i]] = tmp;
        tmp->key_ = key.substr(i);
        now = tmp;
        pos = now->key_.size();
        break;
      } else {
        now = now->next_[key[i]];
        // first is same to key[i]
        pos = 1;
      }
    }
  }
  if (pos != now->key_.size()) {
    ForkNode(now, pos);
  }
  now->data_ = data;
  now->stop_ = true;
}

const Trie::Node *Trie::GetNode(const std::string &key) const {
  auto now = root_;
  auto pos = 0;

  if (key.empty()) {
    if (now->stop_) {
      return now;
    } else {
      return nullptr;
    }
  }

  if (now->next_[key[0]] == nullptr) {
    return nullptr;
  }
  now = now->next_[key[0]];

  for (unsigned i = 0; i < key.size(); i++) {
    if (pos < now->key_.size()) {
      if (now->key_[pos] == key[i]) {
        pos++;
      } else {
        return nullptr;
      }
    } else {
      if (now->next_[key[i]] == nullptr) {
        return nullptr;
      }
      now = now->next_[key[i]];
      // first is same to key[i]
      pos = 1;
    }
  }

  if (pos != now->key_.size()) {
    return nullptr;
  }

  if (now->stop_ == false) {
    return nullptr;
  }
  return now;
}

void Trie::SearchNode(const std::string &key,
                      std::function<bool(const Node *, bool)> callback) const {
  auto now = root_;
  auto pos = 0;
  if (now->stop_) {
    auto is_continue = callback(now, key.empty());
    if (!is_continue) {
      return;
    }
  }
  if (key.empty()) {
    return;
  }

  if (now->next_[key[0]] == nullptr) {
    return;
  }
  now = now->next_[key[0]];

  for (unsigned i = 0; i < key.size(); i++) {
    if (pos < now->key_.size()) {
      if (now->key_[pos] == key[i]) {
        pos++;
      } else {
        return;
      }
    } else {
      if (now->stop_) {
        auto is_continue = callback(now, false);
        if (!is_continue) {
          return;
        }
      }

      if (now->next_[key[i]] == nullptr) {
        return;
      }
      now = now->next_[key[i]];
      // first is same to key[i]
      pos = 1;
    }
  }

  if (now->stop_ && pos == now->key_.size()) {
    callback(now, true);
  }
}

} // namespace cppnet
