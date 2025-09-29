#include "crown_tpch10.h"

void CrownTPCH10::process(const std::string &line) {
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
      insert_into_customer(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_comment);
    } else {
      delete_from_customer(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_comment);
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
      insert_into_orders(o_orderkey, o_custkey, o_orderdate);
    } else {
      delete_from_orders(o_orderkey, o_custkey, o_orderdate);
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
      insert_into_lineitem(l_orderkey, l_extendedprice, l_discount, l_returnflag);
    } else {
      delete_from_lineitem(l_orderkey, l_extendedprice, l_discount, l_returnflag);
    }
  } else if (relation_id == 6) {
    getline(stream, field, '|');
    int n_nationkey = std::stoi(field);
    getline(stream, field, '|');
    std::string n_name = field;
    getline(stream, field, '|');
    int n_regionkey = std::stoi(field);
    getline(stream, field, '|');
    std::string n_comment = field;

    if (is_insert) {
      insert_into_nation(n_nationkey, n_name);
    } else {
      delete_from_nation(n_nationkey, n_name);
    }
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void CrownTPCH10::milestone() {
  if (!delta_mode) {
    auto gk_go = sv_go.get_keys();
    while (gk_go != nullptr) {
      print(gk_go->c_custkey, gk_go->c_name, gk_go->c_address, gk_go->c_phone, gk_go->c_acctbal, gk_go->c_comment,
            gk_go->n_name, gk_go->annotation);
      gk_go = gk_go->next;
    }
  }
}

void CrownTPCH10::insert_into_customer(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                       unsigned long c_nationkey, const std::string &c_phone, double c_acctbal,
                                       const std::string &c_comment) {
  update_activate_r_c(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_comment);
  t_n.n_nationkey = c_nationkey;
  auto g_n = r_n.get(t_n);
  if (g_n != nullptr) {
    update_activate_pv_c(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n->n_name);
    t_o.o_custkey = c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_activate_pv_o(f_o->object->o_orderkey, c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment,
                           g_n->n_name);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l = pv_l.get(k_l);
      if (g_l != nullptr) {
        update_add_sv_go(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n->n_name, g_l->annotation);
        if (delta_mode) {
          auto g_go = sv_go.get(t_go);
          if (g_go != nullptr) {
            print(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n->n_name, g_go->annotation);
          }
        }
      }
      f_o = f_o->next;
    }
  }
}

void CrownTPCH10::delete_from_customer(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                       unsigned long c_nationkey, const std::string &c_phone, double c_acctbal,
                                       const std::string &c_comment) {
  update_remove_r_c(c_custkey);
  t_n.n_nationkey = c_nationkey;
  auto g_n = r_n.get(t_n);
  if (g_n != nullptr) {
    update_remove_pv_c(c_custkey);
    t_o.o_custkey = c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_remove_pv_o(f_o->object->o_orderkey);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l = pv_l.get(k_l);
      if (g_l != nullptr) {
        update_subtract_sv_go(c_custkey, g_l->annotation);
        if (delta_mode) {
          print(c_custkey, c_name, c_address, c_phone, c_acctbal, c_comment, g_n->n_name, 0);
        }
      }
      f_o = f_o->next;
    }
  }
}

void CrownTPCH10::insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate) {
  if (o_orderdate >= 19931001 && o_orderdate < 19940101) {
    update_activate_r_o(o_orderkey, o_custkey);
    k_c.c_custkey = o_custkey;
    auto g_c = pv_c.get(k_c);
    if (g_c != nullptr) {
      update_activate_pv_o(o_orderkey, o_custkey, g_c->c_name, g_c->c_address, g_c->c_phone, g_c->c_acctbal,
                           g_c->c_comment, g_c->n_name);
      k_l.l_orderkey = o_orderkey;
      auto g_l = pv_l.get(k_l);
      if (g_l != nullptr) {
        update_add_sv_go(o_custkey, g_c->c_name, g_c->c_address, g_c->c_phone, g_c->c_acctbal,
                         g_c->c_comment, g_c->n_name, g_l->annotation);
        if (delta_mode) {
          auto g_go = sv_go.get(t_go);
          if (g_go != nullptr) {
            print(g_c->c_custkey, g_c->c_name, g_c->c_address, g_c->c_phone, g_c->c_acctbal, g_c->c_comment,
                  g_c->n_name, g_go->annotation);
          }
        }
      }
    }
  }
}

