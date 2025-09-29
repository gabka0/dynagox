#include "dynagox_tpcds15.h"

void DynaGoxTPCDS15::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 1) {
    getline(stream, field, '|');
    unsigned long cs_sold_date_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long cs_bill_customer_sk = std::stoul(field);
    getline(stream, field, '|');
    double cs_sales_price = std::stod(field);

    insert_into_catalog_sales(cs_sold_date_sk, cs_bill_customer_sk, cs_sales_price);
  } else if (relation_id == 3) {
    getline(stream, field, '|');
    unsigned long c_customer_sk = std::stoul(field);
    getline(stream, field, '|');
    unsigned long c_current_addr_sk = std::stoul(field);

    insert_into_customer(c_customer_sk, c_current_addr_sk);
  } else if (relation_id == 4) {
    getline(stream, field, '|');
    unsigned long ca_address_sk = std::stoul(field);
    getline(stream, field, '|');
    std::string ca_zip = field;

    insert_into_customer_address(ca_address_sk, ca_zip);
  } else if (relation_id == 5) {
    getline(stream, field, '|');
    unsigned long d_date_sk = std::stoul(field);

    insert_into_date_dim(d_date_sk);
  }
}

void DynaGoxTPCDS15::milestone() {
  if (!delta_mode) {
    auto gk_gc = hv_gc.get_keys();
    while (gk_gc != nullptr) {
      print(gk_gc->ca_zip, gk_gc->annotation);
      gk_gc = gk_gc->next;
    }
  }
}

void DynaGoxTPCDS15::insert_into_catalog_sales(unsigned long cs_sold_date_sk, unsigned long cs_bill_customer_sk,
                                               double cs_sales_price) {
  t_cs.cs_sold_date_sk = cs_sold_date_sk;
  t_cs.cs_bill_customer_sk = cs_bill_customer_sk;
  t_cs.cs_sales_price = cs_sales_price;
  r_cs.update_add(t_cs, 1);

  t_d.d_date_sk = cs_sold_date_sk;
  auto g_d = r_d.get(t_d);
  if (g_d != nullptr) {
    k_cs.cs_bill_customer_sk = cs_bill_customer_sk;
    apv_cs.update_add(k_cs, cs_sales_price);

    k_c.c_customer_sk = cs_bill_customer_sk;
    auto g_c = apv_c.get(k_c);
    if (g_c != nullptr) {
      t_gc.ca_zip = g_c->ca_zip;
      hv_gc.update_add(t_gc, cs_sales_price);
      if (delta_mode) {
        auto g_gc = hv_gc.get(t_gc);
        print(g_gc->ca_zip, g_gc->annotation);
      }
    }
  }
}

void DynaGoxTPCDS15::insert_into_customer(unsigned long c_customer_sk, unsigned long c_current_addr_sk) {
  t_c.c_customer_sk = c_customer_sk;
  t_c.c_current_addr_sk = c_current_addr_sk;
  r_c.update_add(t_c, 1);

  t_ca.ca_address_sk = c_current_addr_sk;
  auto g_ca = r_ca.get(t_ca);
  if (g_ca != nullptr) {
    k_c.c_customer_sk = c_customer_sk;
    k_c.ca_zip = g_ca->ca_zip;
    apv_c.update_add(k_c, 1);

    k_cs.cs_bill_customer_sk = c_customer_sk;
    auto g_cs = apv_cs.get(k_cs);
    if (g_cs != nullptr) {
      t_gc.ca_zip = g_ca->ca_zip;
      hv_gc.update_add(t_gc, g_cs->annotation);
      if (delta_mode) {
        auto g_gc = hv_gc.get(t_gc);
        print(g_gc->ca_zip, g_gc->annotation);
      }
    }
  }
}

void DynaGoxTPCDS15::insert_into_customer_address(unsigned long ca_address_sk, const std::string &ca_zip) {
  t_ca.ca_address_sk = ca_address_sk;
  t_ca.ca_zip = ca_zip;
  r_ca.update_add(t_ca, 1);

  t_c.c_current_addr_sk = ca_address_sk;
  auto f_c = r_c.find(0, t_c);
  while (f_c != nullptr) {
    k_c.c_customer_sk = f_c->object->c_customer_sk;
    k_c.ca_zip = ca_zip;
    apv_c.update_add(k_c, 1);

    k_cs.cs_bill_customer_sk = f_c->object->c_customer_sk;
    auto g_cs = apv_cs.get(k_cs);
    if (g_cs != nullptr) {
      t_gc.ca_zip = ca_zip;
      hv_gc.update_add(t_gc, g_cs->annotation);
      if (delta_mode) {
        auto g_gc = hv_gc.get(t_gc);
        print(g_gc->ca_zip, g_gc->annotation);
      }
    }
    f_c = f_c->next;
  }
}

void DynaGoxTPCDS15::insert_into_date_dim(unsigned long d_date_sk) {
  t_d.d_date_sk = d_date_sk;
  r_d.update_add(t_d, 1);

  t_cs.cs_sold_date_sk = d_date_sk;
  auto f_cs = r_cs.find(0, t_cs);
  while (f_cs != nullptr) {
    k_cs.cs_bill_customer_sk = f_cs->object->cs_bill_customer_sk;
    apv_cs.update_add(k_cs, f_cs->object->cs_sales_price * f_cs->object->annotation);

    k_c.c_customer_sk = f_cs->object->cs_bill_customer_sk;
    auto g_c = apv_c.get(k_c);
    if (g_c != nullptr) {
      t_gc.ca_zip = g_c->ca_zip;
      hv_gc.update_add(t_gc, f_cs->object->cs_sales_price * f_cs->object->annotation);
      if (delta_mode) {
        auto g_gc = hv_gc.get(t_gc);
        print(g_gc->ca_zip, g_gc->annotation);
      }
    }

    f_cs = f_cs->next;
  }
}

void DynaGoxTPCDS15::print(const std::string &ca_zip, double value) {
  if (print_result) {
    fmt::println(output, "({}) -> {:.4f}", ca_zip, value);
  }
}