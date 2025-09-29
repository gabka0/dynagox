#include "multi_hash_map.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

struct MultiHashMapTestTriple {
  int f0;
  int f1;
  int f2;
  double annotation;
  double approximate;
  bool active;
  MultiHashMapTestTriple *previous;
  MultiHashMapTestTriple *next;

  MultiHashMapTestTriple()
      : f0(0), f1(0), f2(0), annotation(0), approximate(0), active(false),
        previous(nullptr), next(nullptr) {}

  MultiHashMapTestTriple(const MultiHashMapTestTriple &other)
      : f0(other.f0), f1(other.f1), f2(other.f2), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct MultiHashMapTestTripleHash {
  static size_t hash(const MultiHashMapTestTriple &triple) {
    return triple.f0 & triple.f1 & triple.f2;
  }
};

struct MultiHashMapTestTripleEqual {
  static bool equal(const MultiHashMapTestTriple &lhs,
                    const MultiHashMapTestTriple &rhs) {
    return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1 && lhs.f2 == rhs.f2;
  }
};

struct MultiHashMapTestTripleHash01 {
  static size_t hash(const MultiHashMapTestTriple &triple) {
    return triple.f0 & triple.f1;
  }
};

struct MultiHashMapTestTripleEqual01 {
  static bool equal(const MultiHashMapTestTriple &lhs,
                    const MultiHashMapTestTriple &rhs) {
    return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1;
  }
};

TEST_CASE("MultiHashMap: should return nullptr on get for non-existent key") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>>();
  auto t = MultiHashMapTestTriple();
  REQUIRE(map.get(t) == nullptr);
}

TEST_CASE("MultiHashMap: should add new key with annotation on update_add") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 3);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(3, 0.000001));
}

TEST_CASE("MultiHashMap: should increase annotation on repeated update_add") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 3);
  map.update_add(t, 1);
  map.update_add(t, 4);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(8, 0.000001));
}

TEST_CASE("MultiHashMap: should add new key with annotation and approximate on "
          "update_add_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_approx(t, 5, 1.1);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(5, 0.000001));
  REQUIRE_THAT(r->approximate, Catch::Matchers::WithinRel(5.5, 0.000001));
}

TEST_CASE("MultiHashMap: should increase annotation and maintain approximate "
          "on repeated update_add_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_approx(t, 5, 1.1);
  map.update_add_approx(t, 8, 1.1);
  auto r1 = map.get(t);
  REQUIRE(r1 != nullptr);
  REQUIRE_THAT(r1->annotation, Catch::Matchers::WithinRel(13, 0.000001));
  REQUIRE_THAT(r1->approximate, Catch::Matchers::WithinRel(14.3, 0.000001));
  map.update_add_approx(t, 4, 1.1);
  auto r2 = map.get(t);
  REQUIRE(r2 != nullptr);
  REQUIRE_THAT(r2->annotation, Catch::Matchers::WithinRel(17, 0.000001));
  REQUIRE_THAT(r2->approximate, Catch::Matchers::WithinRel(18.7, 0.000001));
}

TEST_CASE("MultiHashMap: should return delta in approximate value on "
          "update_add_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  auto r1 = map.update_add_approx(t, 17, 1.1);
  auto e1 = 18.7 - 0;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(e1, 0.000001));
  auto r2 = map.update_add_approx(t, 1, 1.1);
  auto e2 = 18.7 - 18.7;
  REQUIRE_THAT(r2, Catch::Matchers::WithinAbs(e2, 0.000001));
  auto r3 = map.update_add_approx(t, 1, 1.1);
  auto e3 = 20.9 - 18.7;
  REQUIRE_THAT(r3, Catch::Matchers::WithinRel(e3, 0.000001));
}

TEST_CASE("MultiHashMap: should activate an inactive key on "
          "update_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 1);
  auto r1 = map.get(t);
  REQUIRE(r1->active == false);
  map.update_activate(t);
  auto r2 = map.get(t);
  REQUIRE(r2->active == true);
}

TEST_CASE("MultiHashMap: should return true when activating "
          "inactive key on update_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 1);
  auto r = map.update_activate(t);
  REQUIRE(r == true);
}

TEST_CASE("MultiHashMap: should return false when key is "
          "already active on update_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 1);
  map.update_activate(t);
  auto r = map.update_activate(t);
  REQUIRE(r == false);
}

TEST_CASE("MultiHashMap: should add new key with annotation and set active on "
          "update_add_and_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_and_activate(t, 1);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(1, 0.000001));
  REQUIRE(r->active == true);
}

