#ifndef DYNAGOX_PRIMARY_HASH_INDEX_H
#define DYNAGOX_PRIMARY_HASH_INDEX_H

#include "memory_pool.h"
#include "singleton.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>

#define PRIMARY_HASH_INDEX_DEFAULT_CAPACITY 16384
#define PRIMARY_HASH_INDEX_DEFAULT_LOAD_FACTOR 0.7

template <typename T> struct PrimaryHashIndexNode {
  T *object;
  size_t hash_value;
  PrimaryHashIndexNode *next;
};

template <typename T, typename Hash, typename Equal> class PrimaryHashIndex {
private:
  typedef Hash key_hash;
  typedef Equal key_equal;
  typedef PrimaryHashIndexNode<T> Node;

  MemoryPool<Node> *pool;

  Node *buckets;
  size_t size;
  size_t capacity;
  size_t mask;
  size_t threshold;
  double load_factor;

  void resize(size_t expected_capacity) {
    auto b = buckets;
    auto c = capacity;
    capacity = expected_capacity;
    mask = capacity - 1;
    threshold = load_factor * capacity;

    int r = posix_memalign((void **)&buckets, 64, capacity * sizeof(Node));
    assert(r == 0);
    memset(buckets, 0, capacity * sizeof(Node));

    for (size_t i = 0; i < c; ++i) {
      Node *node_ptr1 = b + i;
      if (node_ptr1->object != nullptr) {
        Node *node_ptr2 = buckets + (node_ptr1->hash_value & mask);

        if (node_ptr2->object != nullptr) {
          Node *new_node = pool->allocate();
          new_node->object = node_ptr1->object;
          new_node->hash_value = node_ptr1->hash_value;
          new_node->next = node_ptr2->next;
          node_ptr2->next = new_node;
        } else {
          node_ptr2->object = node_ptr1->object;
          node_ptr2->hash_value = node_ptr1->hash_value;
        }

        node_ptr1 = node_ptr1->next;
        while (node_ptr1 != nullptr) {
          Node *node_ptr3 = buckets + (node_ptr1->hash_value & mask);
          auto p = node_ptr1;
          node_ptr1 = node_ptr1->next;

          if (node_ptr3->object != nullptr) {
            p->next = node_ptr3->next;
            node_ptr3->next = p;
          } else {
            node_ptr3->object = p->object;
            node_ptr3->hash_value = p->hash_value;
            pool->deallocate(p);
          }
        }
      }
    }

    delete[] b;
  }

public:
  explicit PrimaryHashIndex(
      size_t capacity = PRIMARY_HASH_INDEX_DEFAULT_CAPACITY,
      double load_factor = PRIMARY_HASH_INDEX_DEFAULT_LOAD_FACTOR)
      : pool(Singleton<MemoryPool<Node>>().acquire()), buckets(nullptr),
        size(0), capacity(0), mask(0), threshold(0), load_factor(load_factor) {
    resize(capacity);
  }

  ~PrimaryHashIndex() {
    clear();
    if (buckets != nullptr) {
      delete[] buckets;
      buckets = nullptr;
    }
    Singleton<MemoryPool<Node>>().release(pool);
    pool = nullptr;
  }

  inline T *get(const T &key, size_t hash_value) {
    Node *node_ptr = buckets + (hash_value & mask);
    while (node_ptr != nullptr) {
      auto p = node_ptr;
      if (p->object != nullptr && p->hash_value == hash_value &&
          key_equal::equal(key, *(p->object))) {
        return p->object;
      }
      node_ptr = p->next;
    }
    return nullptr;
  }

  inline void put(T *object, size_t hash_value) {
    if (size >= threshold) {
      resize(capacity * 2);
    }

    Node *node_ptr = buckets + (hash_value & mask);
    if (node_ptr->object != nullptr) {
      Node *new_node = pool->allocate();
      new_node->object = object;
      new_node->hash_value = hash_value;
      new_node->next = node_ptr->next;
      node_ptr->next = new_node;
    } else {
      node_ptr->object = object;
      node_ptr->hash_value = hash_value;
    }

    size += 1;
  }

  void remove(T *object, size_t hash_value) {
    Node *current = buckets + (hash_value & mask);
    Node *previous = nullptr;

    do {
      if (object == current->object) {
        if (previous == nullptr) {
          if (current->next != nullptr) {
            auto p = current->next;
            current->object = p->object;
            current->hash_value = p->hash_value;
            current->next = p->next;
            pool->deallocate(p);
          } else {
            current->object = nullptr;
            current->hash_value = 0;
            current->next = nullptr;
          }
        } else {
          auto p = current;
          previous->next = p->next;
          pool->deallocate(p);
        }

        size -= 1;
        return;
      } else {
        previous = current;
        current = current->next;
      }
    } while (current != nullptr);
  }

  void clear() {
    if (size > 0) {
      for (size_t i = 0; i < capacity; ++i) {
        Node *node_ptr = buckets[i].next;
        buckets[i].object = nullptr;
        buckets[i].hash_value = 0;
        buckets[i].next = nullptr;

        while (node_ptr != nullptr) {
          auto p = node_ptr;
          node_ptr = node_ptr->next;
          pool->deallocate(p);
        }
      }
      size = 0;
    }
  }

  inline size_t get_size() { return size; }

  inline size_t get_capacity() { return capacity; }

  inline size_t hash(const T &key) { return key_hash::hash(key); }
};

#endif
