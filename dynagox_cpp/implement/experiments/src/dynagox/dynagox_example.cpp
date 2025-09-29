#include "dynagox_example.h"

void DynaGoxExample::process(const std::string &line) {}

void DynaGoxExample::milestone() {
  if (!delta_mode) {
    auto gk_gr2 = r_gr2.get_keys();
    while (gk_gr2 != nullptr) {
      k_r2.f0 = gk_gr2->f0;
      auto g_r2 = apv_r2.get(k_r2);
      k_r3.f0 = gk_gr2->f0;
      auto g_r3 = apv_r3.get(k_r3);

      print(gk_gr2->f0, g_r2->annotation * g_r3->annotation);
      gk_gr2 = gk_gr2->next;
    }
  }
}

void DynaGoxExample::run() {
  fmt::print("Initial state:\n");
  for (int i = 0; i < 9; i++) {
    insert_into_r1(1, 2);
  }
  for (int i = 0; i < 2; i++) {
    insert_into_r1(1, 3);
  }
  for (int i = 0; i < 3; i++) {
    insert_into_r1(2, 3);
  }
  for (int i = 0; i < 10; i++) {
    insert_into_r1(3, 3);
  }

  for (int i = 0; i < 4; i++) {
    insert_into_r2(2, 4);
  }
  for (int i = 0; i < 2; i++) {
    insert_into_r2(2, 6);
  }
  for (int i = 0; i < 1; i++) {
    insert_into_r2(3, 4);
  }
  for (int i = 0; i < 3; i++) {
    insert_into_r2(3, 5);
  }

  for (int i = 0; i < 2; i++) {
    insert_into_r3(4, 5);
  }
  for (int i = 0; i < 3; i++) {
    insert_into_r3(5, 6);
  }
  for (int i = 0; i < 2; i++) {
    insert_into_r3(5, 7);
  }

  print_state();
  milestone();
  fmt::print("\n");

  fmt::print("Inserting (1,3) into R1:\n");
  insert_into_r1(1,3);
  print_state();
  milestone();
  fmt::print("\n");

  fmt::print("Inserting (2,3) into R1:\n");
  insert_into_r1(2,3);
  print_state();
  milestone();
  fmt::print("\n");

  fmt::print("Inserting (6,7) into R3:\n");
  insert_into_r3(6,7);
  print_state();
  milestone();
  fmt::print("\n");
}

void DynaGoxExample::print_state() {
  fmt::println("R_R1:");
  auto gk_r_r1 = r_r1.get_keys();
  while (gk_r_r1 != nullptr) {
    fmt::println("({},{}) -> {:.4f}", gk_r_r1->f0, gk_r_r1->f1, gk_r_r1->annotation);
    gk_r_r1 = gk_r_r1->next;
  }
  fmt::println("");

  fmt::println("A_R1 & P_R1:");
  auto gk_apv_r1 = apv_r1.get_keys();
  while (gk_apv_r1 != nullptr) {
    fmt::println("({}) -> ({:.4f},{:.4f})", gk_apv_r1->f0, gk_apv_r1->annotation, gk_apv_r1->approximate);
    gk_apv_r1 = gk_apv_r1->next;
  }
  fmt::println("");

  fmt::println("R_R2:");
  auto gk_r_r2 = r_r2.get_keys();
  while (gk_r_r2 != nullptr) {
    t_r2.f0 = gk_r_r2->f0;
    t_r2.f1 = gk_r_r2->f1;
    auto g_h_r2 = h_r2.get(t_r2);

    fmt::println("({},{}) -> ({:.4f},{:.4f})", gk_r_r2->f0, gk_r_r2->f1, gk_r_r2->annotation, g_h_r2 != nullptr ? g_h_r2->annotation : 0);
    gk_r_r2 = gk_r_r2->next;
  }
  fmt::println("");

  fmt::println("A_R2:");
  auto gk_apv_r2 = apv_r2.get_keys();
  while (gk_apv_r2 != nullptr) {
    fmt::println("({}) -> ({:.4f})", gk_apv_r2->f0, gk_apv_r2->annotation);
    gk_apv_r2 = gk_apv_r2->next;
  }
  fmt::println("");

  fmt::println("R_R3:");
  auto gk_r_r3 = r_r3.get_keys();
  while (gk_r_r3 != nullptr) {
    fmt::println("({},{}) -> {:.4f}", gk_r_r3->f0, gk_r_r3->f1, gk_r_r3->annotation);
    gk_r_r3 = gk_r_r3->next;
  }
  fmt::println("");

  fmt::println("A_R3:");
  auto gk_apv_r3 = apv_r3.get_keys();
  while (gk_apv_r3 != nullptr) {
    fmt::println("({}) -> ({:.4f})", gk_apv_r3->f0, gk_apv_r3->annotation);
    gk_apv_r3 = gk_apv_r3->next;
  }
  fmt::println("");

  fmt::println("H_GR2:");
  auto gk_r_gr2 = r_gr2.get_keys();
  while (gk_r_gr2 != nullptr) {
    fmt::println("({})", gk_r_gr2->f0);
    gk_r_gr2 = gk_r_gr2->next;
  }
  fmt::println("");
}

