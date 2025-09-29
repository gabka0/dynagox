#ifndef DYNAGOX_CROWN_TPCH8_H
#define DYNAGOX_CROWN_TPCH8_H

#include "crown_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpch8_tuples.h"
#include <sstream>

class CrownTPCH8 : public CrownBase {
private:
  inline void insert_into_part(unsigned long p_partkey, const std::string &p_type);

  inline void insert_into_supplier(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void insert_into_customer(unsigned long c_custkey, unsigned long c_nationkey);

  inline void insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                 unsigned long o_year);

  inline void insert_into_lineitem(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                   unsigned long l_linenumber, double l_extendedprice, double l_discount);

  inline void insert_into_nation1(unsigned long n_nationkey, unsigned long n_regionkey);

  inline void insert_into_nation2(unsigned long n_nationkey, const std::string &n_name);

  inline void insert_into_region(unsigned long r_regionkey, std::string &r_name);

  inline void print(unsigned long o_year, double value1, double value2);

  inline void update_activate_r_p(unsigned long p_partkey);

  inline void update_activate_r_s(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void update_activate_pv_s(unsigned long s_suppkey, const std::string &n_name);

  inline void update_activate_r_c(unsigned long c_custkey, unsigned long c_nationkey);

  inline void update_activate_pv_c(unsigned long c_custkey);

  inline void update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_year);

  inline void update_activate_pv_o(unsigned long o_orderkey, unsigned long o_year);

  inline void update_activate_r_l(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                  unsigned long l_linenumber, double l_extendedprice, double l_discount);

  inline void update_add_pv_l1(unsigned long l_orderkey, double value);

  inline void update_add_pv_l2(unsigned long l_orderkey, double value);

  inline void update_activate_r_n1(unsigned long n_nationkey, unsigned long n_regionkey);

  inline void update_activate_pv_n1(unsigned long n_nationkey);

  inline void update_activate_r_n2(unsigned long n_nationkey, const std::string &n_name);

  inline void update_activate_r_r(unsigned long r_regionkey);

  inline void update_add_sv_go1(unsigned long o_year, double value);

  inline void update_add_sv_go2(unsigned long o_year, double value);

  bool delta_mode;

  PartTPCH8 t_p;
  SupplierTPCH8 t_s;
  SupplierJoinKeyTPCH8 k_s;
  CustomerTPCH8 t_c;
  CustomerJoinKeyTPCH8 k_c;
  OrdersTPCH8 t_o;
  OrdersJoinKeyTPCH8 k_o;
  LineitemTPCH8 t_l;
  LineitemJoinKeyTPCH8 k_l;
  Nation1TPCH8 t_n1;
  Nation1JoinKeyTPCH8 k_n1;
  Nation2TPCH8 t_n2;
  RegionTPCH8 t_r;
  GOrdersTPCH8 t_go;

  MultiHashMap<PartTPCH8, PrimaryHashIndex<PartTPCH8, PartTPCH8Hash, PartTPCH8Equal>> r_p;

  MultiHashMap<SupplierTPCH8, PrimaryHashIndex<SupplierTPCH8, SupplierTPCH8Hash0, SupplierTPCH8Equal0>,
      SecondaryHashIndex<SupplierTPCH8, SupplierTPCH8Hash1, SupplierTPCH8Equal1>> r_s;
  MultiHashMap<SupplierJoinKeyTPCH8, PrimaryHashIndex<SupplierJoinKeyTPCH8, SupplierJoinKeyTPCH8Hash,
      SupplierJoinKeyTPCH8Equal>> pv_s;

  MultiHashMap<CustomerTPCH8, PrimaryHashIndex<CustomerTPCH8, CustomerTPCH8Hash0, CustomerTPCH8Equal0>,
      SecondaryHashIndex<CustomerTPCH8, CustomerTPCH8Hash1, CustomerTPCH8Equal1>> r_c;
  MultiHashMap<CustomerJoinKeyTPCH8, PrimaryHashIndex<CustomerJoinKeyTPCH8, CustomerJoinKeyTPCH8Hash,
      CustomerJoinKeyTPCH8Equal>> pv_c;

  MultiHashMap<OrdersTPCH8, PrimaryHashIndex<OrdersTPCH8, OrdersTPCH8Hash0, OrdersTPCH8Equal0>,
      SecondaryHashIndex<OrdersTPCH8, OrdersTPCH8Hash1, OrdersTPCH8Equal1>> r_o;
  MultiHashMap<OrdersJoinKeyTPCH8, PrimaryHashIndex<OrdersJoinKeyTPCH8, OrdersJoinKeyTPCH8Hash,
      OrdersJoinKeyTPCH8Equal>> pv_o;

  MultiHashMap<LineitemTPCH8, PrimaryHashIndex<LineitemTPCH8, LineitemTPCH8Hash, LineitemTPCH8Equal>,
      SecondaryHashIndex<LineitemTPCH8, LineitemTPCH8Hash1, LineitemTPCH8Equal1>,
      SecondaryHashIndex<LineitemTPCH8, LineitemTPCH8Hash2, LineitemTPCH8Equal2>> r_l;
  MultiHashMap<LineitemJoinKeyTPCH8, PrimaryHashIndex<LineitemJoinKeyTPCH8, LineitemJoinKeyTPCH8Hash,
      LineitemJoinKeyTPCH8Equal>> pv_l1;
  MultiHashMap<LineitemJoinKeyTPCH8, PrimaryHashIndex<LineitemJoinKeyTPCH8, LineitemJoinKeyTPCH8Hash,
      LineitemJoinKeyTPCH8Equal>> pv_l2;

  MultiHashMap<Nation1TPCH8, PrimaryHashIndex<Nation1TPCH8, Nation1TPCH8Hash0, Nation1TPCH8Equal0>,
      SecondaryHashIndex<Nation1TPCH8, Nation1TPCH8Hash1, Nation1TPCH8Equal1>> r_n1;
  MultiHashMap<Nation1JoinKeyTPCH8, PrimaryHashIndex<Nation1JoinKeyTPCH8, Nation1JoinKeyTPCH8Hash,
      Nation1JoinKeyTPCH8Equal>> pv_n1;

  MultiHashMap<Nation2TPCH8, PrimaryHashIndex<Nation2TPCH8, Nation2TPCH8Hash, Nation2TPCH8Equal>> r_n2;

  MultiHashMap<RegionTPCH8, PrimaryHashIndex<RegionTPCH8, RegionTPCH8Hash, RegionTPCH8Equal>> r_r;

  MultiHashMap<GOrdersTPCH8, PrimaryHashIndex<GOrdersTPCH8, GOrdersTPCH8Hash, GOrdersTPCH8Equal>> sv_go1;

  MultiHashMap<GOrdersTPCH8, PrimaryHashIndex<GOrdersTPCH8, GOrdersTPCH8Hash, GOrdersTPCH8Equal>> sv_go2;

public:
  explicit CrownTPCH8(std::ostream &output, bool print_result, bool delta_mode) : CrownBase(output, print_result),
                                                                                  delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};


#endif
