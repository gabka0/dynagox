#include "dynagox_line5m.h"

void DynaGoxLine5m::process(const std::string &line) {
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
}

void DynaGoxLine5m::milestone() { print(); }

inline void DynaGoxLine5m::insert_into_r1(unsigned long src,
                                          unsigned long dst) {
  r1k.f0 = dst;
  double delta_pv_r1 =
      apv_r1.update_max_approx(r1k, static_cast<double>(src), b);
  if (delta_pv_r1 != 0) {
    r2t.f0 = dst;
    auto tuple_r2 = r2.find(0, r2t);
    while (tuple_r2 != nullptr) {
      r2k.f0 = tuple_r2->object->f1;
      apv_r2.update_max(r2k, delta_pv_r1);

      r3k.f0 = tuple_r2->object->f1;
      auto k3 = apv_r3.get(r3k);
      if (k3 != nullptr) {
        result = std::max(result, delta_pv_r1 * k3->annotation);
      }
      tuple_r2 = tuple_r2->next;
    }
  }
}

inline void DynaGoxLine5m::insert_into_r2(unsigned long src,
                                          unsigned long dst) {
  r2t.f0 = src;
  r2t.f1 = dst;
  r2.update_max(r2t, 1);

  r1k.f0 = src;
  auto k1 = apv_r1.get(r1k);
  if (k1 != nullptr) {
    r2k.f0 = dst;
    apv_r2.update_max(r2k, k1->approximate);

    r3k.f0 = dst;
    auto k3 = apv_r3.get(r3k);
    if (k3 != nullptr) {
      result = std::max(result, k1->approximate * k3->annotation);
    }
  }
}

inline void DynaGoxLine5m::insert_into_r3(unsigned long src,
                                          unsigned long dst) {
  r3t.f0 = src;
  r3t.f1 = dst;
  r3.update_max(r3t, 1);

  r4k.f0 = dst;
  auto k4 = apv_r4.get(r4k);
  if (k4 != nullptr) {
    r3k.f0 = src;
    apv_r3.update_max(r3k, k4->approximate);

    r2k.f0 = src;
    auto k2 = apv_r2.get(r2k);
    if (k2 != nullptr) {
      result = std::max(result, k2->annotation * k4->approximate);
    }
  }
}

inline void DynaGoxLine5m::insert_into_r4(unsigned long src,
                                          unsigned long dst) {
  r4t.f0 = src;
  r4t.f1 = dst;
  r4.update_max(r4t, 1);

  r5k.f0 = dst;
  auto k5 = apv_r5.get(r5k);
  if (k5 != nullptr) {
    r4k.f0 = src;
    auto delta_pv_r4 = apv_r4.update_max_approx(r4k, k5->approximate, b);
    if (delta_pv_r4 != 0) {
      r3t.f1 = src;
      auto tuple_r3 = r3.find(0, r3t);
      while (tuple_r3 != nullptr) {
        r3k.f0 = tuple_r3->object->f0;
        apv_r3.update_max(r3k, delta_pv_r4);

        r2k.f0 = tuple_r3->object->f0;
        auto k2 = apv_r2.get(r2k);
        if (k2 != nullptr) {
          result = std::max(result, k2->annotation * delta_pv_r4);
        }
        tuple_r3 = tuple_r3->next;
      }
    }
  }
}

inline void DynaGoxLine5m::insert_into_r5(unsigned long src,
                                          unsigned long dst) {
  r5k.f0 = src;
  auto delta_pv_r5 = apv_r5.update_max_approx(r5k, static_cast<double>(dst), b);
  if (delta_pv_r5 != 0) {
    delta_pv_r4s.clear();
    r4t.f1 = src;
    auto tuple_r4 = r4.find(0, r4t);
    while (tuple_r4 != nullptr) {
      r4k.f0 = tuple_r4->object->f0;
      auto delta_pv_r4 = apv_r4.update_max_approx(r4k, delta_pv_r5, b);
      if (delta_pv_r4 != 0) {
        delta_pv_r4s.update_max(r4k, delta_pv_r4);
      }
      tuple_r4 = tuple_r4->next;
    }

    auto k4 = delta_pv_r4s.get_keys();
    while (k4 != nullptr) {
      r3t.f1 = k4->f0;
      auto tuple_r3 = r3.find(0, r3t);
      while (tuple_r3 != nullptr) {
        r3k.f0 = tuple_r3->object->f0;
        apv_r3.update_max(r3k, k4->annotation);

        r2k.f0 = tuple_r3->object->f0;
        auto k2 = apv_r2.get(r2k);
        if (k2 != nullptr) {
          result = std::max(result, k2->annotation * k4->annotation);
        }
        tuple_r3 = tuple_r3->next;
      }
      k4 = k4->next;
    }
  }
}

void DynaGoxLine5m::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}