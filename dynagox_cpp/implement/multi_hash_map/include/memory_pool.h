#ifndef DYNAGOX_MEMORY_POOL_H
#define DYNAGOX_MEMORY_POOL_H

#include <cassert>
#include <cstddef>
#include <cstdlib>
#include <utility>

#define MEMORY_POOL_DEFAULT_CHUNK_SIZE 4096
#define MEMORY_POOL_MAX_CHUNK_SIZE 1048576

template <typename T> class MemoryPool {
private:
  union Slot {
    T object;
    Slot *next;
  };

  Slot *current_slot;
  Slot *last_slot;
  Slot *free_slots;
  Slot *chunks;
  size_t chunk_size;

  void allocate_new_chunk() {
    assert(free_slots == nullptr);

    Slot *new_chunk =
        reinterpret_cast<Slot *>(malloc(chunk_size * sizeof(Slot)));
    assert(new_chunk != nullptr);

    new_chunk->next = chunks;
    chunks = new_chunk;
    current_slot = new_chunk + 1;
    last_slot = new_chunk + (chunk_size - 1);

    auto next_chunk_size = (chunk_size * 2) < MEMORY_POOL_MAX_CHUNK_SIZE
                               ? (chunk_size * 2)
                               : MEMORY_POOL_MAX_CHUNK_SIZE;
    chunk_size = next_chunk_size;
  }

public:
  explicit MemoryPool()
      : current_slot(nullptr), last_slot(nullptr), free_slots(nullptr),
        chunks(nullptr), chunk_size(MEMORY_POOL_DEFAULT_CHUNK_SIZE) {
    allocate_new_chunk();
  }

  ~MemoryPool() {
    Slot *chunk = chunks;
    while (chunk != nullptr) {
      auto p = chunk;
      chunk = chunk->next;
      free(p);
    }
    current_slot = nullptr;
    last_slot = nullptr;
    free_slots = nullptr;
    chunks = nullptr;
  }

  inline T *allocate() {
    if (free_slots != nullptr) {
      T *object = reinterpret_cast<T *>(free_slots);
      free_slots = free_slots->next;
      return object;
    } else {
      if (current_slot > last_slot) {
        allocate_new_chunk();
      }
      return reinterpret_cast<T *>(current_slot++);
    }
  }

  inline void deallocate(T *object) {
    if (object != nullptr) {
      Slot *slot = reinterpret_cast<Slot *>(object);
      slot->next = free_slots;
      free_slots = slot;
    }
  }

  template <class... Args> inline T *new_element(Args &&...args) {
    T *object = allocate();
    new (object) T(std::forward<Args>(args)...);
    return object;
  }

  inline void delete_element(T *object) {
    if (object != nullptr) {
      object->~T();
      deallocate(object);
    }
  }
};

#endif
