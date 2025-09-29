#include "crown_line5c.h"

void CrownLine5c::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  std::string type = field;
  if (type == "+") {
    getline(stream, field, '|');
    unsigned long src = std::stoul(field);
    getline(stream, field, '|');
    unsigned long dst = std::stoul(field);

    insert_into_r1(src, dst);
    insert_into_r2(src, dst);
    insert_into_r3(src, dst);
    insert_into_r4(src, dst);
    insert_into_r5(src, dst);
  } else if (type == "-") {
    getline(stream, field, '|');
    unsigned long src = std::stoul(field);
    getline(stream, field, '|');
    unsigned long dst = std::stoul(field);

    delete_from_r1(src, dst);
    delete_from_r2(src, dst);
    delete_from_r3(src, dst);
    delete_from_r4(src, dst);
    delete_from_r5(src, dst);
  } else {
    fmt::println("Unknown type '{}' encountered in line '{}'.", type, line);
    exit(1);
  }
}

void CrownLine5c::milestone() { print(); }

void CrownLine5c::insert_into_r1(unsigned long src, unsigned long dst) {
  r1k.f0 = dst;
  pv_r1.update_add(r1k, 1);

  r2t.f0 = dst;
  auto tuple_r2 = r2.find(0, r2t);
  while (tuple_r2 != nullptr) {
    r2k.f0 = tuple_r2->object->f1;
    pv_r2.update_add(r2k, 1);

    r3k.f0 = tuple_r2->object->f1;
    auto k3 = pv_r3.get(r3k);
    if (k3 != nullptr) {
      result += (k3->annotation);
    }
    tuple_r2 = tuple_r2->next;
  }
}

void CrownLine5c::insert_into_r2(unsigned long src, unsigned long dst) {
  r2t.f0 = src;
  r2t.f1 = dst;
  r2.update_add(r2t, 1);

  r1k.f0 = src;
  auto k1 = pv_r1.get(r1k);
  if (k1 != nullptr) {
    r2k.f0 = dst;
    pv_r2.update_add(r2k, k1->annotation);

    r3k.f0 = dst;
    auto k3 = pv_r3.get(r3k);
    if (k3 != nullptr) {
      result += k1->annotation * k3->annotation;
    }
  }
}

void CrownLine5c::insert_into_r3(unsigned long src, unsigned long dst) {
  r3t.f0 = src;
  r3t.f1 = dst;
  r3.update_add(r3t, 1);

  r4k.f0 = dst;
  auto k4 = pv_r4.get(r4k);
  if (k4 != nullptr) {
    r3k.f0 = src;
    pv_r3.update_add(r3k, k4->annotation);

    r2k.f0 = src;
    auto k2 = pv_r2.get(r2k);
    if (k2 != nullptr) {
      result += (k2->annotation * k4->annotation);
    }
  }
}

void CrownLine5c::insert_into_r4(unsigned long src, unsigned long dst) {
  r4t.f0 = src;
  r4t.f1 = dst;
  r4.update_add(r4t, 1);

  r5k.f0 = dst;
  auto k5 = pv_r5.get(r5k);
  if (k5 != nullptr) {
    r4k.f0 = src;
    pv_r4.update_add(r4k, k5->annotation);

    r3t.f1 = src;
    auto tuple_r3 = r3.find(0, r3t);
    while (tuple_r3 != nullptr) {
      r3k.f0 = tuple_r3->object->f0;
      pv_r3.update_add(r3k, k5->annotation);

      r2k.f0 = tuple_r3->object->f0;
      auto k2 = pv_r2.get(r2k);
      if (k2 != nullptr) {
        result += (k2->annotation * k5->annotation);
      }
      tuple_r3 = tuple_r3->next;
    }
  }
}

void CrownLine5c::insert_into_r5(unsigned long src, unsigned long dst) {
  r5k.f0 = src;
  pv_r5.update_add(r5k, 1);

  delta_pv_r4s.clear();
  r4t.f1 = src;
  auto tuple_r4 = r4.find(0, r4t);
  while (tuple_r4 != nullptr) {
    r4k.f0 = tuple_r4->object->f0;
    pv_r4.update_add(r4k, 1);
    delta_pv_r4s.update_add(r4k, 1);
    tuple_r4 = tuple_r4->next;
  }

  auto k4 = delta_pv_r4s.get_keys();
  while (k4 != nullptr) {
    r3t.f1 = k4->f0;
    auto tuple_r3 = r3.find(0, r3t);
    while (tuple_r3 != nullptr) {
      r3k.f0 = tuple_r3->object->f0;
      pv_r3.update_add(r3k, k4->annotation);

      r2k.f0 = tuple_r3->object->f0;
      auto k2 = pv_r2.get(r2k);
      if (k2 != nullptr) {
        result += (k2->annotation * k4->annotation);
      }
      tuple_r3 = tuple_r3->next;
    }
    k4 = k4->next;
  }
}

