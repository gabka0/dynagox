#ifndef DYNAGOX_TPCH5_TUPLES_H
#define DYNAGOX_TPCH5_TUPLES_H

#include <cstddef>
#include <string>

struct SupplierTPCH5 {
  unsigned long s_suppkey;
  unsigned long s_nationkey;

  double annotation;
  double approximate;
  bool active;

  SupplierTPCH5 *previous;
  SupplierTPCH5 *next;

  SupplierTPCH5() : s_suppkey(0), s_nationkey(0),
                    annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  SupplierTPCH5(const SupplierTPCH5 &other) :
      s_suppkey(other.s_suppkey), s_nationkey(other.s_nationkey),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct SupplierTPCH5Hash0 {
  inline static size_t hash(const SupplierTPCH5 &t) { return t.s_suppkey + 0x9e3779b9; }
};

struct SupplierTPCH5Hash1 {
  inline static size_t hash(const SupplierTPCH5 &t) { return t.s_nationkey + 0x9e3779b9; }
};

struct SupplierTPCH5Equal0 {
  inline static bool equal(const SupplierTPCH5 &lhs, const SupplierTPCH5 &rhs) {
    return lhs.s_suppkey == rhs.s_suppkey;
  }
};

struct SupplierTPCH5Equal1 {
  inline static bool equal(const SupplierTPCH5 &lhs, const SupplierTPCH5 &rhs) {
    return lhs.s_nationkey == rhs.s_nationkey;
  }
};

struct SupplierJoinKeyTPCH5 {
  unsigned long s_suppkey;
  unsigned long s_nationkey;

  double annotation;
  double approximate;
  bool active;

  SupplierJoinKeyTPCH5 *previous;
  SupplierJoinKeyTPCH5 *next;

  SupplierJoinKeyTPCH5() : s_suppkey(0), s_nationkey(0),
                           annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  SupplierJoinKeyTPCH5(const SupplierJoinKeyTPCH5 &other) :
      s_suppkey(other.s_suppkey), s_nationkey(other.s_nationkey),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct SupplierJoinKeyTPCH5Hash {
  inline static size_t hash(const SupplierJoinKeyTPCH5 &t) { return t.s_suppkey + 0x9e3779b9; }
};

struct SupplierJoinKeyTPCH5Equal {
  inline static bool equal(const SupplierJoinKeyTPCH5 &lhs, const SupplierJoinKeyTPCH5 &rhs) {
    return lhs.s_suppkey == rhs.s_suppkey;
  }
};

struct CustomerTPCH5 {
  unsigned long c_custkey;
  unsigned long c_nationkey;

  double annotation;
  double approximate;
  bool active;

  CustomerTPCH5 *previous;
  CustomerTPCH5 *next;

