#include "dynagox_tpch8.h"

void DynaGoxTPCH8::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  bool is_insert = (field == "+");
  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 0) {
    getline(stream, field, '|');
    unsigned long p_partkey = std::stoul(field);
    getline(stream, field, '|');
    std::string p_name = field;
    getline(stream, field, '|');
    std::string p_mfgr = field;
    getline(stream, field, '|');
    std::string p_brand = field;
    getline(stream, field, '|');
    std::string p_type = field;
    getline(stream, field, '|');
    unsigned long p_size = std::stoul(field);
    getline(stream, field, '|');
    std::string p_container = field;
    getline(stream, field, '|');
    double p_retailprice = std::stod(field);
    getline(stream, field, '|');
    std::string p_comment = field;

    if (is_insert) {
      insert_into_part(p_partkey, p_type);
    }
  } else if (relation_id == 1) {
    getline(stream, field, '|');
    unsigned long s_suppkey = std::stoul(field);
    getline(stream, field, '|');
    std::string s_name = field;
    getline(stream, field, '|');
    std::string s_address = field;
    getline(stream, field, '|');
    unsigned long s_nationkey = std::stoi(field);
    getline(stream, field, '|');
    std::string s_phone = field;
    getline(stream, field, '|');
    double s_acctbal = std::stod(field);
    getline(stream, field, '|');
    std::string s_comment = field;

    if (is_insert) {
      insert_into_supplier(s_suppkey, s_nationkey);
    }
  } else if (relation_id == 3) {
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
      insert_into_customer(c_custkey, c_nationkey);
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
    unsigned long o_year = std::stoul(field.substr(0, 4));
    unsigned long o_orderdate = o_year * 10000 + std::stoul(field.substr(5, 7)) * 100 +
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
      insert_into_orders(o_orderkey, o_custkey, o_orderdate, o_year);
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
      insert_into_lineitem(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_extendedprice, l_discount);
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
      insert_into_nation1(n_nationkey, n_regionkey);
      insert_into_nation2(n_nationkey, n_name);
    }
  } else if (relation_id == 7) {
    getline(stream, field, '|');
    int r_regionkey = std::stoi(field);
    getline(stream, field, '|');
    std::string r_name = field;
    getline(stream, field, '|');
    std::string r_comment = field;

    if (is_insert) {
      insert_into_region(r_regionkey, r_name);
    }
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void DynaGoxTPCH8::milestone() {
  if (!delta_mode) {
    auto gk_go2 = hv_go2.get_keys();
    while (gk_go2 != nullptr) {
      auto o_year = gk_go2->o_year;
      auto value2 = gk_go2->annotation;

      t_go.o_year = o_year;
      auto g_go1 = hv_go1.get(t_go);
      auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;

      print(gk_go2->o_year, value1, value2);
      gk_go2 = gk_go2->next;
    }
  }
}

void DynaGoxTPCH8::insert_into_part(unsigned long p_partkey, const std::string &p_type) {
  if (p_type == "ECONOMY ANODIZED STEEL") {
    update_activate_r_p(p_partkey);
    t_l.l_partkey = p_partkey;
    auto f_l = r_l.find(0, t_l);
    while (f_l != nullptr) {
      k_s.s_suppkey = f_l->object->l_suppkey;
      auto g_s = apv_s.get(k_s);
      if (g_s != nullptr) {
        double volume = f_l->object->l_extendedprice * (1 - f_l->object->l_discount);
        double delta_pv_l1 = 0;
        double delta_pv_l2 = 0;
        if (g_s->n_name == "BRAZIL") {
          delta_pv_l1 = update_add_approx_apv_l1(f_l->object->l_orderkey, volume);
        }
        delta_pv_l2 = update_add_approx_apv_l2(f_l->object->l_orderkey, volume);

        if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
          k_o.o_orderkey = f_l->object->l_orderkey;
          auto g_o = apv_o.get(k_o);
          if (g_o != nullptr) {
            if (g_s->n_name == "BRAZIL") {
              update_add_hv_go1(g_o->o_year, delta_pv_l1);
            }
            update_add_hv_go2(g_o->o_year, delta_pv_l2);

            if (delta_mode) {
              t_go.o_year = g_o->o_year;
              auto g_go1 = hv_go1.get(t_go);
              auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
              auto g_go2 = hv_go2.get(t_go);
              auto value2 = g_go2->annotation;
              print(g_o->o_year, value1, value2);
            }
          }
        }
      }
      f_l = f_l->next;
    }
  }
}

