#ifndef DYNAGOX_CROWN_TPCH10_H
#define DYNAGOX_CROWN_TPCH10_H

#include "crown_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tpch10_tuples.h"
#include <sstream>

class CrownTPCH10 : public CrownBase {
private:
  inline void insert_into_customer(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                   unsigned long c_nationkey, const std::string &c_phone, double c_acctbal,
                                   const std::string &c_comment);

  inline void insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate);

  inline void insert_into_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                   const std::string &l_returnflag);

  inline void insert_into_nation(unsigned long n_nationkey, const std::string &n_name);

  inline void delete_from_customer(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                   unsigned long c_nationkey, const std::string &c_phone, double c_acctbal,
                                   const std::string &c_comment);

  inline void delete_from_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate);

  inline void delete_from_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                   const std::string &l_returnflag);

  inline void delete_from_nation(unsigned long n_nationkey, const std::string &n_name);

  inline void print(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                    const std::string &c_phone, double c_acctbal, const std::string &c_comment,
                    const std::string &n_name, double value);

  inline void update_activate_r_c(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                  unsigned long c_nationkey, const std::string &c_phone, double c_acctbal,
                                  const std::string &c_comment);

  inline void update_remove_r_c(unsigned long c_custkey);

  inline void update_activate_pv_c(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                   const std::string &c_phone, double c_acctbal, const std::string &c_comment,
                                   const std::string &n_name);

  inline void update_remove_pv_c(unsigned long c_custkey);

  inline void update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey);

  inline void update_remove_r_o(unsigned long o_orderkey);

  inline void update_activate_pv_o(unsigned long o_orderkey, unsigned long c_custkey, const std::string &c_name,
                                   const std::string &c_address, const std::string &c_phone, double c_acctbal,
                                   const std::string &c_comment, const std::string &n_name);

  inline void update_remove_pv_o(unsigned long o_orderkey);

  inline void update_add_pv_l(unsigned long l_orderkey, double value);

  inline void update_subtract_pv_l(unsigned long l_orderkey, double value);

  inline void update_activate_r_n(unsigned long n_nationkey, const std::string &n_name);

  inline void update_remove_r_n(unsigned long n_nationkey);

  inline void update_add_sv_go(unsigned long c_custkey, const std::string &c_name,
                               const std::string &c_address, const std::string &c_phone, double c_acctbal,
                               const std::string &c_comment, const std::string &n_name, double value);

  inline void update_subtract_sv_go(unsigned long c_custkey, double value);

  bool delta_mode;

  CustomerTPCH10 t_c;
  CustomerJoinKeyTPCH10 k_c;
  OrdersTPCH10 t_o;
  OrdersJoinKeyTPCH10 k_o;
  LineitemJoinKeyTPCH10 k_l;
  NationTPCH10 t_n;
  GOrdersTPCH10 t_go;

  MultiHashMap<CustomerTPCH10, PrimaryHashIndex<CustomerTPCH10, CustomerTPCH10Hash0, CustomerTPCH10Equal0>,
      SecondaryHashIndex<CustomerTPCH10, CustomerTPCH10Hash1, CustomerTPCH10Equal1>> r_c;
  MultiHashMap<CustomerJoinKeyTPCH10, PrimaryHashIndex<CustomerJoinKeyTPCH10, CustomerJoinKeyTPCH10Hash,
      CustomerJoinKeyTPCH10Equal>> pv_c;

  MultiHashMap<OrdersTPCH10, PrimaryHashIndex<OrdersTPCH10, OrdersTPCH10Hash0, OrdersTPCH10Equal0>,
      SecondaryHashIndex<OrdersTPCH10, OrdersTPCH10Hash1, OrdersTPCH10Equal1>> r_o;
  MultiHashMap<OrdersJoinKeyTPCH10, PrimaryHashIndex<OrdersJoinKeyTPCH10, OrdersJoinKeyTPCH10Hash,
      OrdersJoinKeyTPCH10Equal>> pv_o;

  MultiHashMap<LineitemJoinKeyTPCH10, PrimaryHashIndex<LineitemJoinKeyTPCH10, LineitemJoinKeyTPCH10Hash,
      LineitemJoinKeyTPCH10Equal>> pv_l;

  MultiHashMap<NationTPCH10, PrimaryHashIndex<NationTPCH10, NationTPCH10Hash, NationTPCH10Equal>> r_n;

  MultiHashMap<GOrdersTPCH10, PrimaryHashIndex<GOrdersTPCH10, GOrdersTPCH10Hash, GOrdersTPCH10Equal>> sv_go;

public:
  explicit CrownTPCH10(std::ostream &output, bool print_result, bool delta_mode) : CrownBase(output, print_result),
                                                                                   delta_mode(delta_mode) {}

  void process(const std::string &line) override;

  void milestone() override;
};

#endif
