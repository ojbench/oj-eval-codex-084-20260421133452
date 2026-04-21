// Implementation for Problem 084 - LinkedHashMap
// Follows the provided interface in /workspace/data/084/src.hpp

#ifndef FINAL_SRC_HPP
#define FINAL_SRC_HPP

#include <string>
#include <vector>

using std::string;
using std::vector;

struct LinkedHashMap;
struct Node {
  Node* next = nullptr;      // next in bucket singly linked list
  Node* timePre = nullptr;   // previous in global insertion-order doubly list
  Node* timeNext = nullptr;  // next in global insertion-order doubly list

  int key = 0;
  string value;
};

struct Data {
  int key;
  string value;
};

vector<Data> traverseByTime(const LinkedHashMap& obj);
vector<Data> traverseByIndex(const LinkedHashMap& obj);

struct LinkedHashMap {
  int len;
  Node** array;
  Node* beg;  // dummy head for time-order doubly linked list
  Node* cur;  // tail pointer for time-order doubly linked list

  // Function pointer to choose traversal strategy
  vector<Data> (*forEachMethod)(const LinkedHashMap& obj);

  void init(int len_, bool forEachByTime) {
    len = len_;
    beg = new Node();
    cur = beg;
    array = new Node*[len];
    for (int i = 0; i < len; i++) {
      array[i] = nullptr;
    }
    if (forEachByTime)
      forEachMethod = traverseByTime;
    else
      forEachMethod = traverseByIndex;
  }

  void clearMemory() {
    // Free all nodes by traversing the insertion-order list
    Node* p = beg->timeNext;
    while (p != nullptr) {
      Node* nxt = p->timeNext;
      delete p;
      p = nxt;
    }
    delete beg;
    // free the buckets array
    if (array != nullptr) {
      delete[] array;
    }
    // reset pointers (defensive; not strictly required)
    beg = nullptr;
    cur = nullptr;
    array = nullptr;
    len = 0;
  }

  void insert(int key, string value) {
    // create new node
    Node* node = new Node();
    node->key = key;
    node->value = value;

    // insert to head of bucket list
    node->next = array[key];
    array[key] = node;

    // append to tail of time-order doubly list
    node->timePre = cur;
    node->timeNext = nullptr;
    cur->timeNext = node;
    cur = node;
  }

  void remove(int key, string value) {
    Node* prev = nullptr;
    Node* p = array[key];
    while (p != nullptr) {
      if (p->value == value) {
        // unlink from bucket list
        Node* nxtInBucket = p->next;
        if (prev == nullptr)
          array[key] = nxtInBucket;
        else
          prev->next = nxtInBucket;

        // unlink from time-order list in O(1)
        Node* tp = p->timePre;
        Node* tn = p->timeNext;
        if (tp != nullptr) tp->timeNext = tn;
        if (tn != nullptr) tn->timePre = tp;
        else cur = tp;  // removed tail; update tail pointer

        delete p;
        p = nxtInBucket;
        // prev remains the same because current node removed
      } else {
        prev = p;
        p = p->next;
      }
    }
  }

  vector<string> ask(int key) const {
    vector<string> res;
    Node* p = array[key];
    while (p != nullptr) {
      res.push_back(p->value);
      p = p->next;
    }
    return res;
  }

  vector<Data> forEach() const { return forEachMethod(*this); }
};

vector<Data> traverseByTime(const LinkedHashMap& obj) {
    vector<Data> res;
    Node* p = obj.beg->timeNext;
    while (p != nullptr) {
      Data d;
      d.key = p->key;
      d.value = p->value;
      res.push_back(d);
      p = p->timeNext;
    }
    return res;
}

vector<Data> traverseByIndex(const LinkedHashMap& obj) {
    vector<Data> res;
    for (int i = 0; i < obj.len; i++) {
      Node* p = obj.array[i];
      while (p != nullptr) {
        Data d;
        d.key = p->key;
        d.value = p->value;
        res.push_back(d);
        p = p->next;
      }
    }
    return res;
}

#endif  // FINAL_SRC_HPP