TEST_CASE("MultiHashMap: should increase annotation on repeated "
          "update_add_and_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_and_activate(t, 3);
  map.update_add_and_activate(t, 1);
  map.update_add_and_activate(t, 4);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(8, 0.000001));
  REQUIRE(r->active == true);
}

TEST_CASE("MultiHashMap: should return true when activating "
          "inactive key on update_add_and_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 1);
  auto r = map.update_add_and_activate(t, 3);
  REQUIRE(r == true);
}

TEST_CASE("MultiHashMap: should return false when key is "
          "already active on update_add_and_activate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 1);
  map.update_activate(t);
  auto r = map.update_add_and_activate(t, 3);
  REQUIRE(r == false);
}

TEST_CASE("MultiHashMap: should decrease annotation on update_subtract") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 100);
  map.update_subtract(t, 3);
  auto r1 = map.get(t);
  REQUIRE(r1 != nullptr);
  REQUIRE_THAT(r1->annotation, Catch::Matchers::WithinRel(97, 0.000001));
  map.update_subtract(t, 27);
  auto r2 = map.get(t);
  REQUIRE(r2 != nullptr);
  REQUIRE_THAT(r2->annotation, Catch::Matchers::WithinRel(70, 0.000001));
}

TEST_CASE("MultiHashMap: should remove key when annotation reaches zero on "
          "update_subtract") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 100);
  map.update_subtract(t, 3);
  map.update_subtract(t, 27);
  map.update_subtract(t, 70);
  auto r = map.get(t);
  REQUIRE(r == nullptr);
}

TEST_CASE("MultiHashMap: should return true only when annotation is reduced to "
          "zero on update_subtract") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 100);
  auto r1 = map.update_subtract(t, 3);
  REQUIRE(r1 == false);
  auto r2 = map.update_subtract(t, 27);
  REQUIRE(r2 == false);
  auto r3 = map.update_subtract(t, 70);
  REQUIRE(r3 == true);
}

TEST_CASE("MultiHashMap: should decrease annotation and maintain approximate "
          "on update_subtract_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_approx(t, 90, 1.1);
  map.update_add_approx(t, 7, 1.1);
  map.update_subtract_approx(t, 5, 1.1);
  auto r1 = map.get(t);
  REQUIRE(r1 != nullptr);
  REQUIRE_THAT(r1->annotation, Catch::Matchers::WithinRel(92, 0.000001));
  REQUIRE_THAT(r1->approximate, Catch::Matchers::WithinRel(99, 0.000001));
  map.update_subtract_approx(t, 22, 1.1);
  auto r2 = map.get(t);
  REQUIRE(r2 != nullptr);
  REQUIRE_THAT(r2->annotation, Catch::Matchers::WithinRel(70, 0.000001));
  REQUIRE_THAT(r2->approximate, Catch::Matchers::WithinRel(77, 0.000001));
  map.update_subtract_approx(t, 27, 1.1);
  auto r3 = map.get(t);
  REQUIRE(r3 != nullptr);
  REQUIRE_THAT(r3->annotation, Catch::Matchers::WithinRel(43, 0.000001));
  REQUIRE_THAT(r3->approximate, Catch::Matchers::WithinRel(47.3, 0.000001));
}

TEST_CASE("MultiHashMap: should remove key when annotation reaches zero on "
          "update_subtract_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_approx(t, 100, 1.1);
  map.update_subtract_approx(t, 3, 1.1);
  map.update_subtract_approx(t, 27, 1.1);
  map.update_subtract_approx(t, 70, 1.1);
  auto r = map.get(t);
  REQUIRE(r == nullptr);
}

TEST_CASE("MultiHashMap: should return delta in approximate value on "
          "update_subtract_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_approx(t, 90, 1.1);
  map.update_add_approx(t, 7, 1.1);
  auto r1 = map.update_subtract_approx(t, 5, 1.1);
  auto e1 = 99 - 99;
  REQUIRE_THAT(r1, Catch::Matchers::WithinAbs(e1, 0.000001));
  auto r2 = map.update_subtract_approx(t, 22, 1.1);
  auto e2 = 99 - 77;
  REQUIRE_THAT(r2, Catch::Matchers::WithinRel(e2, 0.000001));
  auto r3 = map.update_subtract_approx(t, 27, 1.1);
  auto e3 = 77 - 47.3;
  REQUIRE_THAT(r3, Catch::Matchers::WithinRel(e3, 0.000001));
}

TEST_CASE("MultiHashMap: should deactivate an active key on "
          "update_deactivate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_and_activate(t, 1);
  auto r1 = map.get(t);
  REQUIRE(r1->active == true);
  map.update_deactivate(t);
  auto r2 = map.get(t);
  REQUIRE(r2->active == false);
}

