#ifndef DYNAGOX_DYNAGOX_TPCH5_H
#define DYNAGOX_DYNAGOX_TPCH5_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpch5_tuples.h"
#include <sstream>

class DynaGoxTPCH5 : public DynaGoxBase {
private:
  inline void insert_into_supplier(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void insert_into_customer(unsigned long c_custkey, unsigned long c_nationkey);

  inline void insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate);

  inline void insert_into_lineitem(unsigned long l_orderkey, unsigned long l_suppkey, unsigned long l_linenumber,
                                   double l_extendedprice, double l_discount);

  inline void insert_into_nation(unsigned long n_nationkey, const std::string &n_name, unsigned long n_regionkey);

  inline void insert_into_region(unsigned long r_regionkey, std::string &r_name);

  inline void print(const std::string &n_name, double value);

  inline void update_activate_r_s(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void update_activate_apv_s(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void update_activate_r_c(unsigned long c_custkey, unsigned long c_nationkey);

  inline void update_activate_apv_c(unsigned long c_custkey, unsigned long c_nationkey, const std::string &n_name);

  inline void update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey);

  inline void update_activate_apv_o(unsigned long o_orderkey, unsigned long c_nationkey, const std::string &n_name);

  inline void update_activate_r_l(unsigned long l_orderkey, unsigned long l_suppkey, unsigned long l_linenumber,
                                  double l_extendedprice, double l_discount);

  inline void update_activate_r_n(unsigned long n_nationkey, const std::string &n_name, unsigned long n_regionkey);

  inline void update_activate_apv_n(unsigned long n_nationkey, const std::string &n_name);

  inline void update_activate_r_r(unsigned long r_regionkey);

  inline void update_add_hv_gl(const std::string &n_name, double value);

  bool delta_mode;

  SupplierTPCH5 t_s;
  SupplierJoinKeyTPCH5 k_s;
  CustomerTPCH5 t_c;
  CustomerJoinKeyTPCH5 k_c;
  OrdersTPCH5 t_o;
  OrdersJoinKeyTPCH5 k_o;
  LineitemTPCH5 t_l;
  NationTPCH5 t_n;
  NationJoinKeyTPCH5 k_n;
  RegionTPCH5 t_r;
  GLineitemTPCH5 t_gl;

  MultiHashMap<SupplierTPCH5, PrimaryHashIndex<SupplierTPCH5, SupplierTPCH5Hash0, SupplierTPCH5Equal0>,
      SecondaryHashIndex<SupplierTPCH5, SupplierTPCH5Hash1, SupplierTPCH5Equal1>> r_s;
  MultiHashMap<SupplierJoinKeyTPCH5, PrimaryHashIndex<SupplierJoinKeyTPCH5, SupplierJoinKeyTPCH5Hash,
      SupplierJoinKeyTPCH5Equal>> apv_s;

  MultiHashMap<CustomerTPCH5, PrimaryHashIndex<CustomerTPCH5, CustomerTPCH5Hash0, CustomerTPCH5Equal0>,
      SecondaryHashIndex<CustomerTPCH5, CustomerTPCH5Hash1, CustomerTPCH5Equal1>> r_c;
  MultiHashMap<CustomerJoinKeyTPCH5, PrimaryHashIndex<CustomerJoinKeyTPCH5, CustomerJoinKeyTPCH5Hash,
      CustomerJoinKeyTPCH5Equal>> apv_c;

  MultiHashMap<OrdersTPCH5, PrimaryHashIndex<OrdersTPCH5, OrdersTPCH5Hash0, OrdersTPCH5Equal0>,
      SecondaryHashIndex<OrdersTPCH5, OrdersTPCH5Hash1, OrdersTPCH5Equal1>> r_o;
  MultiHashMap<OrdersJoinKeyTPCH5, PrimaryHashIndex<OrdersJoinKeyTPCH5, OrdersJoinKeyTPCH5Hash,
      OrdersJoinKeyTPCH5Equal>> apv_o;

  MultiHashMap<LineitemTPCH5, PrimaryHashIndex<LineitemTPCH5, LineitemTPCH5Hash, LineitemTPCH5Equal>,
      SecondaryHashIndex<LineitemTPCH5, LineitemTPCH5Hash0, LineitemTPCH5Equal0>,
      SecondaryHashIndex<LineitemTPCH5, LineitemTPCH5Hash1, LineitemTPCH5Equal1>> r_l;

  MultiHashMap<NationTPCH5, PrimaryHashIndex<NationTPCH5, NationTPCH5Hash0, NationTPCH5Equal0>,
      SecondaryHashIndex<NationTPCH5, NationTPCH5Hash2, NationTPCH5Equal2>> r_n;
  MultiHashMap<NationJoinKeyTPCH5, PrimaryHashIndex<NationJoinKeyTPCH5, NationJoinKeyTPCH5Hash,
      NationJoinKeyTPCH5Equal>> apv_n;

  MultiHashMap<RegionTPCH5, PrimaryHashIndex<RegionTPCH5, RegionTPCH5Hash, RegionTPCH5Equal>> r_r;

  MultiHashMap<GLineitemTPCH5, PrimaryHashIndex<GLineitemTPCH5, GLineitemTPCH5Hash, GLineitemTPCH5Equal>> hv_gl;

public:
  explicit DynaGoxTPCH5(std::ostream &output, bool print_result, double b, bool delta_mode)
      : DynaGoxBase(output, print_result, b), delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};

#endif
