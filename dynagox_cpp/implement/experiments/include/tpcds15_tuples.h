#ifndef DYNAGOX_TPCDS15_TUPLES_H
#define DYNAGOX_TPCDS15_TUPLES_H

#include <cstddef>
#include <string>

struct CatalogSalesTPCDS15 {
  unsigned long cs_sold_date_sk;
  unsigned long cs_bill_customer_sk;
  double cs_sales_price;

  double annotation;
  double approximate;
  bool active;

  CatalogSalesTPCDS15 *previous;
  CatalogSalesTPCDS15 *next;

  CatalogSalesTPCDS15()
      : cs_sold_date_sk(0), cs_bill_customer_sk(0), cs_sales_price(0),
        annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CatalogSalesTPCDS15(const CatalogSalesTPCDS15 &other)
      : cs_sold_date_sk(other.cs_sold_date_sk), cs_bill_customer_sk(other.cs_bill_customer_sk),
        cs_sales_price(other.cs_sales_price), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CatalogSalesTPCDS15Hash {
  inline static size_t hash(const CatalogSalesTPCDS15 &t) {
    std::hash<double> hash;
    auto hash_value = hash(t.cs_sales_price);
    hash_value = t.cs_sold_date_sk + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    hash_value = t.cs_bill_customer_sk + 0x9e3779b9 + (hash_value << 6) + (hash_value >> 2);
    return hash_value;
  }
};

struct CatalogSalesTPCDS15Equal {
  inline static bool equal(const CatalogSalesTPCDS15 &lhs, const CatalogSalesTPCDS15 &rhs) {
    return lhs.cs_sold_date_sk == rhs.cs_sold_date_sk
           && lhs.cs_bill_customer_sk == rhs.cs_bill_customer_sk
           && lhs.cs_sales_price == rhs.cs_sales_price;
  }
};

struct CatalogSalesTPCDS15Hash0 {
  inline static size_t hash(const CatalogSalesTPCDS15 &t) {
    return t.cs_sold_date_sk + 0x9e3779b9;
  }
};

struct CatalogSalesTPCDS15Equal0 {
  inline static bool equal(const CatalogSalesTPCDS15 &lhs, const CatalogSalesTPCDS15 &rhs) {
    return lhs.cs_sold_date_sk == rhs.cs_sold_date_sk;
  }
};

struct CatalogSalesJoinKeyTPCDS15 {
  unsigned long cs_bill_customer_sk;

  double annotation;
  double approximate;
  bool active;

  CatalogSalesJoinKeyTPCDS15 *previous;
  CatalogSalesJoinKeyTPCDS15 *next;

  CatalogSalesJoinKeyTPCDS15() : cs_bill_customer_sk(0),
                                 annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  CatalogSalesJoinKeyTPCDS15(const CatalogSalesJoinKeyTPCDS15 &other) : cs_bill_customer_sk(other.cs_bill_customer_sk),
                                                                        annotation(other.annotation),
                                                                        approximate(other.approximate),
                                                                        active(false),
                                                                        previous(nullptr),
                                                                        next(nullptr) {}
};

struct CatalogSalesJoinKeyTPCDS15Hash {
  inline static size_t hash(const CatalogSalesJoinKeyTPCDS15 &t) { return t.cs_bill_customer_sk + 0x9e3779b9; }
};

struct CatalogSalesJoinKeyTPCDS15Equal {
  inline static bool equal(const CatalogSalesJoinKeyTPCDS15 &lhs, const CatalogSalesJoinKeyTPCDS15 &rhs) {
    return lhs.cs_bill_customer_sk == rhs.cs_bill_customer_sk;
  }
};

struct CustomerTPCDS15 {
  unsigned long c_customer_sk;
  unsigned long c_current_addr_sk;

  double annotation;
  double approximate;
  bool active;

  CustomerTPCDS15 *previous;
  CustomerTPCDS15 *next;