void CrownTPCH10::delete_from_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate) {
  if (o_orderdate >= 19931001 && o_orderdate < 19940101) {
    update_remove_r_o(o_orderkey);
    k_c.c_custkey = o_custkey;
    auto g_c = pv_c.get(k_c);
    if (g_c != nullptr) {
      update_remove_pv_o(o_orderkey);
      k_l.l_orderkey = o_orderkey;
      auto g_l = pv_l.get(k_l);
      if (g_l != nullptr) {
        update_subtract_sv_go(g_c->c_custkey, g_l->annotation);
        if (delta_mode) {
          t_go.c_custkey = g_c->c_custkey;
          auto g_go = sv_go.get(t_go);
          if (g_go != nullptr) {
            print(g_c->c_custkey, g_c->c_name, g_c->c_address, g_c->c_phone, g_c->c_acctbal, g_c->c_comment,
                  g_c->n_name, g_go->annotation);
          } else {
            print(g_c->c_custkey, g_c->c_name, g_c->c_address, g_c->c_phone, g_c->c_acctbal, g_c->c_comment,
                  g_c->n_name, 0);
          }
        }
      }
    }
  }
}

void CrownTPCH10::insert_into_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                       const std::string &l_returnflag) {
  if (l_returnflag == "R") {
    update_add_pv_l(l_orderkey, l_extendedprice * (1 - l_discount));
    k_o.o_orderkey = l_orderkey;
    auto g_o = pv_o.get(k_o);
    if (g_o != nullptr) {
      update_add_sv_go(g_o->c_custkey, g_o->c_name, g_o->c_address, g_o->c_phone, g_o->c_acctbal, g_o->c_comment,
                       g_o->n_name, l_extendedprice * (1 - l_discount));
      if (delta_mode) {
        auto g_go = sv_go.get(t_go);
        if (g_go != nullptr) {
          print(g_o->c_custkey, g_o->c_name, g_o->c_address, g_o->c_phone, g_o->c_acctbal, g_o->c_comment, g_o->n_name,
                g_go->annotation);
        }
      }
    }
  }
}

void CrownTPCH10::delete_from_lineitem(unsigned long l_orderkey, double l_extendedprice, double l_discount,
                                       const std::string &l_returnflag) {
  if (l_returnflag == "R") {
    update_subtract_pv_l(l_orderkey, l_extendedprice * (1 - l_discount));
    k_o.o_orderkey = l_orderkey;
    auto g_o = pv_o.get(k_o);
    if (g_o != nullptr) {
      update_subtract_sv_go(g_o->c_custkey, l_extendedprice * (1 - l_discount));
      if (delta_mode) {
        t_go.c_custkey = g_o->c_custkey;
        auto g_go = sv_go.get(t_go);
        if (g_go != nullptr) {
          print(g_o->c_custkey, g_o->c_name, g_o->c_address, g_o->c_phone, g_o->c_acctbal, g_o->c_comment,
                g_o->n_name, g_go->annotation);
        } else {
          print(g_o->c_custkey, g_o->c_name, g_o->c_address, g_o->c_phone, g_o->c_acctbal, g_o->c_comment,
                g_o->n_name, 0);
        }
      }
    }
  }
}

void CrownTPCH10::insert_into_nation(unsigned long n_nationkey, const std::string &n_name) {
  update_activate_r_n(n_nationkey, n_name);
  t_c.c_nationkey = n_nationkey;
  auto f_c = r_c.find(0, t_c);
  while (f_c != nullptr) {
    update_activate_pv_c(f_c->object->c_custkey, f_c->object->c_name, f_c->object->c_address, f_c->object->c_phone,
                         f_c->object->c_acctbal, f_c->object->c_comment, n_name);
    t_o.o_custkey = f_c->object->c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_activate_pv_o(f_o->object->o_orderkey, f_c->object->c_custkey, f_c->object->c_name, f_c->object->c_address,
                           f_c->object->c_phone, f_c->object->c_acctbal, f_c->object->c_comment, n_name);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l = pv_l.get(k_l);
      if (g_l != nullptr) {
        update_add_sv_go(f_c->object->c_custkey, f_c->object->c_name, f_c->object->c_address,
                         f_c->object->c_phone, f_c->object->c_acctbal, f_c->object->c_comment, n_name, g_l->annotation);
        if (delta_mode) {
          auto g_go = sv_go.get(t_go);
          if (g_go != nullptr) {
            print(f_c->object->c_custkey, f_c->object->c_name, f_c->object->c_address, f_c->object->c_phone,
                  f_c->object->c_acctbal, f_c->object->c_comment, n_name, g_go->annotation);
          }
        }
      }
      f_o = f_o->next;
    }
    f_c = f_c->next;
  }
}

void CrownTPCH10::delete_from_nation(unsigned long n_nationkey, const std::string &n_name) {
  update_remove_r_n(n_nationkey);
  t_c.c_nationkey = n_nationkey;
  auto f_c = r_c.find(0, t_c);
  while (f_c != nullptr) {
    update_remove_pv_c(f_c->object->c_custkey);
    t_o.o_custkey = f_c->object->c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_remove_pv_o(f_o->object->o_orderkey);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l = pv_l.get(k_l);
      if (g_l != nullptr) {
        update_subtract_sv_go(f_c->object->c_custkey, g_l->annotation);
        if (delta_mode) {
          print(f_c->object->c_custkey, f_c->object->c_name, f_c->object->c_address, f_c->object->c_phone,
                f_c->object->c_acctbal, f_c->object->c_comment, n_name, 0);
        }
      }
      f_o = f_o->next;
    }
    f_c = f_c->next;
  }
}