TEST_CASE("MultiHashMap: should return true when deactivating "
          "active key on update_deactivate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add_and_activate(t, 1);
  auto r = map.update_deactivate(t);
  REQUIRE(r == true);
}

TEST_CASE("MultiHashMap: should return false when key is "
          "already inactive on update_deactivate") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_add(t, 1);
  map.update_deactivate(t);
  auto r = map.update_deactivate(t);
  REQUIRE(r == false);
}

TEST_CASE("MultiHashMap: should add new key with annotation on update_max") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_max(t, 3);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(3, 0.000001));
}

TEST_CASE(
    "MultiHashMap: should retain maximum annotation on repeated update_max") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_max(t, 3);
  auto r1 = map.get(t);
  REQUIRE(r1 != nullptr);
  REQUIRE_THAT(r1->annotation, Catch::Matchers::WithinRel(3, 0.000001));
  map.update_max(t, 1);
  auto r2 = map.get(t);
  REQUIRE(r2 != nullptr);
  REQUIRE_THAT(r2->annotation, Catch::Matchers::WithinRel(3, 0.000001));
  map.update_max(t, 4);
  auto r3 = map.get(t);
  REQUIRE(r3 != nullptr);
  REQUIRE_THAT(r3->annotation, Catch::Matchers::WithinRel(4, 0.000001));
}

TEST_CASE("MultiHashMap: should return true on update_max only if annotation "
          "was updated") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  auto r1 = map.update_max(t, 3);
  REQUIRE(r1 == true);
  auto r2 = map.update_max(t, 1);
  REQUIRE(r2 == false);
  auto r3 = map.update_max(t, 4);
  REQUIRE(r3 == true);
}

TEST_CASE(
    "MultiHashMap: should add new key with annotation on update_max_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_max_approx(t, 3, 1.1);
  auto r = map.get(t);
  REQUIRE(r != nullptr);
  REQUIRE_THAT(r->annotation, Catch::Matchers::WithinRel(3, 0.000001));
  REQUIRE_THAT(r->approximate, Catch::Matchers::WithinRel(3.3, 0.000001));
}

TEST_CASE("MultiHashMap: should retain maximum annotation on repeated "
          "update_max_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  map.update_max_approx(t, 3, 1.1);
  auto r1 = map.get(t);
  REQUIRE(r1 != nullptr);
  REQUIRE_THAT(r1->annotation, Catch::Matchers::WithinRel(3, 0.000001));
  REQUIRE_THAT(r1->approximate, Catch::Matchers::WithinRel(3.3, 0.000001));
  map.update_max_approx(t, 1, 1.1);
  auto r2 = map.get(t);
  REQUIRE(r2 != nullptr);
  REQUIRE_THAT(r2->annotation, Catch::Matchers::WithinRel(3, 0.000001));
  REQUIRE_THAT(r2->approximate, Catch::Matchers::WithinRel(3.3, 0.000001));
  map.update_max_approx(t, 4, 1.1);
  auto r3 = map.get(t);
  REQUIRE(r3 != nullptr);
  REQUIRE_THAT(r3->annotation, Catch::Matchers::WithinRel(4, 0.000001));
  REQUIRE_THAT(r3->approximate, Catch::Matchers::WithinRel(4.4, 0.000001));
}

TEST_CASE("MultiHashMap: should return delta in approximate value on "
          "update_max_approx") {
  auto map = MultiHashMap<
      MultiHashMapTestTriple,
      PrimaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash,
                       MultiHashMapTestTripleEqual>,
      SecondaryHashIndex<MultiHashMapTestTriple, MultiHashMapTestTripleHash01,
                         MultiHashMapTestTripleEqual01>>();
  auto t = MultiHashMapTestTriple();
  auto r1 = map.update_max_approx(t, 17, 1.1);
  auto e1 = 18.7;
  REQUIRE_THAT(r1, Catch::Matchers::WithinRel(e1, 0.000001));
  auto r2 = map.update_max_approx(t, 16, 1.1);
  auto e2 = 0;
  REQUIRE_THAT(r2, Catch::Matchers::WithinAbs(e2, 0.000001));
  auto r3 = map.update_max_approx(t, 18, 1.1);
  auto e3 = 0;
  REQUIRE_THAT(r3, Catch::Matchers::WithinAbs(e3, 0.000001));
  auto r4 = map.update_max_approx(t, 19, 1.1);
  auto e4 = 20.9;
  REQUIRE_THAT(r4, Catch::Matchers::WithinRel(e4, 0.000001));
}