  CustomerTPCDS15()
      : c_customer_sk(0), c_current_addr_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerTPCDS15(const CustomerTPCDS15 &other)
      : c_customer_sk(other.c_customer_sk), c_current_addr_sk(other.c_current_addr_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerTPCDS15Hash {
  inline static size_t hash(const CustomerTPCDS15 &t) { return t.c_customer_sk + 0x9e3779b9; }
};

struct CustomerTPCDS15Equal {
  inline static bool equal(const CustomerTPCDS15 &lhs, const CustomerTPCDS15 &rhs) {
    return lhs.c_customer_sk == rhs.c_customer_sk;
  }
};

struct CustomerTPCDS15Hash1 {
  inline static size_t hash(const CustomerTPCDS15 &t) { return t.c_current_addr_sk + 0x9e3779b9; }
};

struct CustomerTPCDS15Equal1 {
  inline static bool equal(const CustomerTPCDS15 &lhs, const CustomerTPCDS15 &rhs) {
    return lhs.c_current_addr_sk == rhs.c_current_addr_sk;
  }
};

struct CustomerJoinKeyTPCDS15 {
  unsigned long c_customer_sk;
  std::string ca_zip;

  double annotation;
  double approximate;
  bool active;

  CustomerJoinKeyTPCDS15 *previous;
  CustomerJoinKeyTPCDS15 *next;

  CustomerJoinKeyTPCDS15()
      : c_customer_sk(0), ca_zip(""), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerJoinKeyTPCDS15(const CustomerJoinKeyTPCDS15 &other)
      : c_customer_sk(other.c_customer_sk), ca_zip(other.ca_zip), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerJoinKeyTPCDS15Hash {
  inline static size_t hash(const CustomerJoinKeyTPCDS15 &t) { return t.c_customer_sk + 0x9e3779b9; }
};

struct CustomerJoinKeyTPCDS15Equal {
  inline static bool equal(const CustomerJoinKeyTPCDS15 &lhs, const CustomerJoinKeyTPCDS15 &rhs) {
    return lhs.c_customer_sk == rhs.c_customer_sk;
  }
};

struct CustomerAddressTPCDS15 {
  unsigned long ca_address_sk;
  std::string ca_zip;

  double annotation;
  double approximate;
  bool active;

  CustomerAddressTPCDS15 *previous;
  CustomerAddressTPCDS15 *next;

  CustomerAddressTPCDS15()
      : ca_address_sk(0), ca_zip(""), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  CustomerAddressTPCDS15(const CustomerAddressTPCDS15 &other)
      : ca_address_sk(other.ca_address_sk), ca_zip(other.ca_zip), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct CustomerAddressTPCDS15Hash {
  inline static size_t hash(const CustomerAddressTPCDS15 &t) { return t.ca_address_sk + 0x9e3779b9; }
};

struct CustomerAddressTPCDS15Equal {
  inline static bool equal(const CustomerAddressTPCDS15 &lhs, const CustomerAddressTPCDS15 &rhs) {
    return lhs.ca_address_sk == rhs.ca_address_sk;
  }
};

struct DateDimTPCDS15 {
  unsigned long d_date_sk;

  double annotation;
  double approximate;
  bool active;

  DateDimTPCDS15 *previous;
  DateDimTPCDS15 *next;

  DateDimTPCDS15()
      : d_date_sk(0), annotation(0), approximate(0), active(false), previous(nullptr),
        next(nullptr) {}

  DateDimTPCDS15(const DateDimTPCDS15 &other)
      : d_date_sk(other.d_date_sk), annotation(other.annotation),
        approximate(other.approximate), active(false), previous(nullptr),
        next(nullptr) {}
};

struct DateDimTPCDS15Hash {
  inline static size_t hash(const DateDimTPCDS15 &t) { return t.d_date_sk + 0x9e3779b9; }
};

struct DateDimTPCDS15Equal {
  inline static bool equal(const DateDimTPCDS15 &lhs, const DateDimTPCDS15 &rhs) {
    return lhs.d_date_sk == rhs.d_date_sk;
  }
};

struct GCustomerTPCDS15 {
  std::string ca_zip;

  double annotation;
  double approximate;
  bool active;

  GCustomerTPCDS15 *previous;
  GCustomerTPCDS15 *next;

  GCustomerTPCDS15() : ca_zip(""), annotation(0), approximate(0), active(false), previous(nullptr), next(nullptr) {}

  GCustomerTPCDS15(const GCustomerTPCDS15 &other) : ca_zip(other.ca_zip),
                                                    annotation(other.annotation), approximate(other.approximate),
                                                    active(false),
                                                    previous(nullptr),
                                                    next(nullptr) {}
};

struct GCustomerTPCDS15Hash {
  inline static size_t hash(const GCustomerTPCDS15 &t) {
    std::hash<std::string> hash;
    auto hash_value = hash(t.ca_zip);
    return hash_value;
  }
};

struct GCustomerTPCDS15Equal {
  inline static bool equal(const GCustomerTPCDS15 &lhs, const GCustomerTPCDS15 &rhs) {
    return lhs.ca_zip == rhs.ca_zip;
  }
};

#endif