  CustomerTPCH5() : c_custkey(0), c_nationkey(0),
                    annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CustomerTPCH5(const CustomerTPCH5 &other) :
      c_custkey(other.c_custkey), c_nationkey(other.c_nationkey),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct CustomerTPCH5Hash0 {
  inline static size_t hash(const CustomerTPCH5 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerTPCH5Hash1 {
  inline static size_t hash(const CustomerTPCH5 &t) { return t.c_nationkey + 0x9e3779b9; }
};

struct CustomerTPCH5Equal0 {
  inline static bool equal(const CustomerTPCH5 &lhs, const CustomerTPCH5 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct CustomerTPCH5Equal1 {
  inline static bool equal(const CustomerTPCH5 &lhs, const CustomerTPCH5 &rhs) {
    return lhs.c_nationkey == rhs.c_nationkey;
  }
};

struct CustomerJoinKeyTPCH5 {
  unsigned long c_custkey;
  unsigned long c_nationkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  CustomerJoinKeyTPCH5 *previous;
  CustomerJoinKeyTPCH5 *next;

  CustomerJoinKeyTPCH5() : c_custkey(0), c_nationkey(0), n_name(""),
                           annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CustomerJoinKeyTPCH5(const CustomerJoinKeyTPCH5 &other) : c_custkey(other.c_custkey), c_nationkey(other.c_nationkey),
                                                            n_name(other.n_name),
                                                            annotation(0), approximate(0), active(false),
                                                            previous(nullptr),
                                                            next(nullptr) {}
};

struct CustomerJoinKeyTPCH5Hash {
  inline static size_t hash(const CustomerJoinKeyTPCH5 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerJoinKeyTPCH5Equal {
  inline static bool equal(const CustomerJoinKeyTPCH5 &lhs, const CustomerJoinKeyTPCH5 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct OrdersTPCH5 {
  unsigned long o_orderkey;
  unsigned long o_custkey;

  double annotation;
  double approximate;
  bool active;

  OrdersTPCH5 *previous;
  OrdersTPCH5 *next;

  OrdersTPCH5() : o_orderkey(0), o_custkey(0),
                  annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersTPCH5(const OrdersTPCH5 &other) : o_orderkey(other.o_orderkey), o_custkey(other.o_custkey),
                                          annotation(other.annotation), approximate(other.approximate), active(false),
                                          previous(nullptr),
                                          next(nullptr) {}
};

struct OrdersTPCH5Hash0 {
  inline static size_t hash(const OrdersTPCH5 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersTPCH5Hash1 {
  inline static size_t hash(const OrdersTPCH5 &t) { return t.o_custkey + 0x9e3779b9; }
};

struct OrdersTPCH5Equal0 {
  inline static bool equal(const OrdersTPCH5 &lhs, const OrdersTPCH5 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct OrdersTPCH5Equal1 {
  inline static bool equal(const OrdersTPCH5 &lhs, const OrdersTPCH5 &rhs) {
    return lhs.o_custkey == rhs.o_custkey;
  }
};

struct OrdersJoinKeyTPCH5 {
  unsigned long o_orderkey;
  unsigned long c_nationkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  OrdersJoinKeyTPCH5 *previous;
  OrdersJoinKeyTPCH5 *next;

  OrdersJoinKeyTPCH5() : o_orderkey(0), c_nationkey(0), n_name(""),
                         annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersJoinKeyTPCH5(const OrdersJoinKeyTPCH5 &other) : o_orderkey(other.o_orderkey), c_nationkey(other.c_nationkey),
                                                        n_name(other.n_name),
                                                        annotation(other.annotation), approximate(other.approximate),
                                                        active(false),
                                                        previous(nullptr),
                                                        next(nullptr) {}
};

struct OrdersJoinKeyTPCH5Hash {
  inline static size_t hash(const OrdersJoinKeyTPCH5 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersJoinKeyTPCH5Equal {
  inline static bool equal(const OrdersJoinKeyTPCH5 &lhs, const OrdersJoinKeyTPCH5 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct LineitemTPCH5 {
  unsigned long l_orderkey;
  unsigned long l_suppkey;
  unsigned long l_linenumber;
  double l_extendedprice;
  double l_discount;

  double annotation;
  double approximate;
  bool active;

  LineitemTPCH5 *previous;
  LineitemTPCH5 *next;

  LineitemTPCH5()
      : l_orderkey(0), l_suppkey(0), l_linenumber(0), l_extendedprice(0), l_discount(0),
        annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  LineitemTPCH5(const LineitemTPCH5 &other)
      : l_orderkey(other.l_orderkey), l_suppkey(other.l_suppkey), l_linenumber(other.l_linenumber),
        l_extendedprice(other.l_extendedprice), l_discount(other.l_discount),
        annotation(other.annotation), approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct LineitemTPCH5Hash {
  inline static size_t hash(const LineitemTPCH5 &t) {
    size_t hash_value = t.l_orderkey + 0x9e3779b9;
    hash_value = t.l_linenumber + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct LineitemTPCH5Equal {
  inline static bool equal(const LineitemTPCH5 &lhs, const LineitemTPCH5 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey && lhs.l_linenumber == rhs.l_linenumber;
  }
};

struct LineitemTPCH5Hash0 {
  inline static size_t hash(const LineitemTPCH5 &t) {
    return t.l_orderkey + 0x9e3779b9;
  }
};

struct LineitemTPCH5Equal0 {
  inline static bool equal(const LineitemTPCH5 &lhs, const LineitemTPCH5 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey;
  }
};

struct LineitemTPCH5Hash1 {
  inline static size_t hash(const LineitemTPCH5 &t) {
    return t.l_suppkey + 0x9e3779b9;
  }
};

struct LineitemTPCH5Equal1 {
  inline static bool equal(const LineitemTPCH5 &lhs, const LineitemTPCH5 &rhs) {
    return lhs.l_suppkey == rhs.l_suppkey;
  }
};

struct NationTPCH5 {
  unsigned long n_nationkey;
  std::string n_name;
  unsigned long n_regionkey;

  double annotation;
  double approximate;
  bool active;

  NationTPCH5 *previous;
  NationTPCH5 *next;

  NationTPCH5()
      : n_nationkey(0), n_name(""), n_regionkey(0), annotation(0), approximate(0), active(false),
        previous(nullptr),
        next(nullptr) {}

  NationTPCH5(const NationTPCH5 &other)
      : n_nationkey(other.n_nationkey), n_name(other.n_name), n_regionkey(other.n_regionkey),
        annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct NationTPCH5Hash0 {
  inline static size_t hash(const NationTPCH5 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct NationTPCH5Equal0 {
  inline static bool equal(const NationTPCH5 &lhs, const NationTPCH5 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct NationTPCH5Hash2 {
  inline static size_t hash(const NationTPCH5 &t) { return t.n_regionkey + 0x9e3779b9; }
};

struct NationTPCH5Equal2 {
  inline static bool equal(const NationTPCH5 &lhs, const NationTPCH5 &rhs) {
    return lhs.n_regionkey == rhs.n_regionkey;
  }
};

struct NationJoinKeyTPCH5 {
  unsigned long n_nationkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  NationJoinKeyTPCH5 *previous;
  NationJoinKeyTPCH5 *next;

  NationJoinKeyTPCH5()
      : n_nationkey(0), n_name(""), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  NationJoinKeyTPCH5(const NationJoinKeyTPCH5 &other)
      : n_nationkey(other.n_nationkey), n_name(other.n_name), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct NationJoinKeyTPCH5Hash {
  inline static size_t hash(const NationJoinKeyTPCH5 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct NationJoinKeyTPCH5Equal {
  inline static bool equal(const NationJoinKeyTPCH5 &lhs, const NationJoinKeyTPCH5 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct RegionTPCH5 {
  unsigned long r_regionkey;

  double annotation;
  double approximate;
  bool active;

  RegionTPCH5 *previous;
  RegionTPCH5 *next;

  RegionTPCH5()
      : r_regionkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  RegionTPCH5(const RegionTPCH5 &other)
      : r_regionkey(other.r_regionkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct RegionTPCH5Hash {
  inline static size_t hash(const RegionTPCH5 &t) { return t.r_regionkey + 0x9e3779b9; }
};

struct RegionTPCH5Equal {
  inline static bool equal(const RegionTPCH5 &lhs, const RegionTPCH5 &rhs) {
    return lhs.r_regionkey == rhs.r_regionkey;
  }
};

struct GLineitemTPCH5 {
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  GLineitemTPCH5 *previous;
  GLineitemTPCH5 *next;

  GLineitemTPCH5() : n_name(""),
                     annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  GLineitemTPCH5(const GLineitemTPCH5 &other) : n_name(other.n_name),
                                                annotation(other.annotation), approximate(other.approximate),
                                                active(false),
                                                previous(nullptr),
                                                next(nullptr) {}
};

struct GLineitemTPCH5Hash {
  inline static size_t hash(const GLineitemTPCH5 &t) {
    std::hash<std::string> hash;
    return hash(t.n_name);
  }
};

struct GLineitemTPCH5Equal {
  inline static bool equal(const GLineitemTPCH5 &lhs, const GLineitemTPCH5 &rhs) {
    return lhs.n_name == rhs.n_name;
  }
};

#endif
