#ifndef DYNAGOX_DYNAGOX_TPCDS15_H
#define DYNAGOX_DYNAGOX_TPCDS15_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpcds15_tuples.h"
#include <sstream>

class DynaGoxTPCDS15 : public DynaGoxBase {
private:
  inline void
  insert_into_catalog_sales(unsigned long cs_sold_date_sk, unsigned long cs_bill_customer_sk, double cs_sales_price);

  inline void insert_into_customer(unsigned long c_customer_sk, unsigned long c_current_addr_sk);

  inline void insert_into_customer_address(unsigned long ca_address_sk, const std::string &ca_zip);

  inline void insert_into_date_dim(unsigned long d_date_sk);

  inline void print(const std::string &ca_zip, double value);

  bool delta_mode;

  CatalogSalesTPCDS15 t_cs;
  CatalogSalesJoinKeyTPCDS15 k_cs;
  CustomerTPCDS15 t_c;
  CustomerJoinKeyTPCDS15 k_c;
  CustomerAddressTPCDS15 t_ca;
  DateDimTPCDS15 t_d;
  GCustomerTPCDS15 t_gc;

  MultiHashMap<CatalogSalesTPCDS15, PrimaryHashIndex<CatalogSalesTPCDS15, CatalogSalesTPCDS15Hash, CatalogSalesTPCDS15Equal>,
      SecondaryHashIndex<CatalogSalesTPCDS15, CatalogSalesTPCDS15Hash0, CatalogSalesTPCDS15Equal0>> r_cs;
  MultiHashMap<CatalogSalesJoinKeyTPCDS15, PrimaryHashIndex<CatalogSalesJoinKeyTPCDS15, CatalogSalesJoinKeyTPCDS15Hash,
      CatalogSalesJoinKeyTPCDS15Equal>> apv_cs;

  MultiHashMap<CustomerTPCDS15, PrimaryHashIndex<CustomerTPCDS15, CustomerTPCDS15Hash, CustomerTPCDS15Equal>,
      SecondaryHashIndex<CustomerTPCDS15, CustomerTPCDS15Hash1, CustomerTPCDS15Equal1>> r_c;
  MultiHashMap<CustomerJoinKeyTPCDS15, PrimaryHashIndex<CustomerJoinKeyTPCDS15, CustomerJoinKeyTPCDS15Hash,
      CustomerJoinKeyTPCDS15Equal>> apv_c;

  MultiHashMap<CustomerAddressTPCDS15, PrimaryHashIndex<CustomerAddressTPCDS15, CustomerAddressTPCDS15Hash,
      CustomerAddressTPCDS15Equal>> r_ca;

  MultiHashMap<DateDimTPCDS15, PrimaryHashIndex<DateDimTPCDS15, DateDimTPCDS15Hash, DateDimTPCDS15Equal>> r_d;

  MultiHashMap<GCustomerTPCDS15, PrimaryHashIndex<GCustomerTPCDS15, GCustomerTPCDS15Hash, GCustomerTPCDS15Equal>> hv_gc;

public:
  explicit DynaGoxTPCDS15(std::ostream &output, bool print_result, double b, bool delta_mode)
      : DynaGoxBase(output, print_result, b), delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};

#endif
