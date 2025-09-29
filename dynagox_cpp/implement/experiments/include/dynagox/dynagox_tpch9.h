#ifndef DYNAGOX_EXPERIMENTS_DYNAGOX_TPCH9_H
#define DYNAGOX_EXPERIMENTS_DYNAGOX_TPCH9_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpch9_tuples.h"
#include <sstream>

class DynaGoxTPCH9 : public DynaGoxBase {
private:
  inline void insert_into_part(unsigned long p_partkey, const std::string &p_name);

  inline void insert_into_supplier(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void insert_into_partsupp(unsigned long ps_partkey, unsigned long ps_suppkey, double ps_supplycost);

  inline void insert_into_orders(unsigned long o_orderkey, unsigned long o_year);

  inline void insert_into_lineitem(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                   unsigned long l_linenumber, double l_quantity, double l_extendedprice,
                                   double l_discount);

  inline void insert_into_nation(unsigned long n_nationkey, const std::string &n_name);

  inline void print(const std::string &n_name, unsigned long o_year, double value1, double value2);

  inline void update_activate_r_p(unsigned long p_partkey);

  inline void update_activate_r_s(unsigned long s_suppkey, unsigned long s_nationkey);

  inline void update_activate_apv_s(unsigned long s_suppkey, const std::string &n_name);

  inline void update_activate_r_ps(unsigned long ps_partkey, unsigned long ps_suppkey, double ps_supplycost);

  inline void update_activate_r_o(unsigned long o_orderkey, unsigned long o_year);

  inline void update_activate_r_l(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                  unsigned long l_linenumber, double l_quantity, double l_extendedprice,
                                  double l_discount);

  inline void update_activate_r_n(unsigned long n_nationkey, const std::string &n_name);

  inline void update_add_hv_gl1(const std::string &n_name, unsigned long o_year, double value);

  inline void update_add_hv_gl2(const std::string &n_name, unsigned long o_year, double value);

  bool delta_mode;

  PartTPCH9 t_p;
  SupplierTPCH9 t_s;
  SupplierJoinKeyTPCH9 k_s;
  PartSuppTPCH9 t_ps;
  OrdersTPCH9 t_o;
  LineitemTPCH9 t_l;
  NationTPCH9 t_n;
  GLineitemTPCH9 t_gl;

  MultiHashMap<PartTPCH9, PrimaryHashIndex<PartTPCH9, PartTPCH9Hash, PartTPCH9Equal>> r_p;

  MultiHashMap<SupplierTPCH9, PrimaryHashIndex<SupplierTPCH9, SupplierTPCH9Hash0, SupplierTPCH9Equal0>,
      SecondaryHashIndex<SupplierTPCH9, SupplierTPCH9Hash1, SupplierTPCH9Equal1>> r_s;
  MultiHashMap<SupplierJoinKeyTPCH9, PrimaryHashIndex<SupplierJoinKeyTPCH9, SupplierJoinKeyTPCH9Hash,
      SupplierJoinKeyTPCH9Equal>> apv_s;

  MultiHashMap<PartSuppTPCH9, PrimaryHashIndex<PartSuppTPCH9, PartSuppTPCH9Hash, PartSuppTPCH9Equal>> r_ps;

  MultiHashMap<OrdersTPCH9, PrimaryHashIndex<OrdersTPCH9, OrdersTPCH9Hash, OrdersTPCH9Equal>> r_o;

  MultiHashMap<LineitemTPCH9, PrimaryHashIndex<LineitemTPCH9, LineitemTPCH9Hash, LineitemTPCH9Equal>,
      SecondaryHashIndex<LineitemTPCH9, LineitemTPCH9Hash0, LineitemTPCH9Equal0>,
      SecondaryHashIndex<LineitemTPCH9, LineitemTPCH9Hash1, LineitemTPCH9Equal1>,
      SecondaryHashIndex<LineitemTPCH9, LineitemTPCH9Hash2, LineitemTPCH9Equal2>,
      SecondaryHashIndex<LineitemTPCH9, LineitemTPCH9Hash12, LineitemTPCH9Equal12>> r_l;

  MultiHashMap<NationTPCH9, PrimaryHashIndex<NationTPCH9, NationTPCH9Hash, NationTPCH9Equal>> r_n;

  MultiHashMap<GLineitemTPCH9, PrimaryHashIndex<GLineitemTPCH9, GLineitemTPCH9Hash, GLineitemTPCH9Equal>> hv_gl1;

  MultiHashMap<GLineitemTPCH9, PrimaryHashIndex<GLineitemTPCH9, GLineitemTPCH9Hash, GLineitemTPCH9Equal>> hv_gl2;

public:
  explicit DynaGoxTPCH9(std::ostream &output, bool print_result, double b, bool delta_mode)
      : DynaGoxBase(output, print_result, b), delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};


#endif
