#include "crown_tpch5.h"

void CrownTPCH5::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  bool is_insert = (field == "+");
  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 1) {
    getline(stream, field, '|');
    int s_suppkey = std::stoi(field);
    getline(stream, field, '|');
    std::string s_name = field;
    getline(stream, field, '|');
    std::string s_address = field;
    getline(stream, field, '|');
    int s_nationkey = std::stoi(field);
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
      insert_into_lineitem(l_orderkey, l_suppkey, l_linenumber, l_extendedprice, l_discount);
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
      insert_into_nation(n_nationkey, n_name, n_regionkey);
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

void CrownTPCH5::milestone() {
  if (!delta_mode) {
    auto gk_gl = sv_gl.get_keys();
    while (gk_gl != nullptr) {
      print(gk_gl->n_name, gk_gl->annotation);
      gk_gl = gk_gl->next;
    }
  }
}

void CrownTPCH5::insert_into_supplier(unsigned long s_suppkey, unsigned long s_nationkey) {
  update_activate_r_s(s_suppkey, s_nationkey);
  k_n.n_nationkey = s_nationkey;
  auto g_n = pv_n.get(k_n);
  if (g_n != nullptr) {
    update_activate_pv_s(s_suppkey, s_nationkey);
    t_l.l_suppkey = s_suppkey;
    auto f_l = r_l.find(1, t_l);
    while (f_l != nullptr) {
      k_o.o_orderkey = f_l->object->l_orderkey;
      auto g_o = pv_o.get(k_o);
      if (g_o != nullptr && g_o->c_nationkey == s_nationkey) {
        update_add_sv_gl(g_o->n_name, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
        if (delta_mode) {
          t_gl.n_name = g_o->n_name;
          auto g_gl = sv_gl.get(t_gl);
          if (g_gl != nullptr) {
            print(g_gl->n_name, g_gl->annotation);
          }
        }
      }
      f_l = f_l->next;
    }
  }
}

void CrownTPCH5::insert_into_customer(unsigned long c_custkey, unsigned long c_nationkey) {
  update_activate_r_c(c_custkey, c_nationkey);
  k_n.n_nationkey = c_nationkey;
  auto g_n = pv_n.get(k_n);
  if (g_n != nullptr) {
    update_activate_pv_c(c_custkey, c_nationkey, g_n->n_name);
    t_o.o_custkey = c_custkey;
    auto f_o = r_o.find(0, t_o);
    while (f_o != nullptr) {
      update_activate_pv_o(f_o->object->o_orderkey, c_nationkey, g_n->n_name);
      t_l.l_orderkey = f_o->object->o_orderkey;
      auto f_l = r_l.find(0, t_l);
      while (f_l != nullptr) {
        k_s.s_suppkey = f_l->object->l_suppkey;
        auto g_s = pv_s.get(k_s);
        if (g_s != nullptr && g_s->s_nationkey == c_nationkey) {
          update_add_sv_gl(g_n->n_name, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
          if (delta_mode) {
            t_gl.n_name = g_n->n_name;
            auto g_gl = sv_gl.get(t_gl);
            if (g_gl != nullptr) {
              print(g_gl->n_name, g_gl->annotation);
            }
          }
        }
        f_l = f_l->next;
      }
      f_o = f_o->next;
    }
  }
}

void CrownTPCH5::insert_into_orders(unsigned long o_orderkey, unsigned long o_custkey, unsigned long o_orderdate) {
  if (o_orderdate >= 19940101 && o_orderdate < 19950101) {
    update_activate_r_o(o_orderkey, o_custkey);
    k_c.c_custkey = o_custkey;
    auto g_c = pv_c.get(k_c);
    if (g_c != nullptr) {
      update_activate_pv_o(o_orderkey, g_c->c_nationkey, g_c->n_name);
      t_l.l_orderkey = o_orderkey;
      auto f_l = r_l.find(0, t_l);
      while (f_l != nullptr) {
        k_s.s_suppkey = f_l->object->l_suppkey;
        auto g_s = pv_s.get(k_s);
        if (g_s != nullptr && g_s->s_nationkey == g_c->c_nationkey) {
          update_add_sv_gl(g_c->n_name, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
          if (delta_mode) {
            t_gl.n_name = g_c->n_name;
            auto g_gl = sv_gl.get(t_gl);
            if (g_gl != nullptr) {
              print(g_gl->n_name, g_gl->annotation);
            }
          }
        }
        f_l = f_l->next;
      }
    }
  }
}

void CrownTPCH5::insert_into_lineitem(unsigned long l_orderkey, unsigned long l_suppkey, unsigned long l_linenumber,
                                      double l_extendedprice, double l_discount) {
  update_activate_r_l(l_orderkey, l_suppkey, l_linenumber, l_extendedprice, l_discount);
  k_o.o_orderkey = l_orderkey;
  auto g_o = pv_o.get(k_o);
  if (g_o != nullptr) {
    k_s.s_suppkey = l_suppkey;
    auto g_s = pv_s.get(k_s);
    if (g_s != nullptr && g_o->c_nationkey == g_s->s_nationkey) {
      update_add_sv_gl(g_o->n_name, l_extendedprice * (1 - l_discount));
      if (delta_mode) {
        t_gl.n_name = g_o->n_name;
        auto g_gl = sv_gl.get(t_gl);
        if (g_gl != nullptr) {
          print(g_gl->n_name, g_gl->annotation);
        }
      }
    }
  }
}

void CrownTPCH5::insert_into_nation(unsigned long n_nationkey, const std::string &n_name, unsigned long n_regionkey) {
  update_activate_r_n(n_nationkey, n_name, n_regionkey);
  t_r.r_regionkey = n_regionkey;
  auto g_r = r_r.get(t_r);
  if (g_r != nullptr) {
    update_activate_pv_n(n_nationkey, n_name);
    t_s.s_nationkey = n_nationkey;
    auto f_s = r_s.find(0, t_s);
    while (f_s != nullptr) {
      update_activate_pv_s(f_s->object->s_suppkey, f_s->object->s_nationkey);
      f_s = f_s->next;
    }

    t_c.c_nationkey = n_nationkey;
    auto f_c = r_c.find(0, t_c);
    while (f_c != nullptr) {
      update_activate_pv_c(f_c->object->c_custkey, f_c->object->c_nationkey, n_name);
      t_o.o_custkey = f_c->object->c_custkey;
      auto f_o = r_o.find(0, t_o);
      while (f_o != nullptr) {
        update_activate_pv_o(f_o->object->o_orderkey, n_nationkey, n_name);
        t_l.l_orderkey = f_o->object->o_orderkey;
        auto f_l = r_l.find(0, t_l);
        while (f_l != nullptr) {
          k_s.s_suppkey = f_l->object->l_suppkey;
          auto g_s = pv_s.get(k_s);
          if (g_s != nullptr && g_s->s_nationkey == n_nationkey) {
            update_add_sv_gl(n_name, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
            if (delta_mode) {
              t_gl.n_name = n_name;
              auto g_gl = sv_gl.get(t_gl);
              if (g_gl != nullptr) {
                print(g_gl->n_name, g_gl->annotation);
              }
            }
          }
          f_l = f_l->next;
        }
        f_o = f_o->next;
      }
      f_c = f_c->next;
    }
  }
}

void CrownTPCH5::insert_into_region(unsigned long r_regionkey, std::string &r_name) {
  if (r_name == "ASIA") {
    update_activate_r_r(r_regionkey);
    t_n.n_regionkey = r_regionkey;
    auto f_n = r_n.find(0, t_n);
    while (f_n != nullptr) {
      update_activate_pv_n(f_n->object->n_nationkey, f_n->object->n_name);
      t_s.s_nationkey = f_n->object->n_nationkey;
      auto f_s = r_s.find(0, t_s);
      while (f_s != nullptr) {
        update_activate_pv_s(f_s->object->s_suppkey, f_s->object->s_nationkey);
        f_s = f_s->next;
      }

      t_c.c_nationkey = f_n->object->n_nationkey;
      auto f_c = r_c.find(0, t_c);
      while (f_c != nullptr) {
        update_activate_pv_c(f_c->object->c_custkey, f_c->object->c_nationkey, f_n->object->n_name);
        t_o.o_custkey = f_c->object->c_custkey;
        auto f_o = r_o.find(0, t_o);
        while (f_o != nullptr) {
          update_activate_pv_o(f_o->object->o_orderkey, f_n->object->n_nationkey, f_n->object->n_name);
          t_l.l_orderkey = f_o->object->o_orderkey;
          auto f_l = r_l.find(0, t_l);
          while (f_l != nullptr) {
            k_s.s_suppkey = f_l->object->l_suppkey;
            auto g_s = pv_s.get(k_s);
            if (g_s != nullptr && g_s->s_nationkey == f_n->object->n_nationkey) {
              update_add_sv_gl(f_n->object->n_name, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
              if (delta_mode) {
                t_gl.n_name = f_n->object->n_name;
                auto g_gl = sv_gl.get(t_gl);
                if (g_gl != nullptr) {
                  print(g_gl->n_name, g_gl->annotation);
                }
              }
            }
            f_l = f_l->next;
          }
          f_o = f_o->next;
        }
        f_c = f_c->next;
      }
      f_n = f_n->next;
    }
  }
}

void CrownTPCH5::print(const std::string &n_name, double value) {
  if (print_result) {
    fmt::println(output, "({}) -> {:.4f}", n_name, value);
  }
}

void CrownTPCH5::update_activate_r_s(unsigned long s_suppkey, unsigned long s_nationkey) {
  t_s.s_suppkey = s_suppkey;
  t_s.s_nationkey = s_nationkey;
  r_s.update_activate(t_s);
}

void CrownTPCH5::update_activate_pv_s(unsigned long s_suppkey, unsigned long s_nationkey) {
  k_s.s_suppkey = s_suppkey;
  k_s.s_nationkey = s_nationkey;
  pv_s.update_activate(k_s);
}

void CrownTPCH5::update_activate_r_c(unsigned long c_custkey, unsigned long c_nationkey) {
  t_c.c_custkey = c_custkey;
  t_c.c_nationkey = c_nationkey;
  r_c.update_activate(t_c);
}

void CrownTPCH5::update_activate_pv_c(unsigned long c_custkey, unsigned long c_nationkey, const std::string &n_name) {
  k_c.c_custkey = c_custkey;
  k_c.c_nationkey = c_nationkey;
  k_c.n_name = n_name;
  pv_c.update_activate(k_c);
}

void CrownTPCH5::update_activate_r_o(unsigned long o_orderkey, unsigned long o_custkey) {
  t_o.o_orderkey = o_orderkey;
  t_o.o_custkey = o_custkey;
  r_o.update_activate(t_o);
}

void CrownTPCH5::update_activate_pv_o(unsigned long o_orderkey, unsigned long c_nationkey, const std::string &n_name) {
  k_o.o_orderkey = o_orderkey;
  k_o.c_nationkey = c_nationkey;
  k_o.n_name = n_name;
  pv_o.update_activate(k_o);
}

void CrownTPCH5::update_activate_r_l(unsigned long l_orderkey, unsigned long l_suppkey, unsigned long l_linenumber,
                                     double l_extendedprice, double l_discount) {
  t_l.l_orderkey = l_orderkey;
  t_l.l_suppkey = l_suppkey;
  t_l.l_linenumber = l_linenumber;
  t_l.l_extendedprice = l_extendedprice;
  t_l.l_discount = l_discount;
  r_l.update_activate(t_l);
}

void CrownTPCH5::update_activate_r_n(unsigned long n_nationkey, const std::string &n_name, unsigned long n_regionkey) {
  t_n.n_nationkey = n_nationkey;
  t_n.n_name = n_name;
  t_n.n_regionkey = n_regionkey;
  r_n.update_activate(t_n);
}

void CrownTPCH5::update_activate_pv_n(unsigned long n_nationkey, const std::string &n_name) {
  k_n.n_nationkey = n_nationkey;
  k_n.n_name = n_name;
  pv_n.update_activate(k_n);
}

void CrownTPCH5::update_activate_r_r(unsigned long r_regionkey) {
  t_r.r_regionkey = r_regionkey;
  r_r.update_activate(t_r);
}

void CrownTPCH5::update_add_sv_gl(const std::string &n_name, double value) {
  t_gl.n_name = n_name;
  sv_gl.update_add(t_gl, value);
}