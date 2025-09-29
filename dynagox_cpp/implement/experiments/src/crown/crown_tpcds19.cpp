#include "crown_tpcds19.h"

void CrownTPCDS19::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 0) {
    getline(stream, field, '|');
    unsigned long ss_sold_date_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long ss_item_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long ss_customer_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long ss_store_sk = std::stoul(field);
    getline(stream, field, '|');
    double ss_ext_sales_price = std::stod(field);

    insert_into_store_sales(ss_sold_date_sk, ss_item_sk, ss_customer_sk, ss_store_sk, ss_ext_sales_price);
  } else if (relation_id == 2) {
    getline(stream, field, '|');
    unsigned long s_store_sk = std::stoul(field);

    insert_into_store(s_store_sk);
  } else if (relation_id == 3) {
    getline(stream, field, '|');
    unsigned long c_customer_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long c_current_addr_sk = std::stoul(field);

    insert_into_customer(c_customer_sk, c_current_addr_sk);
  } else if (relation_id == 4) {
    getline(stream, field, '|');
    unsigned long ca_address_sk = std::stoul(field);

    insert_into_customer_address(ca_address_sk);
  } else if (relation_id == 5) {
    getline(stream, field, '|');
    unsigned long d_date_sk = std::stoul(field);

    insert_into_date_dim(d_date_sk);
  } else if (relation_id == 6) {
    getline(stream, field, '|');
    unsigned long i_item_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long i_brand_id = std::stoul(field);
    getline(stream, field, '|');
    std::string i_brand = field;
    getline(stream, field, '|');
    unsigned long i_manufact_id = std::stoul(field);
    getline(stream, field, '|');
    std::string i_manufact = field;

    insert_into_item(i_item_sk, i_brand_id, i_brand, i_manufact_id, i_manufact);
  }
}

void CrownTPCDS19::milestone() {
  if (!delta_mode) {
    auto gk_gss = sv_gss.get_keys();
    while (gk_gss != nullptr) {
      print(gk_gss->i_brand_id, gk_gss->i_brand, gk_gss->i_manufact_id, gk_gss->i_manufact, gk_gss->annotation);
      gk_gss = gk_gss->next;
    }
  }
}

void CrownTPCDS19::insert_into_store_sales(unsigned long ss_sold_date_sk, unsigned long ss_item_sk,
                                           unsigned long ss_customer_sk, unsigned long ss_store_sk,
                                           double ss_ext_sales_price) {
  t_ss.ss_sold_date_sk = ss_sold_date_sk;
  t_ss.ss_item_sk = ss_item_sk;
  t_ss.ss_customer_sk = ss_customer_sk;
  t_ss.ss_store_sk = ss_store_sk;
  t_ss.ss_ext_sales_price = ss_ext_sales_price;
  r_ss.update_add(t_ss, 1);

  t_s.s_store_sk = ss_store_sk;
  auto g_s = r_s.get(t_s);
  if (g_s != nullptr) {
    k_c.c_customer_sk = ss_customer_sk;
    auto g_c = pv_c.get(k_c);
    if (g_c != nullptr) {
      t_d.d_date_sk = ss_sold_date_sk;
      auto g_d = r_d.get(t_d);
      if (g_d != nullptr) {
        t_i.i_item_sk = ss_item_sk;
        auto g_i = r_i.get(t_i);
        if (g_i != nullptr) {
          t_gss.i_brand_id = g_i->i_brand_id;
          t_gss.i_brand = g_i->i_brand;
          t_gss.i_manufact_id = g_i->i_manufact_id;
          t_gss.i_manufact = g_i->i_manufact;
          sv_gss.update_add(t_gss, ss_ext_sales_price);
          if (delta_mode) {
            auto g_gss = sv_gss.get(t_gss);
            print(g_gss->i_brand_id, g_gss->i_brand, g_gss->i_manufact_id, g_gss->i_manufact, g_gss->annotation);
          }
        }
      }
    }
  }
}

