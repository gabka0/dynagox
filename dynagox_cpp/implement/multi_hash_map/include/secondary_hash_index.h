#ifndef DYNAGOX_SECONDARY_HASH_INDEX_H
#define DYNAGOX_SECONDARY_HASH_INDEX_H

#include "memory_pool.h"
#include "singleton.h"
#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#ifdef _WIN32
#include <malloc.h>
#endif

#define SECONDARY_HASH_INDEX_DEFAULT_CAPACITY 16384
#define SECONDARY_HASH_INDEX_DEFAULT_LOAD_FACTOR 0.7

template <typename T> struct LinkedListNode {
  T *object;
  LinkedListNode *next;
};

template <typename T> struct SecondaryHashIndexNode {
  LinkedListNode<T> linked_list_node;
  size_t hash_value;
  SecondaryHashIndexNode *next;
};

template <typename T> class AbstractSecondaryHashIndex {
public:
  virtual ~AbstractSecondaryHashIndex() = default;
  virtual void put(T *object) = 0;
  virtual LinkedListNode<T> *find(const T &key) = 0;
  virtual void remove(T *object) = 0;
  virtual void clear() = 0;
};

template <typename T, typename Hash, typename Equal>
class SecondaryHashIndex : public AbstractSecondaryHashIndex<T> {
private:
  typedef Hash key_hash;
  typedef Equal key_equal;
  typedef SecondaryHashIndexNode<T> Node;
  typedef LinkedListNode<T> LinkedNode;

  MemoryPool<Node> *node_pool;
  MemoryPool<LinkedNode> *linked_node_pool;

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

#ifdef _WIN32
    buckets = (Node *)_aligned_malloc(capacity * sizeof(Node), 64);
    assert(buckets != nullptr);
#else
    int r = posix_memalign((void **)&buckets, 64, capacity * sizeof(Node));
    assert(r == 0);
#endif
    memset(buckets, 0, capacity * sizeof(Node));

    for (size_t i = 0; i < c; ++i) {
      Node *node_ptr1 = b + i;
      if (node_ptr1->linked_list_node.object != nullptr) {
        Node *node_ptr2 = buckets + (node_ptr1->hash_value & mask);

        if (node_ptr2->linked_list_node.object != nullptr) {
          Node *new_node = node_pool->allocate();
          new_node->linked_list_node.object =
              node_ptr1->linked_list_node.object;
          new_node->linked_list_node.next = node_ptr1->linked_list_node.next;
          new_node->hash_value = node_ptr1->hash_value;
          new_node->next = node_ptr2->next;
          node_ptr2->next = new_node;
        } else {
          node_ptr2->linked_list_node = node_ptr1->linked_list_node;
          node_ptr2->hash_value = node_ptr1->hash_value;
          assert(node_ptr2->next == nullptr);
        }

        node_ptr1 = node_ptr1->next;
        while (node_ptr1 != nullptr) {
          Node *node_ptr3 = buckets + (node_ptr1->hash_value & mask);
          auto p = node_ptr1;
          node_ptr1 = node_ptr1->next;

          if (node_ptr3->linked_list_node.object != nullptr) {
            p->next = node_ptr3->next;
            node_ptr3->next = p;
          } else {
            node_ptr3->linked_list_node.object = p->linked_list_node.object;
            node_ptr3->linked_list_node.next = p->linked_list_node.next;
            node_ptr3->hash_value = p->hash_value;
            assert(node_ptr3->next == nullptr);
            node_pool->deallocate(p);
          }
        }
      }
    }

