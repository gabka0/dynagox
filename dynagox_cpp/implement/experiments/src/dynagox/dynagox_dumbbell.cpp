#include "dynagox_dumbbell.h"

void DynaGoxDumbbell::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  getline(stream, field, '|');
  unsigned long src = std::stoul(field);
  getline(stream, field, '|');
  unsigned long dst = std::stoul(field);

  insert_into_r1(src, dst);
  insert_into_r2(src, dst);
  insert_into_r3(src, dst);
  insert_into_r4(src, dst);
  insert_into_r5(src, dst);
  insert_into_r6(src, dst);
  insert_into_r7(src, dst);
}

void DynaGoxDumbbell::milestone() { print(); }

void DynaGoxDumbbell::insert_into_r1(unsigned long src, unsigned long dst) {
  r1t.f0 = src;
  r1t.f1 = dst;
  r1.update_add(r1t, 1);

  r2t.f0 = dst;
  auto tuple_r2 = r2.find(0, r2t);
  while (tuple_r2 != nullptr) {
    r3t.f0 = tuple_r2->object->f1;
    r3t.f1 = src;
    if (r3.contains(r3t)) {
      insert_into_b1(src, dst, tuple_r2->object->f1);
    }
    tuple_r2 = tuple_r2->next;
  }
}

void DynaGoxDumbbell::insert_into_r2(unsigned long src, unsigned long dst) {
  r2t.f0 = src;
  r2t.f1 = dst;
  r2.update_add(r2t, 1);

  r3t.f0 = dst;
  auto tuple_r3 = r3.find(0, r3t);
  while (tuple_r3 != nullptr) {
    r1t.f0 = tuple_r3->object->f1;
    r1t.f1 = src;
    if (r1.contains(r1t)) {
      insert_into_b1(tuple_r3->object->f1, src, dst);
    }
    tuple_r3 = tuple_r3->next;
  }
}

void DynaGoxDumbbell::insert_into_r3(unsigned long src, unsigned long dst) {
  r3t.f0 = src;
  r3t.f1 = dst;
  r3.update_add(r3t, 1);

  r1t.f0 = dst;
  auto tuple_r1 = r1.find(0, r1t);
  while (tuple_r1 != nullptr) {
    r2t.f0 = tuple_r1->object->f1;
    r2t.f1 = src;
    if (r2.contains(r2t)) {
      insert_into_b1(dst, tuple_r1->object->f1, src);
    }
    tuple_r1 = tuple_r1->next;
  }
}

void DynaGoxDumbbell::insert_into_r4(unsigned long src, unsigned long dst) {
  r4t.f0 = src;
  r4t.f1 = dst;
  r4.update_add(r4t, 1);

  b2k.f0 = dst;
  auto kb2 = apv_b2.get(b2k);
  if (kb2 != nullptr) {
    r4k.f0 = src;
    pv_r4.update_add(r4k, kb2->approximate);

    b1k.f0 = src;
    auto kb1 = apv_b1.get(b1k);
    if (kb1 != nullptr) {
      result += (kb1->annotation * kb2->approximate);
    }
  }
}

void DynaGoxDumbbell::insert_into_r5(unsigned long src, unsigned long dst) {
  r5t.f0 = src;
  r5t.f1 = dst;
  r5.update_add(r5t, 1);

  r6t.f0 = dst;
  auto tuple_r6 = r6.find(0, r6t);
  while (tuple_r6 != nullptr) {
    r7t.f0 = tuple_r6->object->f1;
    r7t.f1 = src;
    if (r7.contains(r7t)) {
      insert_into_b2(src, dst, tuple_r6->object->f1);
    }
    tuple_r6 = tuple_r6->next;
  }
}

void DynaGoxDumbbell::insert_into_r6(unsigned long src, unsigned long dst) {
  r6t.f0 = src;
  r6t.f1 = dst;
  r6.update_add(r6t, 1);

  r7t.f0 = dst;
  auto tuple_r7 = r7.find(0, r7t);
  while (tuple_r7 != nullptr) {
    r5t.f0 = tuple_r7->object->f1;
    r5t.f1 = src;
    if (r5.contains(r5t)) {
      insert_into_b2(tuple_r7->object->f1, src, dst);
    }
    tuple_r7 = tuple_r7->next;
  }
}

void DynaGoxDumbbell::insert_into_r7(unsigned long src, unsigned long dst) {
  r7t.f0 = src;
  r7t.f1 = dst;
  r7.update_add(r7t, 1);

  r5t.f0 = dst;
  auto tuple_r5 = r5.find(0, r5t);
  while (tuple_r5 != nullptr) {
    r6t.f0 = tuple_r5->object->f1;
    r6t.f1 = src;
    if (r6.contains(r6t)) {
      insert_into_b2(dst, tuple_r5->object->f1, src);
    }
    tuple_r5 = tuple_r5->next;
  }
}

void DynaGoxDumbbell::insert_into_b1(unsigned long f0, unsigned long f1,
                                     unsigned long f2) {
  b1k.f0 = f2;
  apv_b1.update_add(b1k, 1);

  r4k.f0 = f2;
  auto k4 = pv_r4.get(r4k);
  if (k4 != nullptr) {
    result += k4->annotation;
  }
}

void DynaGoxDumbbell::insert_into_b2(unsigned long f0, unsigned long f1,
                                     unsigned long f2) {
  b2k.f0 = f0;
  auto delta_pv_b2 = apv_b2.update_add_approx(b2k, 1, b);
  if (delta_pv_b2 != 0) {
    r4t.f1 = f0;
    auto tuple_r4 = r4.find(0, r4t);
    while (tuple_r4 != nullptr) {
      r4k.f0 = tuple_r4->object->f0;
      pv_r4.update_add(r4k, delta_pv_b2);

      b1k.f0 = tuple_r4->object->f0;
      auto kb1 = apv_b1.get(b1k);
      if (kb1 != nullptr) {
        result += (kb1->annotation * delta_pv_b2);
      }
      tuple_r4 = tuple_r4->next;
    }
  }
}

void DynaGoxDumbbell::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}