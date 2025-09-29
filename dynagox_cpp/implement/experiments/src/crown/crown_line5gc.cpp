#include "crown_line5gc.h"

void CrownLine5gc::process(const std::string &line) {
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

void CrownLine5gc::milestone() {
  if (!delta_mode) {
    auto k3 = r3.get_keys();
    while (k3 != nullptr) {
      if (k3->active) {
        r2k.f0 = k3->f0;
        auto k2 = pv_r2.get(r2k);
        r4k.f0 = k3->f1;
        auto k4 = pv_r4.get(r4k);
        print(k3->f0, k3->f1, k2->annotation * k4->annotation);
      }
      k3 = k3->next;
    }
  }
}

void CrownLine5gc::insert_into_r1(unsigned long src, unsigned long dst) {
  r1k.f0 = dst;
  pv_r1.update_add(r1k, 1);

  r2t.f0 = dst;
  auto tuple_r2 = r2.find(0, r2t);
  while (tuple_r2 != nullptr) {
    r2k.f0 = tuple_r2->object->f1;
    bool activated_pv_r2 = pv_r2.update_add_and_activate(r2k, 1);
    if (activated_pv_r2) {
      r3t.f0 = tuple_r2->object->f1;
      auto tuple_r3 = r3.find(0, r3t);
      while (tuple_r3 != nullptr) {
        r4k.f0 = tuple_r3->object->f1;
        auto k4 = pv_r4.get(r4k);
        if (k4 != nullptr) {
          r3.update_activate(*(tuple_r3->object));
          if (delta_mode) {
            r2k.f0 = tuple_r3->object->f0;
            auto k2 = pv_r2.get(r2k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k2 = pv_r2.get(r2k);
        r3t.f0 = tuple_r2->object->f1;
        auto tuple_r3 = r3.find(0, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r4k.f0 = tuple_r3->object->f1;
            auto k4 = pv_r4.get(r4k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
    tuple_r2 = tuple_r2->next;
  }
}

void CrownLine5gc::insert_into_r2(unsigned long src, unsigned long dst) {
  r2t.f0 = src;
  r2t.f1 = dst;
  r2.update_add(r2t, 1);

  r1k.f0 = src;
  auto k1 = pv_r1.get(r1k);
  if (k1 != nullptr) {
    r2k.f0 = dst;
    bool activated_pv_r2 = pv_r2.update_add_and_activate(r2k, k1->annotation);
    if (activated_pv_r2) {
      r3t.f0 = dst;
      auto tuple_r3 = r3.find(0, r3t);
      while (tuple_r3 != nullptr) {
        r4k.f0 = tuple_r3->object->f1;
        auto k4 = pv_r4.get(r4k);
        if (k4 != nullptr) {
          r3.update_activate(*(tuple_r3->object));
          if (delta_mode) {
            r2k.f0 = dst;
            auto k2 = pv_r2.get(r2k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k2 = pv_r2.get(r2k);
        r3t.f0 = dst;
        auto tuple_r3 = r3.find(0, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r4k.f0 = tuple_r3->object->f1;
            auto k4 = pv_r4.get(r4k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
  }
}

void CrownLine5gc::insert_into_r3(unsigned long src, unsigned long dst) {
  r3t.f0 = src;
  r3t.f1 = dst;

  r2k.f0 = src;
  auto k2 = pv_r2.get(r2k);
  if (k2 != nullptr) {
    r4k.f0 = dst;
    auto k4 = pv_r4.get(r4k);
    if (k4 != nullptr) {
      r3.update_add_and_activate(r3t, 1);
      if (delta_mode) {
        print(src, dst, k2->annotation * k4->annotation);
      }
    } else {
      r3.update_add(r3t, 1);
    }
  } else {
    r3.update_add(r3t, 1);
  }
}

void CrownLine5gc::insert_into_r4(unsigned long src, unsigned long dst) {
  r4t.f0 = src;
  r4t.f1 = dst;
  r4.update_add(r4t, 1);

  r5k.f0 = dst;
  auto k5 = pv_r5.get(r5k);
  if (k5 != nullptr) {
    r4k.f0 = src;
    bool activated_pv_r4 = pv_r4.update_add_and_activate(r4k, k5->annotation);
    if (activated_pv_r4) {
      r3t.f1 = src;
      auto tuple_r3 = r3.find(1, r3t);
      while (tuple_r3 != nullptr) {
        r2k.f0 = tuple_r3->object->f0;
        auto k2 = pv_r2.get(r2k);
        if (k2 != nullptr) {
          r3.update_activate(*(tuple_r3->object));
          if (delta_mode) {
            r4k.f0 = src;
            auto k4 = pv_r4.get(r4k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k4 = pv_r4.get(r4k);
        r3t.f1 = src;
        auto tuple_r3 = r3.find(1, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r2k.f0 = tuple_r3->object->f0;
            auto k2 = pv_r2.get(r2k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
  }
}

void CrownLine5gc::insert_into_r5(unsigned long src, unsigned long dst) {
  r5k.f0 = src;
  pv_r5.update_add(r5k, 1);

  r4t.f1 = src;
  auto tuple_r4 = r4.find(0, r4t);
  while (tuple_r4 != nullptr) {
    r4k.f0 = tuple_r4->object->f0;
    bool activated_pv_r4 = pv_r4.update_add_and_activate(r4k, 1);
    if (activated_pv_r4) {
      r3t.f1 = tuple_r4->object->f0;
      auto tuple_r3 = r3.find(1, r3t);
      while (tuple_r3 != nullptr) {
        r2k.f0 = tuple_r3->object->f0;
        auto k2 = pv_r2.get(r2k);
        if (k2 != nullptr) {
          r3.update_activate(*(tuple_r3->object));
          if (delta_mode) {
            r4k.f0 = tuple_r3->object->f1;
            auto k4 = pv_r4.get(r4k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k4 = pv_r4.get(r4k);
        r3t.f1 = tuple_r4->object->f0;
        auto tuple_r3 = r3.find(1, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r2k.f0 = tuple_r3->object->f0;
            auto k2 = pv_r2.get(r2k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
    tuple_r4 = tuple_r4->next;
  }
}

void CrownLine5gc::delete_from_r1(unsigned long src, unsigned long dst) {
  r1k.f0 = dst;
  pv_r1.update_subtract(r1k, 1);

  r2t.f0 = dst;
  auto tuple_r2 = r2.find(0, r2t);
  while (tuple_r2 != nullptr) {
    r2k.f0 = tuple_r2->object->f1;
    bool deactivated_pv_r2 = pv_r2.update_subtract(r2k, 1);
    if (deactivated_pv_r2) {
      r3t.f0 = tuple_r2->object->f1;
      auto tuple_r3 = r3.find(0, r3t);
      while (tuple_r3 != nullptr) {
        if (tuple_r3->object->active) {
          r3.update_deactivate(*(tuple_r3->object));
          if (delta_mode) {
            print(tuple_r3->object->f0, tuple_r3->object->f1, 0);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k2 = pv_r2.get(r2k);
        r3t.f0 = tuple_r2->object->f1;
        auto tuple_r3 = r3.find(0, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r4k.f0 = tuple_r3->object->f1;
            auto k4 = pv_r4.get(r4k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
    tuple_r2 = tuple_r2->next;
  }
}

void CrownLine5gc::delete_from_r2(unsigned long src, unsigned long dst) {
  r2t.f0 = src;
  r2t.f1 = dst;
  r2.update_subtract(r2t, 1);

  r1k.f0 = src;
  auto k1 = pv_r1.get(r1k);
  if (k1 != nullptr) {
    r2k.f0 = dst;
    bool deactivated_pv_r2 = pv_r2.update_subtract(r2k, k1->annotation);
    if (deactivated_pv_r2) {
      r3t.f0 = dst;
      auto tuple_r3 = r3.find(0, r3t);
      while (tuple_r3 != nullptr) {
        if (tuple_r3->object->active) {
          r3.update_deactivate(*(tuple_r3->object));
          if (delta_mode) {
            print(tuple_r3->object->f0, tuple_r3->object->f1, 0);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k2 = pv_r2.get(r2k);
        r3t.f0 = dst;
        auto tuple_r3 = r3.find(0, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r4k.f0 = tuple_r3->object->f1;
            auto k4 = pv_r4.get(r4k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
  }
}

void CrownLine5gc::delete_from_r3(unsigned long src, unsigned long dst) {
  r3t.f0 = src;
  r3t.f1 = dst;
  auto tuple_r3 = r3.get(r3t);
  if (tuple_r3->active) {
    if (delta_mode) {
      print(src, dst, 0);
    }
  }
  r3.update_subtract(r3t, 1);
}

void CrownLine5gc::delete_from_r4(unsigned long src, unsigned long dst) {
  r4t.f0 = src;
  r4t.f1 = dst;
  r4.update_subtract(r4t, 1);

  r5k.f0 = dst;
  auto k5 = pv_r5.get(r5k);
  if (k5 != nullptr) {
    r4k.f0 = src;
    bool deactivated_pv_r4 = pv_r4.update_subtract(r4k, k5->annotation);
    if (deactivated_pv_r4) {
      r3t.f1 = src;
      auto tuple_r3 = r3.find(1, r3t);
      while (tuple_r3 != nullptr) {
        if (tuple_r3->object->active) {
          r3.update_deactivate(*(tuple_r3->object));
          if (delta_mode) {
            print(tuple_r3->object->f0, tuple_r3->object->f1, 0);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k4 = pv_r4.get(r4k);
        r3t.f1 = src;
        auto tuple_r3 = r3.find(1, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r2k.f0 = tuple_r3->object->f0;
            auto k2 = pv_r2.get(r2k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
  }
}

void CrownLine5gc::delete_from_r5(unsigned long src, unsigned long dst) {
  r5k.f0 = src;
  pv_r5.update_subtract(r5k, 1);

  r4t.f1 = src;
  auto tuple_r4 = r4.find(0, r4t);
  while (tuple_r4 != nullptr) {
    r4k.f0 = tuple_r4->object->f0;
    bool deactivated_pv_r4 = pv_r4.update_subtract(r4k, 1);
    if (deactivated_pv_r4) {
      r3t.f1 = tuple_r4->object->f0;
      auto tuple_r3 = r3.find(1, r3t);
      while (tuple_r3 != nullptr) {
        if (tuple_r3->object->active) {
          r3.update_deactivate(*(tuple_r3->object));
          if (delta_mode) {
            print(tuple_r3->object->f0, tuple_r3->object->f1, 0);
          }
        }
        tuple_r3 = tuple_r3->next;
      }
    } else {
      if (delta_mode) {
        auto k4 = pv_r4.get(r4k);
        r3t.f1 = tuple_r4->object->f0;
        auto tuple_r3 = r3.find(1, r3t);
        while (tuple_r3 != nullptr) {
          if (tuple_r3->object->active) {
            r2k.f0 = tuple_r3->object->f0;
            auto k2 = pv_r2.get(r2k);
            print(tuple_r3->object->f0, tuple_r3->object->f1,
                  k2->annotation * k4->annotation);
          }
          tuple_r3 = tuple_r3->next;
        }
      }
    }
    tuple_r4 = tuple_r4->next;
  }
}

void CrownLine5gc::print(unsigned long src, unsigned long dst, double value) {
  if (print_result) {
    fmt::println(output, "({},{}) -> {:.4f}", src, dst, value);
  }
}