void CrownTPCDS19::insert_into_store(unsigned long s_store_sk) {
  t_s.s_store_sk = s_store_sk;
  r_s.update_add(t_s, 1);

  t_ss.ss_store_sk = s_store_sk;
  auto f_ss = r_ss.find(3, t_ss);
  while (f_ss != nullptr) {
    k_c.c_customer_sk = f_ss->object->ss_customer_sk;
    auto g_c = pv_c.get(k_c);
    if (g_c != nullptr) {
      t_d.d_date_sk = f_ss->object->ss_sold_date_sk;
      auto g_d = r_d.get(t_d);
      if (g_d != nullptr) {
        t_i.i_item_sk = f_ss->object->ss_item_sk;
        auto g_i = r_i.get(t_i);
        if (g_i != nullptr) {
          t_gss.i_brand_id = g_i->i_brand_id;
          t_gss.i_brand = g_i->i_brand;
          t_gss.i_manufact_id = g_i->i_manufact_id;
          t_gss.i_manufact = g_i->i_manufact;
          sv_gss.update_add(t_gss, f_ss->object->ss_ext_sales_price);
          if (delta_mode) {
            auto g_gss = sv_gss.get(t_gss);
            print(g_gss->i_brand_id, g_gss->i_brand, g_gss->i_manufact_id, g_gss->i_manufact, g_gss->annotation);
          }
        }
      }
    }
    f_ss = f_ss->next;
  }
}

void CrownTPCDS19::insert_into_customer(unsigned long c_customer_sk, unsigned long c_current_addr_sk) {
  t_c.c_customer_sk = c_customer_sk;
  t_c.c_current_addr_sk = c_current_addr_sk;
  r_c.update_add(t_c, 1);

  t_ca.ca_address_sk = c_current_addr_sk;
  auto g_ca = r_ca.get(t_ca);
  if (g_ca != nullptr) {
    k_c.c_customer_sk = c_customer_sk;
    pv_c.update_add(k_c, 1);

    t_ss.ss_customer_sk = c_customer_sk;
    auto f_ss = r_ss.find(2, t_ss);
    while (f_ss != nullptr) {
      t_s.s_store_sk = f_ss->object->ss_store_sk;
      auto g_s = r_s.get(t_s);
      if (g_s != nullptr) {
        t_d.d_date_sk = f_ss->object->ss_sold_date_sk;
        auto g_d = r_d.get(t_d);
        if (g_d != nullptr) {
          t_i.i_item_sk = f_ss->object->ss_item_sk;
          auto g_i = r_i.get(t_i);
          if (g_i != nullptr) {
            t_gss.i_brand_id = g_i->i_brand_id;
            t_gss.i_brand = g_i->i_brand;
            t_gss.i_manufact_id = g_i->i_manufact_id;
            t_gss.i_manufact = g_i->i_manufact;
            sv_gss.update_add(t_gss, f_ss->object->ss_ext_sales_price);
            if (delta_mode) {
              auto g_gss = sv_gss.get(t_gss);
              print(g_gss->i_brand_id, g_gss->i_brand, g_gss->i_manufact_id, g_gss->i_manufact, g_gss->annotation);
            }
          }
        }
      }
      f_ss = f_ss->next;
    }
  }
}

void CrownTPCDS19::insert_into_customer_address(unsigned long ca_address_sk) {
  t_ca.ca_address_sk = ca_address_sk;
  r_ca.update_add(t_ca, 1);

  t_c.c_current_addr_sk = ca_address_sk;
  auto f_c = r_c.find(0, t_c);
  while (f_c != nullptr) {
    k_c.c_customer_sk = f_c->object->c_customer_sk;
    pv_c.update_add(k_c, 1);

    t_ss.ss_customer_sk = f_c->object->c_customer_sk;
    auto f_ss = r_ss.find(2, t_ss);
    while (f_ss != nullptr) {
      t_s.s_store_sk = f_ss->object->ss_store_sk;
      auto g_s = r_s.get(t_s);
      if (g_s != nullptr) {
        t_d.d_date_sk = f_ss->object->ss_sold_date_sk;
        auto g_d = r_d.get(t_d);
        if (g_d != nullptr) {
          t_i.i_item_sk = f_ss->object->ss_item_sk;
          auto g_i = r_i.get(t_i);
          if (g_i != nullptr) {
            t_gss.i_brand_id = g_i->i_brand_id;
            t_gss.i_brand = g_i->i_brand;
            t_gss.i_manufact_id = g_i->i_manufact_id;
            t_gss.i_manufact = g_i->i_manufact;
            sv_gss.update_add(t_gss, f_ss->object->ss_ext_sales_price);
            if (delta_mode) {
              auto g_gss = sv_gss.get(t_gss);
              print(g_gss->i_brand_id, g_gss->i_brand, g_gss->i_manufact_id, g_gss->i_manufact, g_gss->annotation);
            }
          }
        }
      }
      f_ss = f_ss->next;
    }
    f_c = f_c->next;
  }
}

