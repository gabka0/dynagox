#include "dynagox_tpch3.h"

void DynaGoxTPCH3::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  bool is_insert = (field == "+");
  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 3) {
    getline(stream, field, '|');
    unsigned long c_custkey = std::stoul(field);
    getline(stream, field, '|');
    std::string c_name = field;
    getline(stream, field, '|');
    std::string c_address = field;
    getline(stream, field, '|');
    unsigned long c_nationkey = std::stoul(field);
    getline(stream, field, '|');
    std::string c_phone = field;
    getline(stream, field, '|');
    double c_acctbal = std::stod(field);
    getline(stream, field, '|');
    std::string c_mktsegment = field;
    getline(stream, field, '|');
    std::string c_comment = field;

    if (is_insert) {
      insert_into_customer(c_custkey, c_mktsegment);
    } else {
      delete_from_customer(c_custkey, c_mktsegment);
    }
  } else if (relation_id == 4) {
    getline(stream, field, '|');
    unsigned long o_orderkey = std::stoul(field);
    getline(stream, field, '|');
    unsigned long o_custkey = std::stoul(field);
    getline(stream, field, '|');
    std::string o_orderstatus = field;
    getline(stream, field, '|');
    double o_totalprice = std::stod(field);
    getline(stream, field, '|');
    unsigned long o_orderdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                std::stoul(field.substr(8, 10));
    getline(stream, field, '|');
    std::string o_orderpriority = field;
    getline(stream, field, '|');
    std::string o_clerk = field;
    getline(stream, field, '|');
    unsigned long o_shippriority = std::stoul(field);
    getline(stream, field, '|');
    std::string o_comment = field;

    if (is_insert) {
      insert_into_orders(o_orderkey, o_custkey, o_orderdate, o_shippriority);
    } else {
      delete_from_orders(o_orderkey, o_custkey, o_orderdate, o_shippriority);
    }
  } else if (relation_id == 5) {
    getline(stream, field, '|');
    unsigned long l_orderkey = std::stoul(field);
    getline(stream, field, '|');
    unsigned long l_partkey = std::stoul(field);
    getline(stream, field, '|');
    unsigned long l_suppkey = std::stoul(field);
    getline(stream, field, '|');
    unsigned long l_linenumber = std::stoul(field);
    getline(stream, field, '|');
    double l_quantity = std::stod(field);
    getline(stream, field, '|');
    double l_extendedprice = std::stod(field);
    getline(stream, field, '|');
    double l_discount = std::stod(field);
    getline(stream, field, '|');
    double l_tax = std::stod(field);
    getline(stream, field, '|');
    std::string l_returnflag = field;
    getline(stream, field, '|');
    std::string l_linestatus = field;
    getline(stream, field, '|');
    unsigned long l_shipdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                               std::stoul(field.substr(8, 10));
    getline(stream, field, '|');
    unsigned long l_commitdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                 std::stoul(field.substr(8, 10));
    getline(stream, field, '|');
    unsigned long l_receiptdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                  std::stoul(field.substr(8, 10));
    getline(stream, field, '|');
    std::string l_shipinstruct = field;
    getline(stream, field, '|');
    std::string l_shipmode = field;
    getline(stream, field, '|');
    std::string l_comment = field;

    if (is_insert) {
      insert_into_lineitem(l_orderkey, l_extendedprice, l_discount, l_shipdate);
    } else {
      delete_from_lineitem(l_orderkey, l_extendedprice, l_discount, l_shipdate);
    }
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void DynaGoxTPCH3::milestone() {
  if (!delta_mode) {
    auto gk_go = hv_go.get_keys();
    while (gk_go != nullptr) {
      print(gk_go->o_orderkey, gk_go->o_orderdate, gk_go->o_shippriority, gk_go->annotation);
      gk_go = gk_go->next;
    }
  }
}

void DynaGoxTPCH3::insert_into_customer(unsigned long c_custkey, const std::string &c_mktsegment) {
  if (c_mktsegment == "BUILDING") {
    update_activate_r_c(c_custkey);
    t_o.o_custkey = c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_activate_apv_o(f_o->object->o_orderkey, f_o->object->o_orderdate, f_o->object->o_shippriority);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l = apv_l.get(k_l);
      if (g_l != nullptr) {
        update_add_hv_go(f_o->object->o_orderkey, f_o->object->o_orderdate, f_o->object->o_shippriority,
                         g_l->approximate);
        if (delta_mode) {
          print(f_o->object->o_orderkey, f_o->object->o_orderdate, f_o->object->o_shippriority, g_l->approximate);
        }
      }
      f_o = f_o->next;
    }
  }
}

void DynaGoxTPCH3::delete_from_customer(unsigned long c_custkey, const std::string &c_mktsegment) {
  if (c_mktsegment == "BUILDING") {
    update_remove_r_c(c_custkey);
    t_o.o_custkey = c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_remove_apv_o(f_o->object->o_orderkey);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l = apv_l.get(k_l);
      if (g_l != nullptr) {
        update_subtract_hv_go(f_o->object->o_orderkey, g_l->approximate);
        if (delta_mode) {
          print(f_o->object->o_orderkey, f_o->object->o_orderdate, f_o->object->o_shippriority, 0);
        }
      }
      f_o = f_o->next;
    }
  }
}

