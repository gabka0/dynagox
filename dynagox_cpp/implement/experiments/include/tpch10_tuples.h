#ifndef DYNAGOX_TPCH10_TUPLES_H
#define DYNAGOX_TPCH10_TUPLES_H

#include <cstddef>
#include <string>

struct CustomerTPCH10 {
  unsigned long c_custkey;
  std::string c_name;
  std::string c_address;
  unsigned long c_nationkey;
  std::string c_phone;
  double c_acctbal;
  std::string c_comment;

  double annotation;
  double approximate;
  bool active;

  CustomerTPCH10 *previous;
  CustomerTPCH10 *next;

  CustomerTPCH10() : c_custkey(0), c_name(""), c_address(""), c_nationkey(0), c_phone(""), c_acctbal(0), c_comment(""),
                     annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CustomerTPCH10(const CustomerTPCH10 &other) :
      c_custkey(other.c_custkey), c_name(other.c_name), c_address(other.c_address), c_nationkey(other.c_nationkey),
      c_phone(other.c_phone), c_acctbal(other.c_acctbal), c_comment(other.c_comment),
      annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}
};

struct CustomerTPCH10Hash0 {
  inline static size_t hash(const CustomerTPCH10 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerTPCH10Hash1 {
  inline static size_t hash(const CustomerTPCH10 &t) { return t.c_nationkey + 0x9e3779b9; }
};

struct CustomerTPCH10Equal0 {
  inline static bool equal(const CustomerTPCH10 &lhs, const CustomerTPCH10 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct CustomerTPCH10Equal1 {
  inline static bool equal(const CustomerTPCH10 &lhs, const CustomerTPCH10 &rhs) {
    return lhs.c_nationkey == rhs.c_nationkey;
  }
};

struct CustomerJoinKeyTPCH10 {
  unsigned long c_custkey;
  std::string c_name;
  std::string c_address;
  std::string c_phone;
  double c_acctbal;
  std::string c_comment;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  CustomerJoinKeyTPCH10 *previous;
  CustomerJoinKeyTPCH10 *next;

  CustomerJoinKeyTPCH10() : c_custkey(0), c_name(""), c_address(""), c_phone(""), c_acctbal(0), c_comment(""),
                            n_name(""),
                            annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CustomerJoinKeyTPCH10(const CustomerJoinKeyTPCH10 &other) : c_custkey(other.c_custkey), c_name(other.c_name),
                                                              c_address(other.c_address),
                                                              c_phone(other.c_phone), c_acctbal(other.c_acctbal),
                                                              c_comment(other.c_comment), n_name(other.n_name),
                                                              annotation(0), approximate(0), active(false),
                                                              previous(nullptr),
                                                              next(nullptr) {}
};

struct CustomerJoinKeyTPCH10Hash {
  inline static size_t hash(const CustomerJoinKeyTPCH10 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct CustomerJoinKeyTPCH10Equal {
  inline static bool equal(const CustomerJoinKeyTPCH10 &lhs, const CustomerJoinKeyTPCH10 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

struct OrdersTPCH10 {
  unsigned long o_orderkey;
  unsigned long o_custkey;

  double annotation;
  double approximate;
  bool active;

  OrdersTPCH10 *previous;
  OrdersTPCH10 *next;

  OrdersTPCH10() : o_orderkey(0), o_custkey(0),
                   annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersTPCH10(const OrdersTPCH10 &other) : o_orderkey(other.o_orderkey), o_custkey(other.o_custkey),
                                            annotation(other.annotation), approximate(other.approximate), active(false),
                                            previous(nullptr),
                                            next(nullptr) {}
};

struct OrdersTPCH10Hash0 {
  inline static size_t hash(const OrdersTPCH10 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersTPCH10Hash1 {
  inline static size_t hash(const OrdersTPCH10 &t) { return t.o_custkey + 0x9e3779b9; }
};

struct OrdersTPCH10Equal0 {
  inline static bool equal(const OrdersTPCH10 &lhs, const OrdersTPCH10 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct OrdersTPCH10Equal1 {
  inline static bool equal(const OrdersTPCH10 &lhs, const OrdersTPCH10 &rhs) {
    return lhs.o_custkey == rhs.o_custkey;
  }
};

struct OrdersJoinKeyTPCH10 {
  unsigned long o_orderkey;
  unsigned long c_custkey;
  std::string c_name;
  std::string c_address;
  std::string c_phone;
  double c_acctbal;
  std::string c_comment;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  OrdersJoinKeyTPCH10 *previous;
  OrdersJoinKeyTPCH10 *next;

  OrdersJoinKeyTPCH10() : o_orderkey(0), c_custkey(0), c_name(""), c_address(""), c_phone(""), c_acctbal(0),
                          c_comment(""), n_name(""),
                          annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  OrdersJoinKeyTPCH10(const OrdersJoinKeyTPCH10 &other) : o_orderkey(other.o_orderkey),
                                                          c_custkey(other.c_custkey), c_name(other.c_name),
                                                          c_address(other.c_address),
                                                          c_phone(other.c_phone), c_acctbal(other.c_acctbal),
                                                          c_comment(other.c_comment), n_name(other.n_name),
                                                          annotation(other.annotation), approximate(other.approximate),
                                                          active(false),
                                                          previous(nullptr),
                                                          next(nullptr) {}
};

struct OrdersJoinKeyTPCH10Hash {
  inline static size_t hash(const OrdersJoinKeyTPCH10 &t) { return t.o_orderkey + 0x9e3779b9; }
};

struct OrdersJoinKeyTPCH10Equal {
  inline static bool equal(const OrdersJoinKeyTPCH10 &lhs, const OrdersJoinKeyTPCH10 &rhs) {
    return lhs.o_orderkey == rhs.o_orderkey;
  }
};

struct LineitemJoinKeyTPCH10 {
  unsigned long l_orderkey;

  double annotation;
  double approximate;
  bool active;

  LineitemJoinKeyTPCH10 *previous;
  LineitemJoinKeyTPCH10 *next;

  LineitemJoinKeyTPCH10()
      : l_orderkey(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  LineitemJoinKeyTPCH10(const LineitemJoinKeyTPCH10 &other)
      : l_orderkey(other.l_orderkey), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct LineitemJoinKeyTPCH10Hash {
  inline static size_t hash(const LineitemJoinKeyTPCH10 &t) { return t.l_orderkey + 0x9e3779b9; }
};

struct LineitemJoinKeyTPCH10Equal {
  inline static bool equal(const LineitemJoinKeyTPCH10 &lhs, const LineitemJoinKeyTPCH10 &rhs) {
    return lhs.l_orderkey == rhs.l_orderkey;
  }
};

struct NationTPCH10 {
  unsigned long n_nationkey;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  NationTPCH10 *previous;
  NationTPCH10 *next;

  NationTPCH10()
      : n_nationkey(0), n_name(""), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  NationTPCH10(const NationTPCH10 &other)
      : n_nationkey(other.n_nationkey), n_name(other.n_name), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct NationTPCH10Hash {
  inline static size_t hash(const NationTPCH10 &t) { return t.n_nationkey + 0x9e3779b9; }
};

struct NationTPCH10Equal {
  inline static bool equal(const NationTPCH10 &lhs, const NationTPCH10 &rhs) {
    return lhs.n_nationkey == rhs.n_nationkey;
  }
};

struct GOrdersTPCH10 {
  unsigned long c_custkey;
  std::string c_name;
  std::string c_address;
  std::string c_phone;
  double c_acctbal;
  std::string c_comment;
  std::string n_name;

  double annotation;
  double approximate;
  bool active;

  GOrdersTPCH10 *previous;
  GOrdersTPCH10 *next;

  GOrdersTPCH10() : c_custkey(0), c_name(""), c_address(""), c_phone(""), c_acctbal(0), c_comment(""), n_name(""),
                    annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  GOrdersTPCH10(const GOrdersTPCH10 &other) : c_custkey(other.c_custkey), c_name(other.c_name),
                                              c_address(other.c_address),
                                              c_phone(other.c_phone), c_acctbal(other.c_acctbal),
                                              c_comment(other.c_comment), n_name(other.n_name),
                                              annotation(other.annotation), approximate(other.approximate),
                                              active(false),
                                              previous(nullptr),
                                              next(nullptr) {}
};

struct GOrdersTPCH10Hash {
  inline static size_t hash(const GOrdersTPCH10 &t) { return t.c_custkey + 0x9e3779b9; }
};

struct GOrdersTPCH10Equal {
  inline static bool equal(const GOrdersTPCH10 &lhs, const GOrdersTPCH10 &rhs) {
    return lhs.c_custkey == rhs.c_custkey;
  }
};

#endif
