#ifndef DYNAGOX_DYNAGOX_TPCH3_H
#define DYNAGOX_DYNAGOX_TPCH3_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpch3_tuples.h"
#include <sstream>

class DynaGoxTPCH3 : public DynaGoxBase {
private:
  inline void insert_into_customer(unsigned long c_custkey, const std::string &c_mktsegment);

  inline void insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                 unsigned long o_shippriority);

  inline void insert_into_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                   unsigned long l_shipdate);

  inline void delete_from_customer(unsigned long c_custkey, const std::string &c_mktsegment);

  inline void delete_from_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                 unsigned long o_shippriority);

  inline void delete_from_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                   unsigned long l_shipdate);

  inline void print(unsigned long o_orderkey, unsigned long o_orderdate, unsigned long o_shippriority, double value);

  inline void update_activate_r_c(unsigned long c_custkey);

  inline void update_remove_r_c(unsigned long c_custkey);

  inline void update_activate_apv_o(unsigned long o_orderkey, unsigned long o_orderdate, unsigned long o_shippriority);

  inline void update_remove_apv_o(unsigned long o_orderkey);

  inline void update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                  unsigned long o_shippriority);

  inline void update_remove_r_o(unsigned long o_orderkey);

  inline double update_add_approx_apv_l(unsigned long l_orderkey, double value);

  inline double update_subtract_approx_apv_l(unsigned long l_orderkey, double value);

  inline void update_add_hv_go(unsigned long o_orderkey, unsigned long o_orderdate, unsigned long o_shippriority,
                               double value);

  inline void update_subtract_hv_go(unsigned long o_orderkey, double value);

  bool delta_mode;

  CustomerTPCH3 t_c;
  LineitemJoinKeyTPCH3 k_l;
  OrdersTPCH3 t_o;
  OrdersJoinKeyTPCH3 k_o;
  GOrdersTPCH3 t_go;

  MultiHashMap<CustomerTPCH3, PrimaryHashIndex<CustomerTPCH3, CustomerTPCH3Hash, CustomerTPCH3Equal>> r_c;

  MultiHashMap<OrdersJoinKeyTPCH3, PrimaryHashIndex<OrdersJoinKeyTPCH3, OrdersJoinKeyTPCH3Hash, OrdersJoinKeyTPCH3Equal>> apv_o;
  MultiHashMap<OrdersTPCH3, PrimaryHashIndex<OrdersTPCH3, OrdersTPCH3Hash0, OrdersTPCH3Equal0>,
      SecondaryHashIndex<OrdersTPCH3, OrdersTPCH3Hash1, OrdersTPCH3Equal1>> r_o;

  MultiHashMap<LineitemJoinKeyTPCH3, PrimaryHashIndex<LineitemJoinKeyTPCH3, LineitemJoinKeyTPCH3Hash, LineitemJoinKeyTPCH3Equal>>
      apv_l;

  MultiHashMap<GOrdersTPCH3, PrimaryHashIndex<GOrdersTPCH3, GOrdersTPCH3Hash, GOrdersTPCH3Equal>> hv_go;

public:
  explicit DynaGoxTPCH3(std::ostream &output, bool print_result, double b, bool delta_mode)
      : DynaGoxBase(output, print_result, b), delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};

#endif
