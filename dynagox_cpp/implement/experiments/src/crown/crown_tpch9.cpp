#include "crown_tpch9.h"

void CrownTPCH9::process(const std::string &line) {
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
      insert_into_part(p_partkey, p_name);
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
  } else if (relation_id == 2) {
    getline(stream, field, '|');
    unsigned long ps_partkey = std::stoul(field);
    getline(stream, field, '|');
    unsigned long ps_suppkey = std::stoul(field);
    getline(stream, field, '|');
    unsigned long ps_availqty = std::stoul(field);
    getline(stream, field, '|');
    double ps_supplycost = std::stod(field);
    getline(stream, field, '|');
    std::string ps_comment = field;

    if (is_insert) {
      insert_into_partsupp(ps_partkey, ps_suppkey, ps_supplycost);
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
      insert_into_orders(o_orderkey, o_year);
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
      insert_into_lineitem(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount);
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
    }
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void CrownTPCH9::milestone() {
  if (!delta_mode) {
    auto gk_gl1 = sv_gl1.get_keys();
    while (gk_gl1 != nullptr) {
      auto n_name = gk_gl1->n_name;
      auto o_year = gk_gl1->o_year;
      auto value1 = gk_gl1->annotation;

      t_gl.n_name = n_name;
      t_gl.o_year = o_year;
      auto g_gl2 = sv_gl2.get(t_gl);
      auto value2 = g_gl2 != nullptr ? g_gl2->annotation : 0;

      print(n_name, o_year, value1, value2);
      gk_gl1 = gk_gl1->next;
    }
  }
}

void CrownTPCH9::insert_into_part(unsigned long p_partkey, const std::string &p_name) {
  if (p_name.find("green") != std::string::npos) {
    update_activate_r_p(p_partkey);
    t_l.l_partkey = p_partkey;
    auto f_l = r_l.find(1, t_l);
    while (f_l != nullptr) {
      k_s.s_suppkey = f_l->object->l_suppkey;
      auto g_s = pv_s.get(k_s);
      if (g_s != nullptr) {
        t_ps.ps_partkey = f_l->object->l_partkey;
        t_ps.ps_suppkey = f_l->object->l_suppkey;
        auto g_ps = r_ps.get(t_ps);
        if (g_ps != nullptr) {
          t_o.o_orderkey = f_l->object->l_orderkey;
          auto g_o = r_o.get(t_o);
          if (g_o != nullptr) {
            update_add_sv_gl1(g_s->n_name, g_o->o_year, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
            update_add_sv_gl2(g_s->n_name, g_o->o_year, g_ps->ps_supplycost * f_l->object->l_quantity);

            if (delta_mode) {
              t_gl.n_name = g_s->n_name;
              t_gl.o_year = g_o->o_year;
              auto g_gl1 = sv_gl1.get(t_gl);
              auto g_gl2 = sv_gl2.get(t_gl);

              print(g_s->n_name, g_o->o_year,
                    (g_gl1 != nullptr ? g_gl1->annotation : 0),
                    (g_gl2 != nullptr ? g_gl2->annotation : 0));
            }
          }
        }
      }
      f_l = f_l->next;
    }
  }
}

void CrownTPCH9::insert_into_supplier(unsigned long s_suppkey, unsigned long s_nationkey) {
  update_activate_r_s(s_suppkey, s_nationkey);
  t_n.n_nationkey = s_nationkey;
  auto g_n = r_n.get(t_n);
  if (g_n != nullptr) {
    update_activate_pv_s(s_suppkey, g_n->n_name);
    t_l.l_suppkey = s_suppkey;
    auto f_l = r_l.find(2, t_l);
    while (f_l != nullptr) {
      t_p.p_partkey = f_l->object->l_partkey;
      auto g_p = r_p.get(t_p);
      if (g_p != nullptr) {
        t_ps.ps_partkey = f_l->object->l_partkey;
        t_ps.ps_suppkey = f_l->object->l_suppkey;
        auto g_ps = r_ps.get(t_ps);
        if (g_ps != nullptr) {
          t_o.o_orderkey = f_l->object->l_orderkey;
          auto g_o = r_o.get(t_o);
          if (g_o != nullptr) {
            update_add_sv_gl1(g_n->n_name, g_o->o_year, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
            update_add_sv_gl2(g_n->n_name, g_o->o_year, g_ps->ps_supplycost * f_l->object->l_quantity);

            if (delta_mode) {
              t_gl.n_name = g_n->n_name;
              t_gl.o_year = g_o->o_year;
              auto g_gl1 = sv_gl1.get(t_gl);
              auto g_gl2 = sv_gl2.get(t_gl);

              print(g_n->n_name, g_o->o_year,
                    (g_gl1 != nullptr ? g_gl1->annotation : 0),
                    (g_gl2 != nullptr ? g_gl2->annotation : 0));
            }
          }
        }
      }
      f_l = f_l->next;
    }
  }
}

void CrownTPCH9::insert_into_partsupp(unsigned long ps_partkey, unsigned long ps_suppkey, double ps_supplycost) {
  update_activate_r_ps(ps_partkey, ps_suppkey, ps_supplycost);
  t_l.l_partkey = ps_partkey;
  t_l.l_suppkey = ps_suppkey;
  auto f_l = r_l.find(3, t_l);
  while (f_l != nullptr) {
    t_p.p_partkey = f_l->object->l_partkey;
    auto g_p = r_p.get(t_p);
    if (g_p != nullptr) {
      k_s.s_suppkey = f_l->object->l_suppkey;
      auto g_s = pv_s.get(k_s);
      if (g_s != nullptr) {
        t_o.o_orderkey = f_l->object->l_orderkey;
        auto g_o = r_o.get(t_o);
        if (g_o != nullptr) {
          update_add_sv_gl1(g_s->n_name, g_o->o_year, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
          update_add_sv_gl2(g_s->n_name, g_o->o_year, ps_supplycost * f_l->object->l_quantity);

          if (delta_mode) {
            t_gl.n_name = g_s->n_name;
            t_gl.o_year = g_o->o_year;
            auto g_gl1 = sv_gl1.get(t_gl);
            auto g_gl2 = sv_gl2.get(t_gl);

            print(g_s->n_name, g_o->o_year,
                  (g_gl1 != nullptr ? g_gl1->annotation : 0),
                  (g_gl2 != nullptr ? g_gl2->annotation : 0));
          }
        }
      }
    }
    f_l = f_l->next;
  }
}

void CrownTPCH9::insert_into_orders(unsigned long o_orderkey, unsigned long o_year) {
  update_activate_r_o(o_orderkey, o_year);
  t_l.l_orderkey = o_orderkey;
  auto f_l = r_l.find(0, t_l);
  while (f_l != nullptr) {
    t_p.p_partkey = f_l->object->l_partkey;
    auto g_p = r_p.get(t_p);
    if (g_p != nullptr) {
      k_s.s_suppkey = f_l->object->l_suppkey;
      auto g_s = pv_s.get(k_s);
      if (g_s != nullptr) {
        t_ps.ps_partkey = f_l->object->l_partkey;
        t_ps.ps_suppkey = f_l->object->l_suppkey;
        auto g_ps = r_ps.get(t_ps);
        if (g_ps != nullptr) {
          update_add_sv_gl1(g_s->n_name, o_year, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
          update_add_sv_gl2(g_s->n_name, o_year, g_ps->ps_supplycost * f_l->object->l_quantity);

          if (delta_mode) {
            t_gl.n_name = g_s->n_name;
            t_gl.o_year = o_year;
            auto g_gl1 = sv_gl1.get(t_gl);
            auto g_gl2 = sv_gl2.get(t_gl);

            print(g_s->n_name, o_year,
                  (g_gl1 != nullptr ? g_gl1->annotation : 0),
                  (g_gl2 != nullptr ? g_gl2->annotation : 0));
          }
        }
      }
    }
    f_l = f_l->next;
  }
}

void CrownTPCH9::insert_into_lineitem(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                      unsigned long l_linenumber, double l_quantity, double l_extendedprice,
                                      double l_discount) {
  update_activate_r_l(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice, l_discount);
  t_p.p_partkey = l_partkey;
  auto g_p = r_p.get(t_p);
  if (g_p != nullptr) {
    k_s.s_suppkey = l_suppkey;
    auto g_s = pv_s.get(k_s);
    if (g_s != nullptr) {
      t_ps.ps_partkey = l_partkey;
      t_ps.ps_suppkey = l_suppkey;
      auto g_ps = r_ps.get(t_ps);
      if (g_ps != nullptr) {
        t_o.o_orderkey = l_orderkey;
        auto g_o = r_o.get(t_o);
        if (g_o != nullptr) {
          update_add_sv_gl1(g_s->n_name, g_o->o_year, l_extendedprice * (1 - l_discount));
          update_add_sv_gl2(g_s->n_name, g_o->o_year, g_ps->ps_supplycost * l_quantity);

          if (delta_mode) {
            t_gl.n_name = g_s->n_name;
            t_gl.o_year = g_o->o_year;
            auto g_gl1 = sv_gl1.get(t_gl);
            auto g_gl2 = sv_gl2.get(t_gl);

            print(g_s->n_name, g_o->o_year,
                  (g_gl1 != nullptr ? g_gl1->annotation : 0),
                  (g_gl2 != nullptr ? g_gl2->annotation : 0));
          }
        }
      }
    }
  }
}

void CrownTPCH9::insert_into_nation(unsigned long n_nationkey, const std::string &n_name) {
  update_activate_r_n(n_nationkey, n_name);
  t_s.s_nationkey = n_nationkey;
  auto f_s = r_s.find(0, t_s);
  while (f_s != nullptr) {
    update_activate_pv_s(f_s->object->s_suppkey, n_name);
    t_l.l_suppkey = f_s->object->s_suppkey;
    auto f_l = r_l.find(2, t_l);
    while (f_l != nullptr) {
      t_p.p_partkey = f_l->object->l_partkey;
      auto g_p = r_p.get(t_p);
      if (g_p != nullptr) {
        t_ps.ps_partkey = f_l->object->l_partkey;
        t_ps.ps_suppkey = f_l->object->l_suppkey;
        auto g_ps = r_ps.get(t_ps);
        if (g_ps != nullptr) {
          t_o.o_orderkey = f_l->object->l_orderkey;
          auto g_o = r_o.get(t_o);
          if (g_o != nullptr) {
            update_add_sv_gl1(n_name, g_o->o_year, f_l->object->l_extendedprice * (1 - f_l->object->l_discount));
            update_add_sv_gl2(n_name, g_o->o_year, g_ps->ps_supplycost * f_l->object->l_quantity);

            if (delta_mode) {
              t_gl.n_name = n_name;
              t_gl.o_year = g_o->o_year;
              auto g_gl1 = sv_gl1.get(t_gl);
              auto g_gl2 = sv_gl2.get(t_gl);

              print(n_name, g_o->o_year,
                    (g_gl1 != nullptr ? g_gl1->annotation : 0),
                    (g_gl2 != nullptr ? g_gl2->annotation : 0));
            }
          }
        }
      }
      f_l = f_l->next;
    }
    f_s = f_s->next;
  }
}

void CrownTPCH9::print(const std::string &n_name, unsigned long o_year, double value1, double value2) {
  if (print_result) {
    fmt::println(output, "({},{}) -> ({:.4f},{:.4f})", n_name, o_year, value1, value2);
  }
}

void CrownTPCH9::update_activate_r_p(unsigned long p_partkey) {
  t_p.p_partkey = p_partkey;
  r_p.update_activate(t_p);
}

void CrownTPCH9::update_activate_r_s(unsigned long s_suppkey, unsigned long s_nationkey) {
  t_s.s_suppkey = s_suppkey;
  t_s.s_nationkey = s_nationkey;
  r_s.update_activate(t_s);
}

void CrownTPCH9::update_activate_pv_s(unsigned long s_suppkey, const std::string &n_name) {
  k_s.s_suppkey = s_suppkey;
  k_s.n_name = n_name;
  pv_s.update_activate(k_s);
}

void CrownTPCH9::update_activate_r_ps(unsigned long ps_partkey, unsigned long ps_suppkey, double ps_supplycost) {
  t_ps.ps_partkey = ps_partkey;
  t_ps.ps_suppkey = ps_suppkey;
  t_ps.ps_supplycost = ps_supplycost;
  r_ps.update_activate(t_ps);
}

void CrownTPCH9::update_activate_r_o(unsigned long o_orderkey, unsigned long o_year) {
  t_o.o_orderkey = o_orderkey;
  t_o.o_year = o_year;
  r_o.update_activate(t_o);
}

void CrownTPCH9::update_activate_r_l(unsigned long l_orderkey, unsigned long l_partkey, unsigned long l_suppkey,
                                     unsigned long l_linenumber, double l_quantity, double l_extendedprice,
                                     double l_discount) {
  t_l.l_orderkey = l_orderkey;
  t_l.l_partkey = l_partkey;
  t_l.l_suppkey = l_suppkey;
  t_l.l_linenumber = l_linenumber;
  t_l.l_quantity = l_quantity;
  t_l.l_extendedprice = l_extendedprice;
  t_l.l_discount = l_discount;
  r_l.update_activate(t_l);
}

void CrownTPCH9::update_activate_r_n(unsigned long n_nationkey, const std::string &n_name) {
  t_n.n_nationkey = n_nationkey;
  t_n.n_name = n_name;
  r_n.update_activate(t_n);
}

void CrownTPCH9::update_add_sv_gl1(const std::string &n_name, unsigned long o_year, double value) {
  t_gl.n_name = n_name;
  t_gl.o_year = o_year;
  sv_gl1.update_add(t_gl, value);
}

void CrownTPCH9::update_add_sv_gl2(const std::string &n_name, unsigned long o_year, double value) {
  t_gl.n_name = n_name;
  t_gl.o_year = o_year;
  sv_gl2.update_add(t_gl, value);
}