void DynaGoxTPCH8::insert_into_supplier(unsigned long s_suppkey, unsigned long s_nationkey) {
  update_activate_r_s(s_suppkey, s_nationkey);
  t_n2.n_nationkey = s_nationkey;
  auto g_n2 = r_n2.get(t_n2);
  if (g_n2 != nullptr) {
    update_activate_apv_s(s_suppkey, g_n2->n_name);
    t_l.l_suppkey = s_suppkey;
    auto f_l = r_l.find(1, t_l);
    while (f_l != nullptr) {
      t_p.p_partkey = f_l->object->l_partkey;
      auto g_p = r_p.get(t_p);
      if (g_p != nullptr) {
        double volume = f_l->object->l_extendedprice * (1 - f_l->object->l_discount);
        double delta_pv_l1 = 0;
        double delta_pv_l2 = 0;
        if (g_n2->n_name == "BRAZIL") {
          delta_pv_l1 = update_add_approx_apv_l1(f_l->object->l_orderkey, volume);
        }
        delta_pv_l2 = update_add_approx_apv_l2(f_l->object->l_orderkey, volume);

        if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
          k_o.o_orderkey = f_l->object->l_orderkey;
          auto g_o = apv_o.get(k_o);
          if (g_o != nullptr) {
            if (g_n2->n_name == "BRAZIL") {
              update_add_hv_go1(g_o->o_year, delta_pv_l1);
            }
            update_add_hv_go2(g_o->o_year, delta_pv_l2);

            if (delta_mode) {
              t_go.o_year = g_o->o_year;
              auto g_go1 = hv_go1.get(t_go);
              auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
              auto g_go2 = hv_go2.get(t_go);
              auto value2 = g_go2->annotation;
              print(g_o->o_year, value1, value2);
            }
          }
        }
      }
      f_l = f_l->next;
    }
  }
}

void DynaGoxTPCH8::insert_into_customer(unsigned long c_custkey, unsigned long c_nationkey) {
  update_activate_r_c(c_custkey, c_nationkey);
  k_n1.n_nationkey = c_nationkey;
  auto g_n1 = apv_n1.get(k_n1);
  if (g_n1 != nullptr) {
    update_activate_apv_c(c_custkey);
    t_o.o_custkey = c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_activate_apv_o(f_o->object->o_orderkey, f_o->object->o_year);
      k_l.l_orderkey = f_o->object->o_orderkey;
      auto g_l1 = apv_l1.get(k_l);
      if (g_l1 != nullptr) {
        update_add_hv_go1(f_o->object->o_year, g_l1->approximate);
      }

      auto g_l2 = apv_l2.get(k_l);
      if (g_l2 != nullptr) {
        update_add_hv_go2(f_o->object->o_year, g_l2->approximate);

        if (delta_mode) {
          t_go.o_year = f_o->object->o_year;
          auto g_go1 = hv_go1.get(t_go);
          auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
          auto g_go2 = hv_go2.get(t_go);
          auto value2 = g_go2->annotation;
          print(f_o->object->o_year, value1, value2);
        }
      }
      f_o = f_o->next;
    }
  }
}

