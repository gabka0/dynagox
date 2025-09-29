#ifndef DYNAGOX_TPCDS19_TUPLES_H
#define DYNAGOX_TPCDS19_TUPLES_H

#include <cstddef>
#include <string>

struct StoreSalesTPCDS19 {
  unsigned long ss_sold_date_sk;
  unsigned long ss_item_sk;
  unsigned long ss_customer_sk;
  unsigned long ss_store_sk;
  double ss_ext_sales_price;

  double annotation;
  double approximate;
  bool active;

  StoreSalesTPCDS19 *previous;
  StoreSalesTPCDS19 *next;

  StoreSalesTPCDS19()
      : ss_sold_date_sk(0), ss_item_sk(0), ss_customer_sk(0), ss_store_sk(0), ss_ext_sales_price(0),
        annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  StoreSalesTPCDS19(const StoreSalesTPCDS19 &other)
      : ss_sold_date_sk(other.ss_sold_date_sk), ss_item_sk(other.ss_item_sk), ss_customer_sk(other.ss_customer_sk),
        ss_store_sk(other.ss_store_sk), ss_ext_sales_price(other.ss_ext_sales_price),
        annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct StoreSalesTPCDS19Hash {
  inline static size_t hash(const StoreSalesTPCDS19 &t) {
    std::hash<double> hash;
    auto hash_value = hash(t.ss_ext_sales_price);
    hash_value = t.ss_sold_date_sk + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value = t.ss_item_sk + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value = t.ss_customer_sk + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value = t.ss_store_sk + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct StoreSalesTPCDS19Equal {
  inline static bool equal(const StoreSalesTPCDS19 &lhs, const StoreSalesTPCDS19 &rhs) {
    return lhs.ss_sold_date_sk == rhs.ss_sold_date_sk
           && lhs.ss_item_sk == rhs.ss_item_sk
           && lhs.ss_customer_sk == rhs.ss_customer_sk
           && lhs.ss_store_sk == rhs.ss_store_sk
           && lhs.ss_ext_sales_price == rhs.ss_ext_sales_price;
  }
};

struct StoreSalesTPCDS19Hash0 {
  inline static size_t hash(const StoreSalesTPCDS19 &t) {
    return t.ss_sold_date_sk + 0x9e3779b9;
  }
};

struct StoreSalesTPCDS19Equal0 {
  inline static bool equal(const StoreSalesTPCDS19 &lhs, const StoreSalesTPCDS19 &rhs) {
    return lhs.ss_sold_date_sk == rhs.ss_sold_date_sk;
  }
};

struct StoreSalesTPCDS19Hash1 {
  inline static size_t hash(const StoreSalesTPCDS19 &t) {
    return t.ss_item_sk + 0x9e3779b9;
  }
};

struct StoreSalesTPCDS19Equal1 {
  inline static bool equal(const StoreSalesTPCDS19 &lhs, const StoreSalesTPCDS19 &rhs) {
    return lhs.ss_item_sk == rhs.ss_item_sk;
  }
};

struct StoreSalesTPCDS19Hash2 {
  inline static size_t hash(const StoreSalesTPCDS19 &t) {
    return t.ss_customer_sk + 0x9e3779b9;
  }
};

struct StoreSalesTPCDS19Equal2 {
  inline static bool equal(const StoreSalesTPCDS19 &lhs, const StoreSalesTPCDS19 &rhs) {
    return lhs.ss_customer_sk == rhs.ss_customer_sk;
  }
};

struct StoreSalesTPCDS19Hash3 {
  inline static size_t hash(const StoreSalesTPCDS19 &t) {
    return t.ss_store_sk + 0x9e3779b9;
  }
};

struct StoreSalesTPCDS19Equal3 {
  inline static bool equal(const StoreSalesTPCDS19 &lhs, const StoreSalesTPCDS19 &rhs) {
    return lhs.ss_store_sk == rhs.ss_store_sk;
  }
};

struct StoreTPCDS19 {
  unsigned long s_store_sk;

  double annotation;
  double approximate;
  bool active;

  StoreTPCDS19 *previous;
  StoreTPCDS19 *next;

  StoreTPCDS19()
      : s_store_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  StoreTPCDS19(const StoreTPCDS19 &other)
      : s_store_sk(other.s_store_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct StoreTPCDS19Hash {
  inline static size_t hash(const StoreTPCDS19 &t) { return t.s_store_sk + 0x9e3779b9; }
};

struct StoreTPCDS19Equal {
  inline static bool equal(const StoreTPCDS19 &lhs, const StoreTPCDS19 &rhs) {
    return lhs.s_store_sk == rhs.s_store_sk;
  }
};

struct CustomerTPCDS19 {
  unsigned long c_customer_sk;
  unsigned long c_current_addr_sk;

  double annotation;
  double approximate;
  bool active;

  CustomerTPCDS19 *previous;
  CustomerTPCDS19 *next;

  CustomerTPCDS19()
      : c_customer_sk(0), c_current_addr_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerTPCDS19(const CustomerTPCDS19 &other)
      : c_customer_sk(other.c_customer_sk), c_current_addr_sk(other.c_current_addr_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerTPCDS19Hash {
  inline static size_t hash(const CustomerTPCDS19 &t) { return t.c_customer_sk + 0x9e3779b9; }
};

struct CustomerTPCDS19Equal {
  inline static bool equal(const CustomerTPCDS19 &lhs, const CustomerTPCDS19 &rhs) {
    return lhs.c_customer_sk == rhs.c_customer_sk;
  }
};

struct CustomerTPCDS19Hash1 {
  inline static size_t hash(const CustomerTPCDS19 &t) { return t.c_current_addr_sk + 0x9e3779b9; }
};

struct CustomerTPCDS19Equal1 {
  inline static bool equal(const CustomerTPCDS19 &lhs, const CustomerTPCDS19 &rhs) {
    return lhs.c_current_addr_sk == rhs.c_current_addr_sk;
  }
};

struct CustomerJoinKeyTPCDS19 {
  unsigned long c_customer_sk;

  double annotation;
  double approximate;
  bool active;

  CustomerJoinKeyTPCDS19 *previous;
  CustomerJoinKeyTPCDS19 *next;

  CustomerJoinKeyTPCDS19()
      : c_customer_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerJoinKeyTPCDS19(const CustomerJoinKeyTPCDS19 &other)
      : c_customer_sk(other.c_customer_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerJoinKeyTPCDS19Hash {
  inline static size_t hash(const CustomerJoinKeyTPCDS19 &t) { return t.c_customer_sk + 0x9e3779b9; }
};

struct CustomerJoinKeyTPCDS19Equal {
  inline static bool equal(const CustomerJoinKeyTPCDS19 &lhs, const CustomerJoinKeyTPCDS19 &rhs) {
    return lhs.c_customer_sk == rhs.c_customer_sk;
  }
};

struct CustomerAddressTPCDS19 {
  unsigned long ca_address_sk;

  double annotation;
  double approximate;
  bool active;

  CustomerAddressTPCDS19 *previous;
  CustomerAddressTPCDS19 *next;

  CustomerAddressTPCDS19()
      : ca_address_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerAddressTPCDS19(const CustomerAddressTPCDS19 &other)
      : ca_address_sk(other.ca_address_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerAddressTPCDS19Hash {
  inline static size_t hash(const CustomerAddressTPCDS19 &t) { return t.ca_address_sk + 0x9e3779b9; }
};

struct CustomerAddressTPCDS19Equal {
  inline static bool equal(const CustomerAddressTPCDS19 &lhs, const CustomerAddressTPCDS19 &rhs) {
    return lhs.ca_address_sk == rhs.ca_address_sk;
  }
};

struct DateDimTPCDS19 {
  unsigned long d_date_sk;

  double annotation;
  double approximate;
  bool active;

  DateDimTPCDS19 *previous;
  DateDimTPCDS19 *next;

  DateDimTPCDS19()
      : d_date_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  DateDimTPCDS19(const DateDimTPCDS19 &other)
      : d_date_sk(other.d_date_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct DateDimTPCDS19Hash {
  inline static size_t hash(const DateDimTPCDS19 &t) { return t.d_date_sk + 0x9e3779b9; }
};

struct DateDimTPCDS19Equal {
  inline static bool equal(const DateDimTPCDS19 &lhs, const DateDimTPCDS19 &rhs) {
    return lhs.d_date_sk == rhs.d_date_sk;
  }
};

struct ItemTPCDS19 {
  unsigned long i_item_sk;
  unsigned long i_brand_id;
  std::string i_brand;
  unsigned long i_manufact_id;
  std::string i_manufact;

  double annotation;
  double approximate;
  bool active;

  ItemTPCDS19 *previous;
  ItemTPCDS19 *next;

  ItemTPCDS19()
      : i_item_sk(0), i_brand_id(0), i_brand(""), i_manufact_id(0), i_manufact(""),
        annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  ItemTPCDS19(const ItemTPCDS19 &other)
      : i_item_sk(other.i_item_sk), i_brand_id(other.i_brand_id), i_brand(other.i_brand),
        i_manufact_id(other.i_manufact_id), i_manufact(other.i_manufact), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct ItemTPCDS19Hash {
  inline static size_t hash(const ItemTPCDS19 &t) { return t.i_item_sk + 0x9e3779b9; }
};

struct ItemTPCDS19Equal {
  inline static bool equal(const ItemTPCDS19 &lhs, const ItemTPCDS19 &rhs) {
    return lhs.i_item_sk == rhs.i_item_sk;
  }
};

struct GStoreSalesTPCDS19 {
  unsigned long i_brand_id;
  std::string i_brand;
  unsigned long i_manufact_id;
  std::string i_manufact;

  double annotation;
  double approximate;
  bool active;

  GStoreSalesTPCDS19 *previous;
  GStoreSalesTPCDS19 *next;

  GStoreSalesTPCDS19()
      : i_brand_id(0), i_brand(""), i_manufact_id(0), i_manufact(""),
        annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  GStoreSalesTPCDS19(const GStoreSalesTPCDS19 &other)
      : i_brand_id(other.i_brand_id), i_brand(other.i_brand),
        i_manufact_id(other.i_manufact_id), i_manufact(other.i_manufact), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct GStoreSalesTPCDS19Hash {
  inline static size_t hash(const GStoreSalesTPCDS19 &t) {
    std::hash<std::string> hash;
    unsigned long hash_value = t.i_brand_id + 0x9e3779b9;
    hash_value = hash(t.i_brand) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value = t.i_manufact_id + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value = hash(t.i_manufact) + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct GStoreSalesTPCDS19Equal {
  inline static bool equal(const GStoreSalesTPCDS19 &lhs, const GStoreSalesTPCDS19 &rhs) {
    return lhs.i_brand_id == rhs.i_brand_id
           && lhs.i_brand == rhs.i_brand
           && lhs.i_manufact_id == rhs.i_manufact_id
           && lhs.i_manufact == rhs.i_manufact;
  }
};

#endif