void DynaGoxExample::insert_into_r1(unsigned long x1, unsigned long x2) {
  t_r1.f0 = x1;
  t_r1.f1 = x2;
  r_r1.update_add(t_r1, 1);

  k_r1.f0 = x2;
  double d_r1 = apv_r1.update_add_approx(k_r1, 1, b);
  if (d_r1 != 0) {
    t_r2.f0 = x2;
    auto f_r2 = r_r2.find(0, t_r2);
    while (f_r2 != nullptr) {
      t_r2.f0 = f_r2->object->f0;
      t_r2.f1 = f_r2->object->f1;
      h_r2.update_add(t_r2, d_r1 * f_r2->object->annotation);

      k_r2.f0 = f_r2->object->f1;
      apv_r2.update_add(k_r2, d_r1 * f_r2->object->annotation);

      k_r3.f0 = f_r2->object->f1;
      auto g_r3 = apv_r3.get(k_r3);
      if (g_r3 != nullptr) {
        t_gr2.f0 = f_r2->object->f1;
        r_gr2.update_activate(t_gr2);
      }

      f_r2 = f_r2->next;
    }
  }
}

void DynaGoxExample::insert_into_r2(unsigned long x2, unsigned long x3) {
  t_r2.f0 = x2;
  t_r2.f1 = x3;
  r_r2.update_add(t_r2, 1);

  k_r1.f0 = x2;
  auto g_r1 = apv_r1.get(k_r1);
  if (g_r1 != nullptr) {
    h_r2.update_add(t_r2, g_r1->approximate);

    k_r2.f0 = x3;
    apv_r2.update_add(k_r2, g_r1->approximate);

    k_r3.f0 = x3;
    auto g_r3 = apv_r3.get(k_r3);
    if (g_r3 != nullptr) {
      t_gr2.f0 = x3;
      r_gr2.update_activate(t_gr2);
    }
  }
}

void DynaGoxExample::insert_into_r3(unsigned long x3, unsigned long x4) {
  t_r3.f0 = x3;
  t_r3.f1 = x4;
  r_r3.update_add(t_r3, 1);

  k_r3.f0 = x3;
  apv_r3.update_add(k_r3, 1);

  k_r2.f0 = x3;
  auto g_r2 = apv_r2.get(k_r2);
  if (g_r2 != nullptr) {
    t_gr2.f0 = x3;
    r_gr2.update_activate(t_gr2);
  }
}

void DynaGoxExample::print(unsigned long x3, double value) {
  if (print_result) {
    fmt::println(output, "({}) -> {:.4f}", x3, value);
  }
}