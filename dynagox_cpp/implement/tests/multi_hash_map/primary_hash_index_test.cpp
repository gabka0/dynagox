#include "primary_hash_index.h"
#include <catch2/catch_test_macros.hpp>
#include <memory>
#include <vector>

struct PrimaryHashIndexTestTriple {
  int f0;
  int f1;
  int f2;

  bool operator==(const PrimaryHashIndexTestTriple &other) const {
    return f0 == other.f0 && f1 == other.f1 && f2 == other.f2;
  }
};

struct PrimaryHashIndexTestTripleHash {
  static size_t hash(const PrimaryHashIndexTestTriple &triple) {
    return triple.f0 & triple.f1 & triple.f2;
  }
};

struct PrimaryHashIndexTestTripleEqual {
  static bool equal(const PrimaryHashIndexTestTriple &lhs,
                    const PrimaryHashIndexTestTriple &rhs) {
    return lhs == rhs;
  }
};

TEST_CASE(
    "PrimaryHashIndex: should have default capacity after initialization") {
  auto index = PrimaryHashIndex<PrimaryHashIndexTestTriple,
                                PrimaryHashIndexTestTripleHash,
                                PrimaryHashIndexTestTripleEqual>();
  REQUIRE(index.get_capacity() == PRIMARY_HASH_INDEX_DEFAULT_CAPACITY);
}

TEST_CASE("PrimaryHashIndex: should insert elements on put") {
  auto index = PrimaryHashIndex<PrimaryHashIndexTestTriple,
                                PrimaryHashIndexTestTripleHash,
                                PrimaryHashIndexTestTripleEqual>();
  auto t1 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{3, 2, 7});
  auto h1 = index.hash(*t1);
  index.put(t1.get(), h1);

  auto t2 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{1, 7, 8});
  auto h2 = index.hash(*t2);
  index.put(t2.get(), h2);
}

TEST_CASE("PrimaryHashIndex: should retrieve matching element on get") {
  auto index = PrimaryHashIndex<PrimaryHashIndexTestTriple,
                                PrimaryHashIndexTestTripleHash,
                                PrimaryHashIndexTestTripleEqual>();
  auto t1 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{3, 2, 7});
  auto h1 = index.hash(*t1);
  index.put(t1.get(), h1);
  auto r1 = index.get(*t1, h1);
  REQUIRE(PrimaryHashIndexTestTripleEqual::equal(*t1, *r1) == true);

  auto t2 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{1, 7, 8});
  auto h2 = index.hash(*t2);
  index.put(t2.get(), h2);
  auto r2 = index.get(*t2, h2);
  REQUIRE(PrimaryHashIndexTestTripleEqual::equal(*t2, *r2) == true);
}

TEST_CASE("PrimaryHashIndex: should remove element on remove") {
  auto index = PrimaryHashIndex<PrimaryHashIndexTestTriple,
                                PrimaryHashIndexTestTripleHash,
                                PrimaryHashIndexTestTripleEqual>();
  auto t1 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{3, 2, 7});
  auto h1 = index.hash(*t1);
  index.put(t1.get(), h1);
  auto t2 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{1, 7, 8});
  auto h2 = index.hash(*t2);
  index.put(t2.get(), h2);

  index.remove(t1.get(), h1);
  auto r1 = index.get(*t1, h1);
  REQUIRE(r1 == nullptr);

  index.remove(t2.get(), h2);
  auto r2 = index.get(*t2, h2);
  REQUIRE(r2 == nullptr);
}

TEST_CASE("PrimaryHashIndex: should resize when threshold is exceeded") {
  constexpr size_t capacity = 32;
  constexpr double load_factor = 0.75;
  const auto threshold = static_cast<size_t>(capacity * load_factor);

  auto index =
      PrimaryHashIndex<PrimaryHashIndexTestTriple,
                       PrimaryHashIndexTestTripleHash,
                       PrimaryHashIndexTestTripleEqual>(capacity, load_factor);
  std::vector<std::unique_ptr<PrimaryHashIndexTestTriple>> hold;

  for (int i = 0; i < threshold; ++i) {
    auto t = std::make_unique<PrimaryHashIndexTestTriple>(
        PrimaryHashIndexTestTriple{i, i + 1, i + 2});
    auto h = index.hash(*t);
    index.put(t.get(), h);
    REQUIRE(index.get_capacity() == capacity);
    hold.emplace_back(std::move(t));
  }

  auto t = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{threshold, threshold + 1, threshold + 2});
  auto h = index.hash(*t);
  index.put(t.get(), h);
  REQUIRE(index.get_capacity() == capacity * 2);
}

TEST_CASE("PrimaryHashIndex: should track the number of keys correctly") {
  auto index = PrimaryHashIndex<PrimaryHashIndexTestTriple,
                                PrimaryHashIndexTestTripleHash,
                                PrimaryHashIndexTestTripleEqual>();
  auto t1 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{3, 2, 7});
  auto h1 = index.hash(*t1);
  index.put(t1.get(), h1);
  REQUIRE(index.get_size() == 1);

  auto t2 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{1, 7, 8});
  auto h2 = index.hash(*t2);
  index.put(t2.get(), h2);
  REQUIRE(index.get_size() == 2);

  auto t3 = std::make_unique<PrimaryHashIndexTestTriple>(
      PrimaryHashIndexTestTriple{3, 2, 3});
  auto h3 = index.hash(*t3);
  index.put(t3.get(), h3);
  REQUIRE(index.get_size() == 3);

  index.remove(t2.get(), h2);
  REQUIRE(index.get_size() == 2);
}