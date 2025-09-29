#ifndef DYNAGOX_CROWN_TPCDS19_H
#define DYNAGOX_CROWN_TPCDS19_H

#include "crown_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpcds19_tuples.h"
#include <sstream>

class CrownTPCDS19 : public CrownBase {
private:
  inline void insert_into_store_sales(unsigned long ss_sold_date_sk, unsigned long ss_item_sk,
                                      unsigned long ss_customer_sk, unsigned long ss_store_sk, double ss_ext_sales_price);

  inline void insert_into_store(unsigned long s_store_sk);

  inline void insert_into_customer(unsigned long c_customer_sk, unsigned long c_current_addr_sk);

  inline void insert_into_customer_address(unsigned long ca_address_sk);

  inline void insert_into_date_dim(unsigned long d_date_sk);

  inline void insert_into_item(unsigned long i_item_sk, unsigned long i_brand_id, const std::string &i_brand,
                               unsigned long i_manufact_id, const std::string &i_manufact);

  inline void print(unsigned long i_brand_id, const std::string &i_brand,
                    unsigned long i_manufact_id, const std::string &i_manufact, double value);

  bool delta_mode;

  StoreSalesTPCDS19 t_ss;
  StoreTPCDS19 t_s;
  CustomerTPCDS19 t_c;
  CustomerJoinKeyTPCDS19 k_c;
  CustomerAddressTPCDS19 t_ca;
  DateDimTPCDS19 t_d;
  ItemTPCDS19 t_i;
  GStoreSalesTPCDS19 t_gss;

  MultiHashMap<StoreSalesTPCDS19, PrimaryHashIndex<StoreSalesTPCDS19, StoreSalesTPCDS19Hash, StoreSalesTPCDS19Equal>,
      SecondaryHashIndex<StoreSalesTPCDS19, StoreSalesTPCDS19Hash0, StoreSalesTPCDS19Equal0>,
      SecondaryHashIndex<StoreSalesTPCDS19, StoreSalesTPCDS19Hash1, StoreSalesTPCDS19Equal1>,
      SecondaryHashIndex<StoreSalesTPCDS19, StoreSalesTPCDS19Hash2, StoreSalesTPCDS19Equal2>,
      SecondaryHashIndex<StoreSalesTPCDS19, StoreSalesTPCDS19Hash3, StoreSalesTPCDS19Equal3>> r_ss;
  
  MultiHashMap<StoreTPCDS19, PrimaryHashIndex<StoreTPCDS19, StoreTPCDS19Hash, StoreTPCDS19Equal>> r_s;

  MultiHashMap<CustomerTPCDS19, PrimaryHashIndex<CustomerTPCDS19, CustomerTPCDS19Hash, CustomerTPCDS19Equal>,
      SecondaryHashIndex<CustomerTPCDS19, CustomerTPCDS19Hash1, CustomerTPCDS19Equal1>> r_c;
  MultiHashMap<CustomerJoinKeyTPCDS19, PrimaryHashIndex<CustomerJoinKeyTPCDS19, CustomerJoinKeyTPCDS19Hash,
      CustomerJoinKeyTPCDS19Equal>> pv_c;

  MultiHashMap<CustomerAddressTPCDS19, PrimaryHashIndex<CustomerAddressTPCDS19, CustomerAddressTPCDS19Hash,
      CustomerAddressTPCDS19Equal>> r_ca;

  MultiHashMap<DateDimTPCDS19, PrimaryHashIndex<DateDimTPCDS19, DateDimTPCDS19Hash, DateDimTPCDS19Equal>> r_d;

  MultiHashMap<ItemTPCDS19, PrimaryHashIndex<ItemTPCDS19, ItemTPCDS19Hash, ItemTPCDS19Equal>> r_i;

  MultiHashMap<GStoreSalesTPCDS19, PrimaryHashIndex<GStoreSalesTPCDS19, GStoreSalesTPCDS19Hash, GStoreSalesTPCDS19Equal>> sv_gss;

public:
  explicit CrownTPCDS19(std::ostream &output, bool print_result, bool delta_mode) : CrownBase(output, print_result),
                                                                                    delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};

#endif