void CrownTPCH10::print(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                        const std::string &c_phone, double c_acctbal, const std::string &c_comment,
                        const std::string &n_name, double value) {
  if (print_result) {
    fmt::println(output, "({},{},{},{},{},{},{}) -> {:.4f}", c_custkey, c_name, c_address, c_phone, c_acctbal,
                 c_comment, n_name, value);
  }
}

void CrownTPCH10::update_activate_r_c(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                      unsigned long c_nationkey, const std::string &c_phone, double c_acctbal,
                                      const std::string &c_comment) {
  t_c.c_custkey = c_custkey;
  t_c.c_name = c_name;
  t_c.c_address = c_address;
  t_c.c_nationkey = c_nationkey;
  t_c.c_phone = c_phone;
  t_c.c_acctbal = c_acctbal;
  t_c.c_comment = c_comment;
  r_c.update_activate(t_c);
}

void CrownTPCH10::update_remove_r_c(unsigned long c_custkey) {
  t_c.c_custkey = c_custkey;
  r_c.update_remove_if_exists(t_c);
}

void CrownTPCH10::update_activate_pv_c(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                       const std::string &c_phone, double c_acctbal, const std::string &c_comment,
                                       const std::string &n_name) {
  k_c.c_custkey = c_custkey;
  k_c.c_name = c_name;
  k_c.c_address = c_address;
  k_c.c_phone = c_phone;
  k_c.c_acctbal = c_acctbal;
  k_c.c_comment = c_comment;
  k_c.n_name = n_name;
  pv_c.update_activate(k_c);
}

void CrownTPCH10::update_remove_pv_c(unsigned long c_custkey) {
  k_c.c_custkey = c_custkey;
  pv_c.update_remove_if_exists(k_c);
}

void CrownTPCH10::update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey) {
  t_o.o_orderkey = o_orderkey;
  t_o.o_custkey = o_custkey;
  r_o.update_activate(t_o);
}

void CrownTPCH10::update_remove_r_o(unsigned long o_orderkey) {
  t_o.o_orderkey = o_orderkey;
  r_o.update_remove_if_exists(t_o);
}

void CrownTPCH10::update_activate_pv_o(unsigned long o_orderkey, unsigned long c_custkey, const std::string &c_name,
                                       const std::string &c_address, const std::string &c_phone, double c_acctbal,
                                       const std::string &c_comment, const std::string &n_name) {
  k_o.o_orderkey = o_orderkey;
  k_o.c_custkey = c_custkey;
  k_o.c_name = c_name;
  k_o.c_address = c_address;
  k_o.c_phone = c_phone;
  k_o.c_acctbal = c_acctbal;
  k_o.c_comment = c_comment;
  k_o.n_name = n_name;
  pv_o.update_activate(k_o);
}

void CrownTPCH10::update_remove_pv_o(unsigned long o_orderkey) {
  k_o.o_orderkey = o_orderkey;
  pv_o.update_remove_if_exists(k_o);
}

void CrownTPCH10::update_add_pv_l(unsigned long l_orderkey, double value) {
  k_l.l_orderkey = l_orderkey;
  pv_l.update_add(k_l, value);
}

void CrownTPCH10::update_subtract_pv_l(unsigned long l_orderkey, double value) {
  k_l.l_orderkey = l_orderkey;
  pv_l.update_subtract(k_l, value);
}

void CrownTPCH10::update_activate_r_n(unsigned long n_nationkey, const std::string &n_name) {
  t_n.n_nationkey = n_nationkey;
  t_n.n_name = n_name;
  r_n.update_activate(t_n);
}

void CrownTPCH10::update_remove_r_n(unsigned long n_nationkey) {
  t_n.n_nationkey = n_nationkey;
  r_n.update_remove_if_exists(t_n);
}

void CrownTPCH10::update_add_sv_go(unsigned long c_custkey, const std::string &c_name, const std::string &c_address,
                                   const std::string &c_phone, double c_acctbal, const std::string &c_comment,
                                   const std::string &n_name, double value) {
  t_go.c_custkey = c_custkey;
  t_go.c_name = c_name;
  t_go.c_address = c_address;
  t_go.c_phone = c_phone;
  t_go.c_acctbal = c_acctbal;
  t_go.c_comment = c_comment;
  t_go.n_name = n_name;
  sv_go.update_add(t_go, value);
}

void CrownTPCH10::update_subtract_sv_go(unsigned long c_custkey, double value) {
  t_go.c_custkey = c_custkey;
  sv_go.update_subtract(t_go, value);
}