#ifndef DYNAGOX_TUPLES_H
#define DYNAGOX_TUPLES_H

#include <cstddef>

struct Tuple1 {
  unsigned long f0;

  double annotation;
  double approximate;
  bool active;

  Tuple1 *previous;
  Tuple1 *next;

  Tuple1()
      : f0(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  Tuple1(const Tuple1 &other)
      : f0(other.f0), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct Tuple2 {
  unsigned long f0;
  unsigned long f1;

  double annotation;
  double approximate;
  bool active;

  Tuple2 *previous;
  Tuple2 *next;

  Tuple2()
      : f0(0), f1(0), annotation(0), approximate(0), active(false),
        previous(nullptr), next(nullptr) {}

  Tuple2(const Tuple2 &other)
      : f0(other.f0), f1(other.f1), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct Tuple1Hash {
  inline static size_t hash(const Tuple1 &t) { return t.f0 + 0x9e3779b9; }
};

struct Tuple1Equal {
  inline static bool equal(const Tuple1 &lhs, const Tuple1 &rhs) {
    return lhs.f0 == rhs.f0;
  }
};

struct Tuple2Hash {
  inline static size_t hash(const Tuple2 &t) {
    size_t hash_value = t.f0 + 0x9e3779b9;
    hash_value = t.f1 + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct Tuple2Equal {
  inline static bool equal(const Tuple2 &lhs, const Tuple2 &rhs) {
    return lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1;
  }
};

struct Tuple2Hash0 {
  inline static size_t hash(const Tuple2 &t) { return t.f0 + 0x9e3779b9; }
};

struct Tuple2Equal0 {
  inline static bool equal(const Tuple2 &lhs, const Tuple2 &rhs) {
    return lhs.f0 == rhs.f0;
  }
};

struct Tuple2Hash1 {
  inline static size_t hash(const Tuple2 &t) { return t.f1 + 0x9e3779b9; }
};

struct Tuple2Equal1 {
  inline static bool equal(const Tuple2 &lhs, const Tuple2 &rhs) {
    return lhs.f1 == rhs.f1;
  }
};

#endif
