#ifndef DYNAGOX_TPCH8_TUPLES_H
#define DYNAGOX_TPCH8_TUPLES_H

#include <cstddef>
#include <string>

struct PartTPCH8 {
  unsigned long p_partkey;

  double annotation;
  double approximate;
  bool active;

  PartTPCH8 *previous;
  PartTPCH8 *next;

  PartTPCH8()
      : p_partkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  PartTPCH8(const PartTPCH8 &other)
      : p_partkey(other.p_partkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct PartTPCH8Hash {
  inline static size_t hash(const PartTPCH8 &t) { return t.p_partkey + 0x9e3779b9; }
};

struct PartTPCH8Equal {
  inline static bool equal(const PartTPCH8 &lhs, const PartTPCH8 &rhs) {
    return lhs.p_partkey == rhs.p_partkey;
  }
};

struct SupplierTPCH8 {
  unsigned long s_suppkey;
  unsigned long s_nationkey;

  double annotation;
  double approximate;
  bool active;

  SupplierTPCH8 *previous;
  SupplierTPCH8 *next;

  SupplierTPCH8() : s_suppkey(0), s_nationkey(0),
                    annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  SupplierTPCH8(const SupplierTPCH8 &other) :
      s_suppkey(other.s_suppkey), s_nationkey(other.s_nationkey),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct SupplierTPCH8Hash0 {
  inline static size_t hash(const SupplierTPCH8 &t) { return t.s_suppkey + 0x9e3779b9; }
};

struct SupplierTPCH8Hash1 {
  inline static size_t hash(const SupplierTPCH8 &t) { return t.s_nationkey + 0x9e3779b9; }
};

struct SupplierTPCH8Equal0 {
  inline static bool equal(const SupplierTPCH8 &lhs, const SupplierTPCH8 &rhs) {
    return lhs.s_suppkey == rhs.s_suppkey;
  }
};

struct SupplierTPCH8Equal1 {
  inline static bool equal(const SupplierTPCH8 &lhs, const SupplierTPCH8 &rhs) {
    return lhs.s_nationkey == rhs.s_nationkey;
  }
};

struct SupplierJoinKeyTPCH8 {
  unsigned long s_suppkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  SupplierJoinKeyTPCH8 *previous;
  SupplierJoinKeyTPCH8 *next;

  SupplierJoinKeyTPCH8() : s_suppkey(0), n_name(""),
                           annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  SupplierJoinKeyTPCH8(const SupplierJoinKeyTPCH8 &other) :
      s_suppkey(other.s_suppkey), n_name(other.n_name),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct SupplierJoinKeyTPCH8Hash {
  inline static size_t hash(const SupplierJoinKeyTPCH8 &t) { return t.s_suppkey + 0x9e3779b9; }
};

struct SupplierJoinKeyTPCH8Equal {
  inline static bool equal(const SupplierJoinKeyTPCH8 &lhs, const SupplierJoinKeyTPCH8 &rhs) {
    return lhs.s_suppkey == rhs.s_suppkey;
  }
};

struct CustomerTPCH8 {
  unsigned long c_custkey;
  unsigned long c_nationkey;

  double annotation;
  double approximate;
  bool active;

  CustomerTPCH8 *previous;
  CustomerTPCH8 *next;

  CustomerTPCH8() : c_custkey(0), c_nationkey(0),
                    annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CustomerTPCH8(const CustomerTPCH8 &other) :
      c_custkey(other.c_custkey), c_nationkey(other.c_nationkey),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct CustomerTPCH8Hash0 {
  inline static size_t hash(const CustomerTPCH8 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerTPCH8Hash1 {
  inline static size_t hash(const CustomerTPCH8 &t) { return t.c_nationkey + 0x9e3779b9; }
};

struct CustomerTPCH8Equal0 {
  inline static bool equal(const CustomerTPCH8 &lhs, const CustomerTPCH8 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct CustomerTPCH8Equal1 {
  inline static bool equal(const CustomerTPCH8 &lhs, const CustomerTPCH8 &rhs) {
    return lhs.c_nationkey == rhs.c_nationkey;
  }
};

struct CustomerJoinKeyTPCH8 {
  unsigned long c_custkey;

  double annotation;
  double approximate;
  bool active;

  CustomerJoinKeyTPCH8 *previous;
  CustomerJoinKeyTPCH8 *next;

  CustomerJoinKeyTPCH8() : c_custkey(0),
                           annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CustomerJoinKeyTPCH8(const CustomerJoinKeyTPCH8 &other) : c_custkey(other.c_custkey),
                                                            annotation(0), approximate(0), active(false),
                                                            previous(nullptr),
                                                            next(nullptr) {}
};

struct CustomerJoinKeyTPCH8Hash {
  inline static size_t hash(const CustomerJoinKeyTPCH8 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerJoinKeyTPCH8Equal {
  inline static bool equal(const CustomerJoinKeyTPCH8 &lhs, const CustomerJoinKeyTPCH8 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct OrdersTPCH8 {
  unsigned long o_orderkey;
  unsigned long o_custkey;
  unsigned long o_year;

  double annotation;
  double approximate;
  bool active;

  OrdersTPCH8 *previous;
  OrdersTPCH8 *next;

  OrdersTPCH8() : o_orderkey(0), o_custkey(0), o_year(0),
                  annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersTPCH8(const OrdersTPCH8 &other) : o_orderkey(other.o_orderkey), o_custkey(other.o_custkey),
                                          o_year(other.o_year),
                                          annotation(other.annotation), approximate(other.approximate), active(false),
                                          previous(nullptr),
                                          next(nullptr) {}
};

struct OrdersTPCH8Hash0 {
  inline static size_t hash(const OrdersTPCH8 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersTPCH8Hash1 {
  inline static size_t hash(const OrdersTPCH8 &t) { return t.o_custkey + 0x9e3779b9; }
};

struct OrdersTPCH8Equal0 {
  inline static bool equal(const OrdersTPCH8 &lhs, const OrdersTPCH8 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct OrdersTPCH8Equal1 {
  inline static bool equal(const OrdersTPCH8 &lhs, const OrdersTPCH8 &rhs) {
    return lhs.o_custkey == rhs.o_custkey;
  }
};

struct OrdersJoinKeyTPCH8 {
  unsigned long o_orderkey;
  unsigned long o_year;

  double annotation;
  double approximate;
  bool active;

  OrdersJoinKeyTPCH8 *previous;
  OrdersJoinKeyTPCH8 *next;

  OrdersJoinKeyTPCH8() : o_orderkey(0), o_year(0),
                         annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersJoinKeyTPCH8(const OrdersJoinKeyTPCH8 &other) : o_orderkey(other.o_orderkey), o_year(other.o_year),
                                                        annotation(other.annotation), approximate(other.approximate),
                                                        active(false),
                                                        previous(nullptr),
                                                        next(nullptr) {}
};

struct OrdersJoinKeyTPCH8Hash {
  inline static size_t hash(const OrdersJoinKeyTPCH8 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersJoinKeyTPCH8Equal {
  inline static bool equal(const OrdersJoinKeyTPCH8 &lhs, const OrdersJoinKeyTPCH8 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct LineitemTPCH8 {
  unsigned long l_orderkey;
  unsigned long l_partkey;
  unsigned long l_suppkey;
  unsigned long l_linenumber;
  double l_extendedprice;
  double l_discount;

  double annotation;
  double approximate;
  bool active;

  LineitemTPCH8 *previous;
  LineitemTPCH8 *next;

  LineitemTPCH8()
      : l_orderkey(0), l_partkey(0), l_suppkey(0), l_linenumber(0), l_extendedprice(0), l_discount(0),
        annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  LineitemTPCH8(const LineitemTPCH8 &other)
      : l_orderkey(other.l_orderkey), l_partkey(other.l_partkey), l_suppkey(other.l_suppkey),
        l_linenumber(other.l_linenumber), l_extendedprice(other.l_extendedprice), l_discount(other.l_discount),
        annotation(other.annotation), approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct LineitemTPCH8Hash {
  inline static size_t hash(const LineitemTPCH8 &t) {
    size_t hash_value = t.l_orderkey + 0x9e3779b9;
    hash_value = t.l_linenumber + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct LineitemTPCH8Equal {
  inline static bool equal(const LineitemTPCH8 &lhs, const LineitemTPCH8 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey && lhs.l_linenumber == rhs.l_linenumber;
  }
};

struct LineitemTPCH8Hash1 {
  inline static size_t hash(const LineitemTPCH8 &t) {
    return t.l_partkey + 0x9e3779b9;
  }
};

struct LineitemTPCH8Equal1 {
  inline static bool equal(const LineitemTPCH8 &lhs, const LineitemTPCH8 &rhs) {
    return lhs.l_partkey == rhs.l_partkey;
  }
};

struct LineitemTPCH8Hash2 {
  inline static size_t hash(const LineitemTPCH8 &t) {
    return t.l_suppkey + 0x9e3779b9;
  }
};

struct LineitemTPCH8Equal2 {
  inline static bool equal(const LineitemTPCH8 &lhs, const LineitemTPCH8 &rhs) {
    return lhs.l_suppkey == rhs.l_suppkey;
  }
};

struct LineitemJoinKeyTPCH8 {
  unsigned long l_orderkey;

  double annotation;
  double approximate;
  bool active;

  LineitemJoinKeyTPCH8 *previous;
  LineitemJoinKeyTPCH8 *next;

  LineitemJoinKeyTPCH8()
      : l_orderkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  LineitemJoinKeyTPCH8(const LineitemJoinKeyTPCH8 &other)
      : l_orderkey(other.l_orderkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct LineitemJoinKeyTPCH8Hash {
  inline static size_t hash(const LineitemJoinKeyTPCH8 &t) { return t.l_orderkey + 0x9e3779b9; }
};

struct LineitemJoinKeyTPCH8Equal {
  inline static bool equal(const LineitemJoinKeyTPCH8 &lhs, const LineitemJoinKeyTPCH8 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey;
  }
};

struct Nation1TPCH8 {
  unsigned long n_nationkey;
  unsigned long n_regionkey;

  double annotation;
  double approximate;
  bool active;

  Nation1TPCH8 *previous;
  Nation1TPCH8 *next;

  Nation1TPCH8()
      : n_nationkey(0), n_regionkey(0), annotation(0), approximate(0), active(false),
        previous(nullptr),
        next(nullptr) {}

  Nation1TPCH8(const Nation1TPCH8 &other)
      : n_nationkey(other.n_nationkey), n_regionkey(other.n_regionkey),
        annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct Nation1TPCH8Hash0 {
  inline static size_t hash(const Nation1TPCH8 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct Nation1TPCH8Equal0 {
  inline static bool equal(const Nation1TPCH8 &lhs, const Nation1TPCH8 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct Nation1TPCH8Hash1 {
  inline static size_t hash(const Nation1TPCH8 &t) { return t.n_regionkey + 0x9e3779b9; }
};

struct Nation1TPCH8Equal1 {
  inline static bool equal(const Nation1TPCH8 &lhs, const Nation1TPCH8 &rhs) {
    return lhs.n_regionkey == rhs.n_regionkey;
  }
};

struct Nation1JoinKeyTPCH8 {
  unsigned long n_nationkey;

  double annotation;
  double approximate;
  bool active;

  Nation1JoinKeyTPCH8 *previous;
  Nation1JoinKeyTPCH8 *next;

  Nation1JoinKeyTPCH8()
      : n_nationkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  Nation1JoinKeyTPCH8(const Nation1JoinKeyTPCH8 &other)
      : n_nationkey(other.n_nationkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct Nation1JoinKeyTPCH8Hash {
  inline static size_t hash(const Nation1JoinKeyTPCH8 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct Nation1JoinKeyTPCH8Equal {
  inline static bool equal(const Nation1JoinKeyTPCH8 &lhs, const Nation1JoinKeyTPCH8 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct Nation2TPCH8 {
  unsigned long n_nationkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  Nation2TPCH8 *previous;
  Nation2TPCH8 *next;

  Nation2TPCH8()
      : n_nationkey(0), n_name(""), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  Nation2TPCH8(const Nation2TPCH8 &other)
      : n_nationkey(other.n_nationkey), n_name(other.n_name), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct Nation2TPCH8Hash {
  inline static size_t hash(const Nation2TPCH8 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct Nation2TPCH8Equal {
  inline static bool equal(const Nation2TPCH8 &lhs, const Nation2TPCH8 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct RegionTPCH8 {
  unsigned long r_regionkey;

  double annotation;
  double approximate;
  bool active;

  RegionTPCH8 *previous;
  RegionTPCH8 *next;

  RegionTPCH8()
      : r_regionkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  RegionTPCH8(const RegionTPCH8 &other)
      : r_regionkey(other.r_regionkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct RegionTPCH8Hash {
  inline static size_t hash(const RegionTPCH8 &t) { return t.r_regionkey + 0x9e3779b9; }
};

struct RegionTPCH8Equal {
  inline static bool equal(const RegionTPCH8 &lhs, const RegionTPCH8 &rhs) {
    return lhs.r_regionkey == rhs.r_regionkey;
  }
};

struct GOrdersTPCH8 {
  unsigned long o_year;

  double annotation;
  double approximate;
  bool active;

  GOrdersTPCH8 *previous;
  GOrdersTPCH8 *next;

  GOrdersTPCH8() : o_year(0),
                   annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  GOrdersTPCH8(const GOrdersTPCH8 &other) : o_year(other.o_year),
                                            annotation(other.annotation), approximate(other.approximate),
                                            active(false),
                                            previous(nullptr),
                                            next(nullptr) {}
};

struct GOrdersTPCH8Hash {
  inline static size_t hash(const GOrdersTPCH8 &t) {
    return t.o_year + 0x9e3779b9;
  }
};

struct GOrdersTPCH8Equal {
  inline static bool equal(const GOrdersTPCH8 &lhs, const GOrdersTPCH8 &rhs) {
    return lhs.o_year == rhs.o_year;
  }
};

#endif