void DynaGoxTPCH8::insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate,
                                    unsigned long o_year) {
  if (19950101 <= o_orderdate && o_orderdate <= 19961231) {
    update_activate_r_o(o_orderkey, o_custkey, o_year);
    k_c.c_custkey = o_custkey;
    auto g_c = apv_c.get(k_c);
    if (g_c != nullptr) {
      update_activate_apv_o(o_orderkey, o_year);
      k_l.l_orderkey = o_orderkey;
      auto g_l1 = apv_l1.get(k_l);
      if (g_l1 != nullptr) {
        update_add_hv_go1(o_year, g_l1->approximate);
      }

      auto g_l2 = apv_l2.get(k_l);
      if (g_l2 != nullptr) {
        update_add_hv_go2(o_year, g_l2->approximate);

        if (delta_mode) {
          t_go.o_year = o_year;
          auto g_go1 = hv_go1.get(t_go);
          auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
          auto g_go2 = hv_go2.get(t_go);
          auto value2 = g_go2->annotation;
          print(o_year, value1, value2);
        }
      }
    }
  }
}

void DynaGoxTPCH8::insert_into_lineitem(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                      unsigned long l_linenumber, double l_extendedprice, double l_discount) {
  update_activate_r_l(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_extendedprice, l_discount);
  t_p.p_partkey = l_partkey;
  auto g_p = r_p.get(t_p);
  if (g_p != nullptr) {
    k_s.s_suppkey = l_suppkey;
    auto g_s = apv_s.get(k_s);
    if (g_s != nullptr) {
      double volume = l_extendedprice * (1 - l_discount);
      double delta_pv_l1 = 0;
      double delta_pv_l2 = 0;
      if (g_s->n_name == "BRAZIL") {
        delta_pv_l1 = update_add_approx_apv_l1(l_orderkey, volume);
      }
      delta_pv_l2 = update_add_approx_apv_l2(l_orderkey, volume);

      if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
        k_o.o_orderkey = l_orderkey;
        auto g_o = apv_o.get(k_o);
        if (g_o != nullptr) {
          if (g_s->n_name == "BRAZIL") {
            update_add_hv_go1(g_o->o_year, delta_pv_l1);
          }
          update_add_hv_go2(g_o->o_year, delta_pv_l2);

          if (delta_mode) {
            t_go.o_year = g_o->o_year;
            auto g_go1 = hv_go1.get(t_go);
            auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
            auto g_go2 = hv_go2.get(t_go);
            auto value2 = g_go2->annotation;
            print(g_o->o_year, value1, value2);
          }
        }
      }
    }
  }
}

void DynaGoxTPCH8::insert_into_nation1(unsigned long n_nationkey, unsigned long n_regionkey) {
  update_activate_r_n1(n_nationkey, n_regionkey);
  t_r.r_regionkey = n_regionkey;
  auto g_r = r_r.get(t_r);
  if (g_r != nullptr) {
    update_activate_apv_n1(n_nationkey);
    t_c.c_nationkey = n_nationkey;
    auto f_c = r_c.find(0, t_c);
    while (f_c != nullptr) {
      update_activate_apv_c(f_c->object->c_custkey);
      t_o.o_custkey = f_c->object->c_custkey;
      auto f_o = r_o.find(0, t_o);
      while (f_o != nullptr) {
        update_activate_apv_o(f_o->object->o_orderkey, f_o->object->o_year);
        k_l.l_orderkey = f_o->object->o_orderkey;
        auto g_l1 = apv_l1.get(k_l);
        if (g_l1 != nullptr) {
          update_add_hv_go1(f_o->object->o_year, g_l1->approximate);
        }

        auto g_l2 = apv_l2.get(k_l);
        if (g_l2 != nullptr) {
          update_add_hv_go2(f_o->object->o_year, g_l2->approximate);

          if (delta_mode) {
            t_go.o_year = f_o->object->o_year;
            auto g_go1 = hv_go1.get(t_go);
            auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
            auto g_go2 = hv_go2.get(t_go);
            auto value2 = g_go2->annotation;
            print(f_o->object->o_year, value1, value2);
          }
        }
        f_o = f_o->next;
      }
      f_c = f_c->next;
    }
  }
}

