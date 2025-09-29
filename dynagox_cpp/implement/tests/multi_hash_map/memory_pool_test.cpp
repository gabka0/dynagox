#include "memory_pool.h"
#include <catch2/catch_test_macros.hpp>

class MemoryPoolTestClass {
public:
  static inline bool constructed = false;
  static inline bool destructed = false;

  unsigned long value;

  MemoryPoolTestClass() { constructed = true; }
  ~MemoryPoolTestClass() { destructed = true; }
};

TEST_CASE("MemoryPool: should return non-null pointer on allocate") {
  MemoryPool<MemoryPoolTestClass> pool;
  auto p = pool.allocate();

  REQUIRE(p != nullptr);
}

TEST_CASE("MemoryPool: should call constructor on new_element") {
  MemoryPool<MemoryPoolTestClass> pool;
  MemoryPoolTestClass::constructed = false;

  MemoryPoolTestClass *object = pool.new_element();

  REQUIRE(object != nullptr);
  REQUIRE(MemoryPoolTestClass::constructed == true);
}

TEST_CASE("MemoryPool: should call destructor on delete_element") {
  MemoryPool<MemoryPoolTestClass> pool;
  MemoryPoolTestClass::destructed = false;

  MemoryPoolTestClass *object = pool.new_element();
  pool.delete_element(object);

  REQUIRE(MemoryPoolTestClass::destructed == true);
}

TEST_CASE("MemoryPool: should reuse memory after delete_element") {
  MemoryPool<MemoryPoolTestClass> pool;
  std::vector<MemoryPoolTestClass *> allocated;

  auto object1 = pool.new_element();
  pool.delete_element(object1);
  auto object2 = pool.new_element();

  REQUIRE(object1 == object2);
}