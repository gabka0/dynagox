#ifndef DYNAGOX_TPCH9_TUPLES_H
#define DYNAGOX_TPCH9_TUPLES_H

#include <cstddef>
#include <string>

struct PartTPCH9 {
  unsigned long p_partkey;

  double annotation;
  double approximate;
  bool active;

  PartTPCH9 *previous;
  PartTPCH9 *next;

  PartTPCH9()
      : p_partkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  PartTPCH9(const PartTPCH9 &other)
      : p_partkey(other.p_partkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct PartTPCH9Hash {
  inline static size_t hash(const PartTPCH9 &t) { return t.p_partkey + 0x9e3779b9; }
};

struct PartTPCH9Equal {
  inline static bool equal(const PartTPCH9 &lhs, const PartTPCH9 &rhs) {
    return lhs.p_partkey == rhs.p_partkey;
  }
};

struct PartSuppTPCH9 {
  unsigned long ps_partkey;
  unsigned long ps_suppkey;
  double ps_supplycost;

  double annotation;
  double approximate;
  bool active;

  PartSuppTPCH9 *previous;
  PartSuppTPCH9 *next;

  PartSuppTPCH9()
      : ps_partkey(0), ps_suppkey(0), ps_supplycost(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  PartSuppTPCH9(const PartSuppTPCH9 &other)
      : ps_partkey(other.ps_partkey), ps_suppkey(other.ps_suppkey), ps_supplycost(other.ps_supplycost),
        annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct PartSuppTPCH9Hash {
  inline static size_t hash(const PartSuppTPCH9 &t) {
    size_t hash_value = t.ps_partkey + 0x9e3779b9;
    hash_value = t.ps_suppkey + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct PartSuppTPCH9Equal {
  inline static bool equal(const PartSuppTPCH9 &lhs, const PartSuppTPCH9 &rhs) {
    return lhs.ps_partkey == rhs.ps_partkey && lhs.ps_suppkey == rhs.ps_suppkey;
  }
};

struct SupplierTPCH9 {
  unsigned long s_suppkey;
  unsigned long s_nationkey;

  double annotation;
  double approximate;
  bool active;

  SupplierTPCH9 *previous;
  SupplierTPCH9 *next;

  SupplierTPCH9() : s_suppkey(0), s_nationkey(0),
                    annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  SupplierTPCH9(const SupplierTPCH9 &other) :
      s_suppkey(other.s_suppkey), s_nationkey(other.s_nationkey),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct SupplierTPCH9Hash0 {
  inline static size_t hash(const SupplierTPCH9 &t) { return t.s_suppkey + 0x9e3779b9; }
};

struct SupplierTPCH9Hash1 {
  inline static size_t hash(const SupplierTPCH9 &t) { return t.s_nationkey + 0x9e3779b9; }
};

struct SupplierTPCH9Equal0 {
  inline static bool equal(const SupplierTPCH9 &lhs, const SupplierTPCH9 &rhs) {
    return lhs.s_suppkey == rhs.s_suppkey;
  }
};

struct SupplierTPCH9Equal1 {
  inline static bool equal(const SupplierTPCH9 &lhs, const SupplierTPCH9 &rhs) {
    return lhs.s_nationkey == rhs.s_nationkey;
  }
};

struct SupplierJoinKeyTPCH9 {
  unsigned long s_suppkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  SupplierJoinKeyTPCH9 *previous;
  SupplierJoinKeyTPCH9 *next;

  SupplierJoinKeyTPCH9() : s_suppkey(0), n_name(""),
                           annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  SupplierJoinKeyTPCH9(const SupplierJoinKeyTPCH9 &other) :
      s_suppkey(other.s_suppkey), n_name(other.n_name),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct SupplierJoinKeyTPCH9Hash {
  inline static size_t hash(const SupplierJoinKeyTPCH9 &t) { return t.s_suppkey + 0x9e3779b9; }
};

struct SupplierJoinKeyTPCH9Equal {
  inline static bool equal(const SupplierJoinKeyTPCH9 &lhs, const SupplierJoinKeyTPCH9 &rhs) {
    return lhs.s_suppkey == rhs.s_suppkey;
  }
};

struct OrdersTPCH9 {
  unsigned long o_orderkey;
  unsigned long o_year;

  double annotation;
  double approximate;
  bool active;

  OrdersTPCH9 *previous;
  OrdersTPCH9 *next;

  OrdersTPCH9() : o_orderkey(0), o_year(0),
                  annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersTPCH9(const OrdersTPCH9 &other) : o_orderkey(other.o_orderkey), o_year(other.o_year),
                                          annotation(other.annotation), approximate(other.approximate), active(false),
                                          previous(nullptr),
                                          next(nullptr) {}
};

struct OrdersTPCH9Hash {
  inline static size_t hash(const OrdersTPCH9 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersTPCH9Equal {
  inline static bool equal(const OrdersTPCH9 &lhs, const OrdersTPCH9 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct LineitemTPCH9 {
  unsigned long l_orderkey;
  unsigned long l_partkey;
  unsigned long l_suppkey;
  unsigned long l_linenumber;
  double l_quantity;
  double l_extendedprice;
  double l_discount;

  double annotation;
  double approximate;
  bool active;

  LineitemTPCH9 *previous;
  LineitemTPCH9 *next;

  LineitemTPCH9()
      : l_orderkey(0), l_partkey(0), l_suppkey(0), l_linenumber(0), l_quantity(0), l_extendedprice(0), l_discount(0),
        annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  LineitemTPCH9(const LineitemTPCH9 &other)
      : l_orderkey(other.l_orderkey), l_partkey(other.l_partkey), l_suppkey(other.l_suppkey),
        l_linenumber(other.l_linenumber), l_quantity(other.l_quantity), l_extendedprice(other.l_extendedprice),
        l_discount(other.l_discount),
        annotation(other.annotation), approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct LineitemTPCH9Hash {
  inline static size_t hash(const LineitemTPCH9 &t) {
    size_t hash_value = t.l_orderkey + 0x9e3779b9;
    hash_value = t.l_linenumber + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct LineitemTPCH9Equal {
  inline static bool equal(const LineitemTPCH9 &lhs, const LineitemTPCH9 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey && lhs.l_linenumber == rhs.l_linenumber;
  }
};

struct LineitemTPCH9Hash0 {
  inline static size_t hash(const LineitemTPCH9 &t) {
    return t.l_orderkey + 0x9e3779b9;
  }
};

struct LineitemTPCH9Equal0 {
  inline static bool equal(const LineitemTPCH9 &lhs, const LineitemTPCH9 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey;
  }
};

struct LineitemTPCH9Hash1 {
  inline static size_t hash(const LineitemTPCH9 &t) {
    return t.l_partkey + 0x9e3779b9;
  }
};

struct LineitemTPCH9Equal1 {
  inline static bool equal(const LineitemTPCH9 &lhs, const LineitemTPCH9 &rhs) {
    return lhs.l_partkey == rhs.l_partkey;
  }
};

struct LineitemTPCH9Hash2 {
  inline static size_t hash(const LineitemTPCH9 &t) {
    return t.l_suppkey + 0x9e3779b9;
  }
};

struct LineitemTPCH9Equal2 {
  inline static bool equal(const LineitemTPCH9 &lhs, const LineitemTPCH9 &rhs) {
    return lhs.l_suppkey == rhs.l_suppkey;
  }
};

struct LineitemTPCH9Hash12 {
  inline static size_t hash(const LineitemTPCH9 &t) {
    size_t hash_value = t.l_partkey + 0x9e3779b9;
    hash_value = t.l_suppkey + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct LineitemTPCH9Equal12 {
  inline static bool equal(const LineitemTPCH9 &lhs, const LineitemTPCH9 &rhs) {
    return lhs.l_partkey == rhs.l_partkey && lhs.l_suppkey == rhs.l_suppkey;
  }
};

struct NationTPCH9 {
  unsigned long n_nationkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  NationTPCH9 *previous;
  NationTPCH9 *next;

  NationTPCH9()
      : n_nationkey(0), n_name(""), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  NationTPCH9(const NationTPCH9 &other)
      : n_nationkey(other.n_nationkey), n_name(other.n_name), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct NationTPCH9Hash {
  inline static size_t hash(const NationTPCH9 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct NationTPCH9Equal {
  inline static bool equal(const NationTPCH9 &lhs, const NationTPCH9 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct GLineitemTPCH9 {
  std::string n_name;
  unsigned long o_year;

  double annotation;
  double approximate;
  bool active;

  GLineitemTPCH9 *previous;
  GLineitemTPCH9 *next;

  GLineitemTPCH9() : n_name(""), o_year(0),
                     annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  GLineitemTPCH9(const GLineitemTPCH9 &other) : n_name(other.n_name), o_year(other.o_year),
                                                annotation(other.annotation), approximate(other.approximate),
                                                active(false),
                                                previous(nullptr),
                                                next(nullptr) {}
};

struct GLineitemTPCH9Hash {
  inline static size_t hash(const GLineitemTPCH9 &t) {
    std::hash<std::string> hash;
    auto hash_value = hash(t.n_name);
    hash_value = t.o_year + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct GLineitemTPCH9Equal {
  inline static bool equal(const GLineitemTPCH9 &lhs, const GLineitemTPCH9 &rhs) {
    return lhs.n_name == rhs.n_name && lhs.o_year == rhs.o_year;
  }
};

#endif