void DynaGoxTPCH8::insert_into_nation2(unsigned long n_nationkey, const std::string &n_name) {
  update_activate_r_n2(n_nationkey, n_name);
  t_s.s_nationkey = n_nationkey;
  auto f_s = r_s.find(0, t_s);
  while (f_s != nullptr) {
    update_activate_apv_s(f_s->object->s_suppkey, n_name);
    t_l.l_suppkey = f_s->object->s_suppkey;
    auto f_l = r_l.find(1, t_l);
    while (f_l != nullptr) {
      t_p.p_partkey = f_l->object->l_partkey;
      auto g_p = r_p.get(t_p);
      if (g_p != nullptr) {
        double volume = f_l->object->l_extendedprice * (1 - f_l->object->l_discount);
        double delta_pv_l1 = 0;
        double delta_pv_l2 = 0;
        if (n_name == "BRAZIL") {
          delta_pv_l1 = update_add_approx_apv_l1(f_l->object->l_orderkey, volume);
        }
        delta_pv_l2 = update_add_approx_apv_l2(f_l->object->l_orderkey, volume);

        if (delta_pv_l1 != 0 || delta_pv_l2 != 0) {
          k_o.o_orderkey = f_l->object->l_orderkey;
          auto g_o = apv_o.get(k_o);
          if (g_o != nullptr) {
            if (n_name == "BRAZIL") {
              update_add_hv_go1(g_o->o_year, delta_pv_l1);
            }
            update_add_hv_go2(g_o->o_year, delta_pv_l2);

            if (delta_mode) {
              t_go.o_year = g_o->o_year;
              auto g_go1 = hv_go1.get(t_go);
              auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
              auto g_go2 = hv_go2.get(t_go);
              auto value2 = g_go2->annotation;
              print(g_o->o_year, value1, value2);
            }
          }
        }
      }
      f_l = f_l->next;
    }
    f_s = f_s->next;
  }
}

void DynaGoxTPCH8::insert_into_region(unsigned long r_regionkey, std::string &r_name) {
  if (r_name == "AMERICA") {
    update_activate_r_r(r_regionkey);
    t_n1.n_regionkey = r_regionkey;
    auto f_n1 = r_n1.find(0, t_n1);
    while (f_n1 != nullptr) {
      update_activate_apv_n1(f_n1->object->n_nationkey);
      t_c.c_nationkey = f_n1->object->n_nationkey;
      auto f_c = r_c.find(0, t_c);
      while (f_c != nullptr) {
        update_activate_apv_c(f_c->object->c_custkey);
        t_o.o_custkey = f_c->object->c_custkey;
        auto f_o = r_o.find(0, t_o);
        while (f_o != nullptr) {
          update_activate_apv_o(f_o->object->o_orderkey, f_o->object->o_year);
          k_l.l_orderkey = f_o->object->o_orderkey;
          auto g_l1 = apv_l1.get(k_l);
          if (g_l1 != nullptr) {
            update_add_hv_go1(f_o->object->o_year, g_l1->approximate);
          }

          auto g_l2 = apv_l2.get(k_l);
          if (g_l2 != nullptr) {
            update_add_hv_go2(f_o->object->o_year, g_l2->approximate);

            if (delta_mode) {
              t_go.o_year = f_o->object->o_year;
              auto g_go1 = hv_go1.get(t_go);
              auto value1 = g_go1 != nullptr ? g_go1->annotation : 0;
              auto g_go2 = hv_go2.get(t_go);
              auto value2 = g_go2->annotation;
              print(f_o->object->o_year, value1, value2);
            }
          }
          f_o = f_o->next;
        }
        f_c = f_c->next;
      }
      f_n1 = f_n1->next;
    }
  }
}