void CrownTPCDS19::insert_into_date_dim(unsigned long d_date_sk) {
  t_d.d_date_sk = d_date_sk;
  r_d.update_add(t_d, 1);

  t_ss.ss_sold_date_sk = d_date_sk;
  auto f_ss = r_ss.find(0, t_ss);
  while (f_ss != nullptr) {
    t_s.s_store_sk = f_ss->object->ss_store_sk;
    auto g_s = r_s.get(t_s);
    if (g_s != nullptr) {
      k_c.c_customer_sk = f_ss->object->ss_customer_sk;
      auto g_c = pv_c.get(k_c);
      if (g_c != nullptr) {
        t_i.i_item_sk = f_ss->object->ss_item_sk;
        auto g_i = r_i.get(t_i);
        if (g_i != nullptr) {
          t_gss.i_brand_id = g_i->i_brand_id;
          t_gss.i_brand = g_i->i_brand;
          t_gss.i_manufact_id = g_i->i_manufact_id;
          t_gss.i_manufact = g_i->i_manufact;
          sv_gss.update_add(t_gss, f_ss->object->ss_ext_sales_price);
          if (delta_mode) {
            auto g_gss = sv_gss.get(t_gss);
            print(g_gss->i_brand_id, g_gss->i_brand, g_gss->i_manufact_id, g_gss->i_manufact, g_gss->annotation);
          }
        }
      }
    }
    f_ss = f_ss->next;
  }
}

void CrownTPCDS19::insert_into_item(unsigned long i_item_sk, unsigned long i_brand_id, const std::string &i_brand,
                                    unsigned long i_manufact_id, const std::string &i_manufact) {
  t_i.i_item_sk = i_item_sk;
  t_i.i_brand_id = i_brand_id;
  t_i.i_brand = i_brand;
  t_i.i_manufact_id = i_manufact_id;
  t_i.i_manufact = i_manufact;
  r_i.update_add(t_i, 1);

  t_ss.ss_item_sk = i_item_sk;
  auto f_ss = r_ss.find(1, t_ss);
  while (f_ss != nullptr) {
    t_s.s_store_sk = f_ss->object->ss_store_sk;
    auto g_s = r_s.get(t_s);
    if (g_s != nullptr) {
      k_c.c_customer_sk = f_ss->object->ss_customer_sk;
      auto g_c = pv_c.get(k_c);
      if (g_c != nullptr) {
        t_d.d_date_sk = f_ss->object->ss_sold_date_sk;
        auto g_d = r_d.get(t_d);
        if (g_d != nullptr) {
          t_gss.i_brand_id = i_brand_id;
          t_gss.i_brand = i_brand;
          t_gss.i_manufact_id = i_manufact_id;
          t_gss.i_manufact = i_manufact;
          sv_gss.update_add(t_gss, f_ss->object->ss_ext_sales_price);
          if (delta_mode) {
            auto g_gss = sv_gss.get(t_gss);
            print(g_gss->i_brand_id, g_gss->i_brand, g_gss->i_manufact_id, g_gss->i_manufact, g_gss->annotation);
          }
        }
      }
    }
    f_ss = f_ss->next;
  }
}

void CrownTPCDS19::print(unsigned long i_brand_id, const std::string &i_brand, unsigned long i_manufact_id,
                         const std::string &i_manufact, double value) {
  if (print_result) {
    fmt::println(output, "({},{},{},{}) -> {:.4f}", i_brand_id, i_brand, i_manufact_id, i_manufact, value);
  }
}