void DynaGoxTPCH3::insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                      unsigned long o_shippriority) {
  if (o_orderdate < 19950315) {
    update_activate_r_o(o_orderkey, o_custkey, o_orderdate, o_shippriority);
    t_c.c_custkey = o_custkey;
    auto g_c = r_c.get(t_c);
    if (g_c != nullptr) {
      update_activate_apv_o(o_orderkey, o_orderdate, o_shippriority);
      k_l.l_orderkey = o_orderkey;
      auto g_l = apv_l.get(k_l);
      if (g_l != nullptr) {
        update_add_hv_go(o_orderkey, o_orderdate, o_shippriority, g_l->approximate);
        if (delta_mode) {
          print(o_orderkey, o_orderdate, o_shippriority, g_l->approximate);
        }
      }
    }
  }
}

void DynaGoxTPCH3::delete_from_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                      unsigned long o_shippriority) {
  if (o_orderdate < 19950315) {
    update_remove_r_o(o_orderkey);
    t_c.c_custkey = o_custkey;
    auto g_c = r_c.get(t_c);
    if (g_c != nullptr) {
      update_remove_apv_o(o_orderkey);
      k_l.l_orderkey = o_orderkey;
      auto g_l = apv_l.get(k_l);
      if (g_l != nullptr) {
        update_subtract_hv_go(o_orderkey, g_l->approximate);
        if (delta_mode) {
          print(o_orderkey, o_orderdate, o_shippriority, 0);
        }
      }
    }
  }
}

void DynaGoxTPCH3::insert_into_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                        unsigned long l_shipdate) {
  if (l_shipdate > 19950315) {
    double delta_pv_l = update_add_approx_apv_l(l_orderkey, l_extendedprice * (1 - l_discount));
    if (delta_pv_l != 0) {
      k_o.o_orderkey = l_orderkey;
      auto g_o = apv_o.get(k_o);
      if (g_o != nullptr) {
        update_add_hv_go(g_o->o_orderkey, g_o->o_orderdate, g_o->o_shippriority, delta_pv_l);
        if (delta_mode) {
          t_go.o_orderkey = l_orderkey;
          auto g_go = hv_go.get(t_go);
          if (g_go != nullptr) {
            print(g_o->o_orderkey, g_o->o_orderdate, g_o->o_shippriority, g_go->annotation);
          }
        }
      }
    }
  }
}

void DynaGoxTPCH3::delete_from_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                        unsigned long l_shipdate) {
  if (l_shipdate > 19950315) {
    double delta_pv_l = update_subtract_approx_apv_l(l_orderkey, l_extendedprice * (1 - l_discount));
    if (delta_pv_l != 0) {
      k_o.o_orderkey = l_orderkey;
      auto g_o = apv_o.get(k_o);
      if (g_o != nullptr) {
        update_subtract_hv_go(g_o->o_orderkey, delta_pv_l);
        if (delta_mode) {
          t_go.o_orderkey = l_orderkey;
          auto g_go = hv_go.get(t_go);
          if (g_go != nullptr) {
            print(g_o->o_orderkey, g_o->o_orderdate, g_o->o_shippriority, g_go->annotation);
          } else {
            print(g_o->o_orderkey, g_o->o_orderdate, g_o->o_shippriority, 0);
          }
        }
      }
    }
  }
}

void DynaGoxTPCH3::print(unsigned long o_orderkey, unsigned long o_orderdate, unsigned long o_shippriority,
                         double value) {
  if (print_result) {
    fmt::println(output, "({},{},{}) -> {:.4f}", o_orderkey, o_orderdate, o_shippriority, value);
  }
}

void DynaGoxTPCH3::update_activate_r_c(unsigned long c_custkey) {
  t_c.c_custkey = c_custkey;
  r_c.update_activate(t_c);
}

void DynaGoxTPCH3::update_remove_r_c(unsigned long c_custkey) {
  t_c.c_custkey = c_custkey;
  r_c.update_remove_if_exists(t_c);
}

void DynaGoxTPCH3::update_activate_apv_o(unsigned long o_orderkey, unsigned long o_orderdate,
                                         unsigned long o_shippriority) {
  k_o.o_orderkey = o_orderkey;
  k_o.o_orderdate = o_orderdate;
  k_o.o_shippriority = o_shippriority;
  apv_o.update_activate(k_o);
}

void DynaGoxTPCH3::update_remove_apv_o(unsigned long o_orderkey) {
  k_o.o_orderkey = o_orderkey;
  apv_o.update_remove_if_exists(k_o);
}

void DynaGoxTPCH3::update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                       unsigned long o_shippriority) {
  t_o.o_orderkey = o_orderkey;
  t_o.o_custkey = o_custkey;
  t_o.o_orderdate = o_orderdate;
  t_o.o_shippriority = o_shippriority;
  r_o.update_activate(t_o);
}

void DynaGoxTPCH3::update_remove_r_o(unsigned long o_orderkey) {
  t_o.o_orderkey = o_orderkey;
  r_o.update_remove_if_exists(t_o);
}

double DynaGoxTPCH3::update_add_approx_apv_l(unsigned long l_orderkey, double value) {
  k_l.l_orderkey = l_orderkey;
  return apv_l.update_add_approx(k_l, value, b);
}

double DynaGoxTPCH3::update_subtract_approx_apv_l(unsigned long l_orderkey, double value) {
  k_l.l_orderkey = l_orderkey;
  return apv_l.update_subtract_approx(k_l, value, b);
}

void DynaGoxTPCH3::update_add_hv_go(unsigned long o_orderkey, unsigned long o_orderdate, unsigned long o_shippriority,
                                    double value) {
  t_go.o_orderkey = o_orderkey;
  t_go.o_orderdate = o_orderdate;
  t_go.o_shippriority = o_shippriority;
  hv_go.update_add(t_go, value);
}

void DynaGoxTPCH3::update_subtract_hv_go(unsigned long o_orderkey, double value) {
  t_go.o_orderkey = o_orderkey;
  hv_go.update_subtract(t_go, value);
}