void DynaGoxTPCH8::print(unsigned long o_year, double value1, double value2) {
  if (print_result) {
    fmt::println(output, "({}) -> ({:.4f},{:.4f})", o_year, value1, value2);
  }
}

void DynaGoxTPCH8::update_activate_r_p(unsigned long p_partkey) {
  t_p.p_partkey = p_partkey;
  r_p.update_activate(t_p);
}

void DynaGoxTPCH8::update_activate_r_s(unsigned long s_suppkey, unsigned long s_nationkey) {
  t_s.s_suppkey = s_suppkey;
  t_s.s_nationkey = s_nationkey;
  r_s.update_activate(t_s);
}

void DynaGoxTPCH8::update_activate_apv_s(unsigned long s_suppkey, const std::string &n_name) {
  k_s.s_suppkey = s_suppkey;
  k_s.n_name = n_name;
  apv_s.update_activate(k_s);
}

void DynaGoxTPCH8::update_activate_r_c(unsigned long c_custkey, unsigned long c_nationkey) {
  t_c.c_custkey = c_custkey;
  t_c.c_nationkey = c_nationkey;
  r_c.update_activate(t_c);
}

void DynaGoxTPCH8::update_activate_apv_c(unsigned long c_custkey) {
  k_c.c_custkey = c_custkey;
  apv_c.update_activate(k_c);
}

void DynaGoxTPCH8::update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_year) {
  t_o.o_orderkey = o_orderkey;
  t_o.o_custkey = o_custkey;
  t_o.o_year = o_year;
  r_o.update_activate(t_o);
}

void DynaGoxTPCH8::update_activate_apv_o(unsigned long o_orderkey, unsigned long o_year) {
  k_o.o_orderkey = o_orderkey;
  k_o.o_year = o_year;
  apv_o.update_activate(k_o);
}

void DynaGoxTPCH8::update_activate_r_l(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                     unsigned long l_linenumber, double l_extendedprice, double l_discount) {
  t_l.l_orderkey = l_orderkey;
  t_l.l_partkey = l_partkey;
  t_l.l_suppkey = l_suppkey;
  t_l.l_linenumber = l_linenumber;
  t_l.l_extendedprice = l_extendedprice;
  t_l.l_discount = l_discount;
  r_l.update_activate(t_l);
}

double DynaGoxTPCH8::update_add_approx_apv_l1(unsigned long l_orderkey, double value) {
  k_l.l_orderkey = l_orderkey;
  return apv_l1.update_add_approx(k_l, value, b);
}

double DynaGoxTPCH8::update_add_approx_apv_l2(unsigned long l_orderkey, double value) {
  k_l.l_orderkey = l_orderkey;
  return apv_l2.update_add_approx(k_l, value, b);
}

void DynaGoxTPCH8::update_activate_r_n1(unsigned long n_nationkey, unsigned long n_regionkey) {
  t_n1.n_nationkey = n_nationkey;
  t_n1.n_regionkey = n_regionkey;
  r_n1.update_activate(t_n1);
}

void DynaGoxTPCH8::update_activate_apv_n1(unsigned long n_nationkey) {
  k_n1.n_nationkey = n_nationkey;
  apv_n1.update_activate(k_n1);
}

void DynaGoxTPCH8::update_activate_r_n2(unsigned long n_nationkey, const std::string &n_name) {
  t_n2.n_nationkey = n_nationkey;
  t_n2.n_name = n_name;
  r_n2.update_activate(t_n2);
}

void DynaGoxTPCH8::update_activate_r_r(unsigned long r_regionkey) {
  t_r.r_regionkey = r_regionkey;
  r_r.update_activate(t_r);
}

void DynaGoxTPCH8::update_add_hv_go1(unsigned long o_year, double value) {
  t_go.o_year = o_year;
  hv_go1.update_add(t_go, value);
}

void DynaGoxTPCH8::update_add_hv_go2(unsigned long o_year, double value) {
  t_go.o_year = o_year;
  hv_go2.update_add(t_go, value);
}