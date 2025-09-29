#ifndef DYNAGOX_MULTI_HASH_MAP_H
#define DYNAGOX_MULTI_HASH_MAP_H

#include "primary_hash_index.h"
#include "secondary_hash_index.h"
#include <algorithm>
#include <vector>

template <typename T, typename P, typename... S> class MultiHashMap {
private:
  MemoryPool<T> *pool;

  P *primary_hash_index;
  AbstractSecondaryHashIndex<T> **secondary_hash_indexes;
  size_t secondary_hash_indexes_count;

  T *keys;

  inline bool is_zero(double annotation) {
    return (-0.000001 < annotation) && (annotation < 0.000001);
  }

  inline T *put(const T &key, size_t hash_value) {
    T *object = pool->new_element(key);
    primary_hash_index->put(object, hash_value);

    for (size_t i = 0; i < secondary_hash_indexes_count; ++i) {
      secondary_hash_indexes[i]->put(object);
    }

    if (keys != nullptr) {
      keys->previous = object;
    }
    object->previous = nullptr;
    object->next = keys;
    keys = object;

    return object;
  }

  inline T *get(const T &key, size_t hash_value) {
    return primary_hash_index->get(key, hash_value);
  }

  inline void remove(T *key, size_t hash_value) {
    if (key->previous != nullptr) {
      key->previous->next = key->next;
    }

    if (key->next != nullptr) {
      key->next->previous = key->previous;
    }

    if (keys == key) {
      keys = key->next;
    }
    key->previous = nullptr;
    key->next = nullptr;

    primary_hash_index->remove(key, hash_value);
    for (size_t i = 0; i < secondary_hash_indexes_count; ++i) {
      secondary_hash_indexes[i]->remove(key);
    }

    pool->delete_element(key);
  }

public:
  explicit MultiHashMap() {
    pool = Singleton<MemoryPool<T>>().acquire();
    primary_hash_index = new P();
    secondary_hash_indexes =
        new AbstractSecondaryHashIndex<T> *[sizeof...(S)] { new S()... };
    secondary_hash_indexes_count = sizeof...(S);

    keys = nullptr;
  }

  ~MultiHashMap() {
    if (primary_hash_index != nullptr) {
      delete primary_hash_index;
      primary_hash_index = nullptr;
    }

    for (size_t i = 0; i < secondary_hash_indexes_count; ++i) {
      if (secondary_hash_indexes[i] != nullptr) {
        delete secondary_hash_indexes[i];
      }
    }
    delete[] secondary_hash_indexes;
    secondary_hash_indexes = nullptr;

    Singleton<MemoryPool<T>>().release(pool);
    pool = nullptr;
  }

  inline T *get(const T &key) {
    size_t hash_value = primary_hash_index->hash(key);
    return primary_hash_index->get(key, hash_value);
  }

  inline bool contains(const T &key) { return get(key) != nullptr; }

  inline LinkedListNode<T> *find(size_t n, const T &key) {
    return secondary_hash_indexes[n]->find(key);
  }

  inline T *get_keys() { return keys; }

  inline void update_add(T &key, double annotation) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      p->annotation += annotation;
    } else {
      key.annotation = annotation;
      put(key, hash_value);
    }
  }

  inline double update_add_approx(T &key, double annotation, double b) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      p->annotation += annotation;
      if (p->annotation > p->approximate) {
        auto d = p->approximate;
        p->approximate = p->annotation * b;
        return p->approximate - d;
      } else {
        return 0;
      }
    } else {
      key.annotation = annotation;
      auto approx = annotation * b;
      key.approximate = approx;
      put(key, hash_value);
      return approx;
    }
  }

  inline bool update_activate(T &key) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      if (!p->active) {
        p->active = true;
        return true;
      } else {
        return false;
      }
    } else {
      auto object = put(key, hash_value);
      object->active = true;
      return true;
    }
  }

  inline bool update_add_and_activate(T &key, double annotation) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      p->annotation += annotation;
      if (!p->active) {
        p->active = true;
        return true;
      } else {
        return false;
      }
    } else {
      key.annotation = annotation;
      auto object = put(key, hash_value);
      object->active = true;
      return true;
    }
  }

  inline bool update_subtract(T &key, double annotation) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    assert(p != nullptr);
    p->annotation -= annotation;
    if (is_zero(p->annotation)) {
      remove(p, hash_value);
      return true;
    } else {
      return false;
    }
  }

  inline double update_subtract_approx(T &key, double annotation, double b) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    assert(p != nullptr);
    p->annotation -= annotation;
    if (p->annotation * b < p->approximate) {
      auto d = p->approximate;
      p->approximate = p->annotation * b;
      if (is_zero(p->approximate)) {
        remove(p, hash_value);
        return d;
      } else {
        auto r = d - p->approximate;
        return is_zero(r) ? 0 : r;
      }
    } else {
      return 0;
    }
  }

  inline bool update_deactivate(T &key) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    assert(p != nullptr);
    if (p->active) {
      p->active = false;
      return true;
    } else {
      return false;
    }
  }

  inline void update_remove_if_exists(T &key) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      remove(p, hash_value);
    }
  }

  inline bool update_max(T &key, double annotation) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      if (annotation > p->annotation) {
        p->annotation = annotation;
        return true;
      } else {
        return false;
      }
    } else {
      key.annotation = annotation;
      put(key, hash_value);
      return true;
    }
  }

  inline double update_max_approx(T &key, double annotation, double b) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      p->annotation = std::max(p->annotation, annotation);
      if (p->annotation > p->approximate) {
        p->approximate = p->annotation * b;
        return p->approximate;
      } else {
        return 0;
      }
    } else {
      key.annotation = annotation;
      auto approx = annotation * b;
      key.approximate = approx;
      put(key, hash_value);
      return approx;
    }
  }

  inline double update_max2_approx(T &key, double annotation, double e) {
    size_t hash_value = primary_hash_index->hash(key);
    auto p = get(key, hash_value);
    if (p != nullptr) {
      p->annotation = std::max(p->annotation, annotation);
      if (p->annotation > p->approximate) {
        p->approximate = p->annotation + e;
        return p->approximate;
      } else {
        return 0;
      }
    } else {
      key.annotation = annotation;
      auto approx = annotation + e;
      key.approximate = approx;
      put(key, hash_value);
      return approx;
    }
  }

  void clear() {
    if (primary_hash_index->get_size() > 0) {
      primary_hash_index->clear();
      for (size_t i = 0; i < secondary_hash_indexes_count; ++i) {
        secondary_hash_indexes[i]->clear();
      }

      while (keys != nullptr) {
        auto p = keys;
        keys = keys->next;
        pool->deallocate(p);
      }
      keys = nullptr;
    }
  }
};

#endif
