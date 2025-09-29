#include "secondary_hash_index.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_vector.hpp>
#include <memory>
#include <vector>

struct SecondaryHashIndexTestTriple {
  int f0;
  int f1;
  int f2;

  bool operator==(const SecondaryHashIndexTestTriple &other) const {
    return f0 == other.f0 && f1 == other.f1 && f2 == other.f2;
  }
};

struct SecondaryHashIndexTestTripleHash {
  static size_t hash(const SecondaryHashIndexTestTriple &triple) {
    return triple.f0 & triple.f1;
  }
};

struct SecondaryHashIndexTestTripleEqual {
  static bool equal(const SecondaryHashIndexTestTriple &lhs,
                    const SecondaryHashIndexTestTriple &rhs) {
    return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1;
  }
};

std::vector<SecondaryHashIndexTestTriple>
collect(LinkedListNode<SecondaryHashIndexTestTriple> *node) {
  std::vector<SecondaryHashIndexTestTriple> result;
  while (node != nullptr) {
    result.emplace_back(*node->object);
    node = node->next;
  }
  return result;
}

TEST_CASE(
    "SecondaryHashIndex: should have default capacity after initialization") {
  auto index = SecondaryHashIndex<SecondaryHashIndexTestTriple,
                                  SecondaryHashIndexTestTripleHash,
                                  SecondaryHashIndexTestTripleEqual>();
  REQUIRE(index.get_capacity() == SECONDARY_HASH_INDEX_DEFAULT_CAPACITY);
}

TEST_CASE("SecondaryHashIndex: should insert elements on put") {
  auto index = SecondaryHashIndex<SecondaryHashIndexTestTriple,
                                  SecondaryHashIndexTestTripleHash,
                                  SecondaryHashIndexTestTripleEqual>();
  std::vector<SecondaryHashIndexTestTriple> expected;
  auto t1 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 1, 1});
  index.put(t1.get());
  auto t2 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 2});
  index.put(t1.get());
  auto t3 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 3, 3});
  index.put(t1.get());
  auto t4 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 4});
  index.put(t1.get());
  auto t5 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 5});
  index.put(t1.get());
}

TEST_CASE("SecondaryHashIndex: should retrieve matching elements on find") {
  auto index = SecondaryHashIndex<SecondaryHashIndexTestTriple,
                                  SecondaryHashIndexTestTripleHash,
                                  SecondaryHashIndexTestTripleEqual>();
  std::vector<SecondaryHashIndexTestTriple> expected;
  auto t1 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 1, 1});
  index.put(t1.get());
  auto t2 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 2});
  index.put(t2.get());
  auto t3 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 3, 3});
  index.put(t3.get());
  auto t4 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 4});
  index.put(t4.get());
  auto t5 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 5});
  index.put(t5.get());

  expected.emplace_back(*t2);
  expected.emplace_back(*t4);
  expected.emplace_back(*t5);

  std::vector<SecondaryHashIndexTestTriple> actual =
      collect(index.find(SecondaryHashIndexTestTriple{0, 2, 0}));
  REQUIRE_THAT(actual, Catch::Matchers::UnorderedEquals(expected));
}

TEST_CASE("SecondaryHashIndex: should remove element on remove") {
  auto index = SecondaryHashIndex<SecondaryHashIndexTestTriple,
                                  SecondaryHashIndexTestTripleHash,
                                  SecondaryHashIndexTestTripleEqual>();
  std::vector<SecondaryHashIndexTestTriple> expected;
  auto t1 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 1, 1});
  index.put(t1.get());
  auto t2 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 2});
  index.put(t2.get());
  auto t3 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 3, 3});
  index.put(t3.get());
  auto t4 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 4});
  index.put(t4.get());
  auto t5 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{0, 2, 5});
  index.put(t5.get());

  index.remove(t2.get());

  expected.emplace_back(*t4);
  expected.emplace_back(*t5);

  std::vector<SecondaryHashIndexTestTriple> actual =
      collect(index.find(SecondaryHashIndexTestTriple{0, 2, 0}));
  REQUIRE_THAT(actual, Catch::Matchers::UnorderedEquals(expected));
}

TEST_CASE("SecondaryHashIndex: should resize when threshold is exceeded") {
  constexpr size_t capacity = 32;
  constexpr double load_factor = 0.75;
  const auto threshold = static_cast<size_t>(capacity * load_factor);

  auto index = SecondaryHashIndex<SecondaryHashIndexTestTriple,
                                  SecondaryHashIndexTestTripleHash,
                                  SecondaryHashIndexTestTripleEqual>(
      capacity, load_factor);
  std::vector<std::unique_ptr<SecondaryHashIndexTestTriple>> hold;

  for (int i = 0; i < threshold; ++i) {
    auto t = std::make_unique<SecondaryHashIndexTestTriple>(
        SecondaryHashIndexTestTriple{1, i, i + 2});
    index.put(t.get());
    REQUIRE(index.get_capacity() == capacity);
    hold.emplace_back(std::move(t));
  }

  auto t1 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{1, 1, 2});
  index.put(t1.get());
  REQUIRE(index.get_capacity() == capacity);

  auto t2 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{1, threshold, threshold + 2});
  index.put(t2.get());
  REQUIRE(index.get_capacity() == capacity * 2);
}

TEST_CASE(
    "SecondaryHashIndex: should track the number of distinct keys correctly") {
  auto index = SecondaryHashIndex<SecondaryHashIndexTestTriple,
                                  SecondaryHashIndexTestTripleHash,
                                  SecondaryHashIndexTestTripleEqual>();
  auto t1 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{3, 2, 7});
  index.put(t1.get());
  REQUIRE(index.get_size() == 1);

  auto t2 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{1, 7, 8});
  index.put(t2.get());
  REQUIRE(index.get_size() == 2);

  auto t3 = std::make_unique<SecondaryHashIndexTestTriple>(
      SecondaryHashIndexTestTriple{3, 2, 3});
  index.put(t3.get());
  REQUIRE(index.get_size() == 2);

  index.remove(t2.get());
  REQUIRE(index.get_size() == 1);
}