void CrownLine5c::delete_from_r1(unsigned long src, unsigned long dst) {
  r1k.f0 = dst;
  pv_r1.update_subtract(r1k, 1);

  r2t.f0 = dst;
  auto tuple_r2 = r2.find(0, r2t);
  while (tuple_r2 != nullptr) {
    r2k.f0 = tuple_r2->object->f1;
    pv_r2.update_subtract(r2k, 1);

    r3k.f0 = tuple_r2->object->f1;
    auto k3 = pv_r3.get(r3k);
    if (k3 != nullptr) {
      result -= (k3->annotation);
    }
    tuple_r2 = tuple_r2->next;
  }
}

void CrownLine5c::delete_from_r2(unsigned long src, unsigned long dst) {
  r2t.f0 = src;
  r2t.f1 = dst;
  r2.update_subtract(r2t, 1);

  r1k.f0 = src;
  auto k1 = pv_r1.get(r1k);
  if (k1 != nullptr) {
    r2k.f0 = dst;
    pv_r2.update_subtract(r2k, k1->annotation);

    r3k.f0 = dst;
    auto k3 = pv_r3.get(r3k);
    if (k3 != nullptr) {
      result -= k1->annotation * k3->annotation;
    }
  }
}

void CrownLine5c::delete_from_r3(unsigned long src, unsigned long dst) {
  r3t.f0 = src;
  r3t.f1 = dst;
  r3.update_subtract(r3t, 1);

  r4k.f0 = dst;
  auto k4 = pv_r4.get(r4k);
  if (k4 != nullptr) {
    r3k.f0 = src;
    pv_r3.update_subtract(r3k, k4->annotation);

    r2k.f0 = src;
    auto k2 = pv_r2.get(r2k);
    if (k2 != nullptr) {
      result -= (k2->annotation * k4->annotation);
    }
  }
}

void CrownLine5c::delete_from_r4(unsigned long src, unsigned long dst) {
  r4t.f0 = src;
  r4t.f1 = dst;
  r4.update_subtract(r4t, 1);

  r5k.f0 = dst;
  auto k5 = pv_r5.get(r5k);
  if (k5 != nullptr) {
    r4k.f0 = src;
    pv_r4.update_subtract(r4k, k5->annotation);

    r3t.f1 = src;
    auto tuple_r3 = r3.find(0, r3t);
    while (tuple_r3 != nullptr) {
      r3k.f0 = tuple_r3->object->f0;
      pv_r3.update_subtract(r3k, k5->annotation);

      r2k.f0 = tuple_r3->object->f0;
      auto k2 = pv_r2.get(r2k);
      if (k2 != nullptr) {
        result -= (k2->annotation * k5->annotation);
      }
      tuple_r3 = tuple_r3->next;
    }
  }
}

void CrownLine5c::delete_from_r5(unsigned long src, unsigned long dst) {
  r5k.f0 = src;
  pv_r5.update_subtract(r5k, 1);

  delta_pv_r4s.clear();
  r4t.f1 = src;
  auto tuple_r4 = r4.find(0, r4t);
  while (tuple_r4 != nullptr) {
    r4k.f0 = tuple_r4->object->f0;
    pv_r4.update_subtract(r4k, 1);
    delta_pv_r4s.update_add(r4k, 1);
    tuple_r4 = tuple_r4->next;
  }

  auto k4 = delta_pv_r4s.get_keys();
  while (k4 != nullptr) {
    r3t.f1 = k4->f0;
    auto tuple_r3 = r3.find(0, r3t);
    while (tuple_r3 != nullptr) {
      r3k.f0 = tuple_r3->object->f0;
      pv_r3.update_subtract(r3k, k4->annotation);

      r2k.f0 = tuple_r3->object->f0;
      auto k2 = pv_r2.get(r2k);
      if (k2 != nullptr) {
        result -= (k2->annotation * k4->annotation);
      }
      tuple_r3 = tuple_r3->next;
    }
    k4 = k4->next;
  }
}

void CrownLine5c::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}