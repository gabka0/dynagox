#ifndef DYNAGOX_TPCH3_TUPLES_H
#define DYNAGOX_TPCH3_TUPLES_H

#include <cstddef>
#include <string>

struct CustomerTPCH3 {
  unsigned long c_custkey;

  double annotation;
  double approximate;
  bool active;

  CustomerTPCH3 *previous;
  CustomerTPCH3 *next;

  CustomerTPCH3()
      : c_custkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerTPCH3(const CustomerTPCH3 &other)
      : c_custkey(other.c_custkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerTPCH3Hash {
  inline static size_t hash(const CustomerTPCH3 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerTPCH3Equal {
  inline static bool equal(const CustomerTPCH3 &lhs, const CustomerTPCH3 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct OrdersTPCH3 {
  unsigned long o_orderkey;
  unsigned long o_custkey;
  unsigned long o_orderdate;
  unsigned long o_shippriority;

  double annotation;
  double approximate;
  bool active;

  OrdersTPCH3 *previous;
  OrdersTPCH3 *next;

  OrdersTPCH3() : o_orderkey(0), o_custkey(0), o_orderdate(0), o_shippriority(0),
                  annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersTPCH3(const OrdersTPCH3 &other) : o_orderkey(other.o_orderkey), o_custkey(other.o_custkey),
                                          o_orderdate(other.o_orderdate), o_shippriority(other.o_shippriority),
                                          annotation(other.annotation), approximate(other.approximate), active(false),
                                          previous(nullptr),
                                          next(nullptr) {}
};

struct OrdersTPCH3Hash0 {
  inline static size_t hash(const OrdersTPCH3 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersTPCH3Hash1 {
  inline static size_t hash(const OrdersTPCH3 &t) { return t.o_custkey + 0x9e3779b9; }
};

struct OrdersTPCH3Equal0 {
  inline static bool equal(const OrdersTPCH3 &lhs, const OrdersTPCH3 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct OrdersTPCH3Equal1 {
  inline static bool equal(const OrdersTPCH3 &lhs, const OrdersTPCH3 &rhs) {
    return lhs.o_custkey == rhs.o_custkey;
  }
};

struct OrdersJoinKeyTPCH3 {
  unsigned long o_orderkey;
  unsigned long o_orderdate;
  unsigned long o_shippriority;

  double annotation;
  double approximate;
  bool active;

  OrdersJoinKeyTPCH3 *previous;
  OrdersJoinKeyTPCH3 *next;

  OrdersJoinKeyTPCH3() : o_orderkey(0), o_orderdate(0), o_shippriority(0),
                         annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersJoinKeyTPCH3(const OrdersJoinKeyTPCH3 &other) : o_orderkey(other.o_orderkey),
                                                        o_orderdate(other.o_orderdate),
                                                        o_shippriority(other.o_shippriority),
                                                        annotation(other.annotation), approximate(other.approximate),
                                                        active(false),
                                                        previous(nullptr),
                                                        next(nullptr) {}
};

struct OrdersJoinKeyTPCH3Hash {
  inline static size_t hash(const OrdersJoinKeyTPCH3 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersJoinKeyTPCH3Equal {
  inline static bool equal(const OrdersJoinKeyTPCH3 &lhs, const OrdersJoinKeyTPCH3 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct LineitemJoinKeyTPCH3 {
  unsigned long l_orderkey;

  double annotation;
  double approximate;
  bool active;

  LineitemJoinKeyTPCH3 *previous;
  LineitemJoinKeyTPCH3 *next;

  LineitemJoinKeyTPCH3()
      : l_orderkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  LineitemJoinKeyTPCH3(const LineitemJoinKeyTPCH3 &other)
      : l_orderkey(other.l_orderkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct LineitemJoinKeyTPCH3Hash {
  inline static size_t hash(const LineitemJoinKeyTPCH3 &t) { return t.l_orderkey + 0x9e3779b9; }
};

struct LineitemJoinKeyTPCH3Equal {
  inline static bool equal(const LineitemJoinKeyTPCH3 &lhs, const LineitemJoinKeyTPCH3 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey;
  }
};

struct GOrdersTPCH3 {
  unsigned long o_orderkey;
  unsigned long o_orderdate;
  unsigned long o_shippriority;

  double annotation;
  double approximate;
  bool active;

  GOrdersTPCH3 *previous;
  GOrdersTPCH3 *next;

  GOrdersTPCH3() : o_orderkey(0), o_orderdate(0), o_shippriority(0),
                   annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  GOrdersTPCH3(const GOrdersTPCH3 &other) : o_orderkey(other.o_orderkey),
                                            o_orderdate(other.o_orderdate), o_shippriority(other.o_shippriority),
                                            annotation(other.annotation), approximate(other.approximate), active(false),
                                            previous(nullptr),
                                            next(nullptr) {}
};

struct GOrdersTPCH3Hash {
  inline static size_t hash(const GOrdersTPCH3 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct GOrdersTPCH3Equal {
  inline static bool equal(const GOrdersTPCH3 &lhs, const GOrdersTPCH3 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

#endif