    if (b != nullptr) {
#ifdef _WIN32
      _aligned_free(b);
#else
      free(b);
#endif
    }
  }

  inline void put(T *object, size_t hash_value) {
    Node *node_ptr = buckets + (hash_value & mask);
    if (node_ptr->linked_list_node.object != nullptr) {
      auto p = node_ptr;
      do {
        if (p->linked_list_node.object != nullptr &&
            p->hash_value == hash_value &&
            key_equal::equal(*object, *(p->linked_list_node.object)))
          break;
        p = p->next;
      } while (p != nullptr);

      if (p != nullptr) {
        auto *new_node = linked_node_pool->allocate();
        new_node->object = object;
        new_node->next = p->linked_list_node.next;
        p->linked_list_node.next = new_node;
      } else {
        Node *new_node = node_pool->allocate();
        new_node->linked_list_node.object = object;
        new_node->linked_list_node.next = nullptr;
        new_node->hash_value = hash_value;
        new_node->next = node_ptr->next;
        node_ptr->next = new_node;
        size += 1;
        if (size > threshold) {
          resize(capacity * 2);
        }
      }
    } else {
      node_ptr->linked_list_node.object = object;
      assert(node_ptr->linked_list_node.next == nullptr);
      node_ptr->hash_value = hash_value;
      assert(node_ptr->next == nullptr);
      size += 1;
      if (size > threshold) {
        resize(capacity * 2);
      }
    }
  }

  inline LinkedNode *find(const T &key, size_t hash_value) {
    Node *node_ptr = buckets + (hash_value & mask);
    do {
      if (node_ptr->linked_list_node.object != nullptr &&
          node_ptr->hash_value == hash_value &&
          key_equal::equal(key, *(node_ptr->linked_list_node.object))) {
        return &(node_ptr->linked_list_node);
      }
      node_ptr = node_ptr->next;
    } while (node_ptr != nullptr);

    return nullptr;
  }

  void remove(T *object, size_t hash_value) {
    Node *current_node = buckets + (hash_value & mask);
    Node *previous_node = nullptr;

    do {
      if (current_node->linked_list_node.object != nullptr &&
          current_node->hash_value == hash_value &&
          key_equal::equal(*object, *(current_node->linked_list_node.object))) {
        if (current_node->linked_list_node.object == object) {
          if (current_node->linked_list_node.next != nullptr) {
            auto p = current_node->linked_list_node.next;
            current_node->linked_list_node.object = p->object;
            current_node->linked_list_node.next = p->next;
            linked_node_pool->deallocate(p);
          } else {
            if (previous_node == nullptr) {
              if (current_node->next != nullptr) {
                auto p = current_node->next;
                current_node->linked_list_node.object =
                    p->linked_list_node.object;
                current_node->linked_list_node.next = p->linked_list_node.next;
                current_node->hash_value = p->hash_value;
                current_node->next = p->next;
                node_pool->deallocate(p);
              } else {
                current_node->linked_list_node.object = nullptr;
                current_node->linked_list_node.next = nullptr;
                current_node->hash_value = 0;
                current_node->next = nullptr;
              }
            } else {
              auto p = current_node;
              previous_node->next = p->next;
              node_pool->deallocate(p);
            }

            size -= 1;
          }
        } else {
          LinkedNode *previous_linked_node = &(current_node->linked_list_node);
          LinkedNode *current_linked_node = previous_linked_node->next;
          while (current_linked_node != nullptr) {
            if (current_linked_node->object == object) {
              auto p = current_linked_node;
              current_linked_node = current_linked_node->next;
              previous_linked_node->next = current_linked_node;
              linked_node_pool->deallocate(p);
              return;
            }

            previous_linked_node = current_linked_node;
            current_linked_node = current_linked_node->next;
          }
        }
        return;
      } else {
        previous_node = current_node;
        current_node = current_node->next;
      }
    } while (current_node != nullptr);
  }

public:
  explicit SecondaryHashIndex(
      size_t capacity = SECONDARY_HASH_INDEX_DEFAULT_CAPACITY,
      double load_factor = SECONDARY_HASH_INDEX_DEFAULT_LOAD_FACTOR)
      : node_pool(Singleton<MemoryPool<Node>>().acquire()),
        linked_node_pool(Singleton<MemoryPool<LinkedNode>>().acquire()),
        buckets(nullptr), size(0), capacity(0), mask(0), threshold(0),
        load_factor(load_factor) {
    resize(capacity);
  }

  ~SecondaryHashIndex() {
    clear();
    if (buckets != nullptr) {
#ifdef _WIN32
      _aligned_free(buckets);
#else
      free(buckets);
#endif
      buckets = nullptr;
    }
    Singleton<MemoryPool<Node>>().release(node_pool);
    node_pool = nullptr;
    Singleton<MemoryPool<LinkedNode>>().release(linked_node_pool);
    linked_node_pool = nullptr;
  }

  inline void put(T *object) {
    size_t hash_value = key_hash::hash(*object);
    put(object, hash_value);
  }

  inline LinkedNode *find(const T &key) {
    size_t hash_value = key_hash::hash(key);
    return find(key, hash_value);
  }

  inline void remove(T *object) {
    size_t hash_value = key_hash::hash(*object);
    remove(object, hash_value);
  }

  void clear() {
    if (size > 0) {
      for (size_t i = 0; i < capacity; ++i) {
        Node *node_ptr = buckets[i].next;
        LinkedNode *linked_node_ptr = buckets[i].linked_list_node.next;
        buckets[i].linked_list_node.object = nullptr;
        buckets[i].linked_list_node.next = nullptr;
        buckets[i].hash_value = 0;
        buckets[i].next = nullptr;

        while (linked_node_ptr != nullptr) {
          auto p = linked_node_ptr;
          linked_node_ptr = linked_node_ptr->next;
          linked_node_pool->deallocate(p);
        }

        while (node_ptr != nullptr) {
          auto p = node_ptr;
          linked_node_ptr = node_ptr->linked_list_node.next;
          node_ptr = node_ptr->next;
          node_pool->deallocate(p);

          while (linked_node_ptr != nullptr) {
            auto p2 = linked_node_ptr;
            linked_node_ptr = linked_node_ptr->next;
            linked_node_pool->deallocate(p2);
          }
        }
      }
      size = 0;
    }
  }

  inline size_t get_size() { return size; }

  inline size_t get_capacity() { return capacity; }
};

#endif
