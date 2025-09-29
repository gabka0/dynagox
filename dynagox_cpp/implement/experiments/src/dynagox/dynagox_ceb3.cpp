#include "dynagox_ceb3.h"

void DynaGoxCEB3::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 0) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long person_id = std::stoul(field);
    insert_into_an(id, person_id);
  } else if (relation_id == 1) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long person_id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long movie_id = std::stoul(field);
    insert_into_ci(person_id, movie_id);
  } else if (relation_id == 3) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long movie_id = std::stoul(field);
    insert_into_mi(id, movie_id);
  } else if (relation_id == 4) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long movie_id = std::stoul(field);
    insert_into_mii1(id, movie_id);
    insert_into_mii2(id, movie_id);
  } else if (relation_id == 6) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long person_id = std::stoul(field);
    insert_into_pi(id, person_id);
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void DynaGoxCEB3::milestone() { print(); }

void DynaGoxCEB3::insert_into_an(unsigned long id, unsigned long person_id) {
  k_an.f0 = person_id;
  apv_an.update_add(k_an, 1);
  k_ci.f0 = person_id;
  auto g_ci = apv_ci.get(k_ci);
  if (g_ci != nullptr) {
    k_pi.f0 = person_id;
    auto g_pi = apv_pi.get(k_pi);
    if (g_pi != nullptr) {
      result += g_ci->annotation * g_pi->annotation;
    }
  }
}

void DynaGoxCEB3::insert_into_ci(unsigned long person_id, unsigned long movie_id) {
  t_ci.f0 = person_id;
  t_ci.f1 = movie_id;
  r_ci.update_add(t_ci, 1);
  k_mi.f0 = movie_id;
  auto g_mi = apv_mi.get(k_mi);
  if (g_mi != nullptr) {
    k_mii1.f0 = movie_id;
    auto g_mii1 = apv_mii1.get(k_mii1);
    if (g_mii1 != nullptr) {
      k_mii2.f0 = movie_id;
      auto g_mii2 = apv_mii2.get(k_mii2);
      if (g_mii2 != nullptr) {
        k_ci.f0 = person_id;
        apv_ci.update_add(k_ci, g_mi->approximate * g_mii1->annotation * g_mii2->annotation);
        k_an.f0 = person_id;
        auto g_an = apv_an.get(k_an);
        if (g_an != nullptr) {
          k_pi.f0 = person_id;
          auto g_pi = apv_pi.get(k_pi);
          if (g_pi != nullptr) {
            result +=
                g_an->annotation * g_mi->approximate * g_mii1->annotation * g_mii2->annotation * g_pi->annotation;
          }
        }
      }
    }
  }
}

void DynaGoxCEB3::insert_into_mi(unsigned long id, unsigned long movie_id) {
  k_mi.f0 = movie_id;
  auto d_mi = apv_mi.update_add_approx(k_mi, 1, b);
  if (d_mi != 0) {
    k_mii1.f0 = movie_id;
    auto g_mii1 = apv_mii1.get(k_mii1);
    if (g_mii1 != nullptr) {
      k_mii2.f0 = movie_id;
      auto g_mii2 = apv_mii2.get(k_mii2);
      if (g_mii2 != nullptr) {
        t_ci.f1 = movie_id;
        auto f_ci = r_ci.find(0, t_ci);
        while (f_ci != nullptr) {
          k_ci.f0 = f_ci->object->f0;
          apv_ci.update_add(k_ci, f_ci->object->annotation * d_mi * g_mii1->annotation * g_mii2->annotation);
          k_an.f0 = f_ci->object->f0;
          auto g_an = apv_an.get(k_an);
          if (g_an != nullptr) {
            k_pi.f0 = f_ci->object->f0;
            auto g_pi = apv_pi.get(k_pi);
            if (g_pi != nullptr) {
              result += g_an->annotation * f_ci->object->annotation * d_mi * g_mii1->annotation * g_mii2->annotation *
                        g_pi->annotation;
            }
          }
          f_ci = f_ci->next;
        }
      }
    }
  }
}

void DynaGoxCEB3::insert_into_mii1(unsigned long id, unsigned long movie_id) {
  k_mii1.f0 = movie_id;
  apv_mii1.update_add(k_mii1, 1);
  k_mi.f0 = movie_id;
  auto g_mi = apv_mi.get(k_mi);
  if (g_mi != nullptr) {
    k_mii2.f0 = movie_id;
    auto g_mii2 = apv_mii2.get(k_mii2);
    if (g_mii2 != nullptr) {
      t_ci.f1 = movie_id;
      auto f_ci = r_ci.find(0, t_ci);
      while (f_ci != nullptr) {
        k_ci.f0 = f_ci->object->f0;
        apv_ci.update_add(k_ci, f_ci->object->annotation * g_mi->approximate * g_mii2->annotation);
        k_an.f0 = f_ci->object->f0;
        auto g_an = apv_an.get(k_an);
        if (g_an != nullptr) {
          k_pi.f0 = f_ci->object->f0;
          auto g_pi = apv_pi.get(k_pi);
          if (g_pi != nullptr) {
            result += g_an->annotation * f_ci->object->annotation * g_mi->approximate * g_mii2->annotation * g_pi->annotation;
          }
        }
        f_ci = f_ci->next;
      }
    }
  }
}

void DynaGoxCEB3::insert_into_mii2(unsigned long id, unsigned long movie_id) {
  k_mii2.f0 = movie_id;
  apv_mii2.update_add(k_mii2, 1);
  k_mi.f0 = movie_id;
  auto g_mi = apv_mi.get(k_mi);
  if (g_mi != nullptr) {
    k_mii1.f0 = movie_id;
    auto g_mii1 = apv_mii1.get(k_mii1);
    if (g_mii1 != nullptr) {
      t_ci.f1 = movie_id;
      auto f_ci = r_ci.find(0, t_ci);
      while (f_ci != nullptr) {
        k_ci.f0 = f_ci->object->f0;
        apv_ci.update_add(k_ci, f_ci->object->annotation * g_mi->approximate * g_mii1->annotation);
        k_an.f0 = f_ci->object->f0;
        auto g_an = apv_an.get(k_an);
        if (g_an != nullptr) {
          k_pi.f0 = f_ci->object->f0;
          auto g_pi = apv_pi.get(k_pi);
          if (g_pi != nullptr) {
            result += g_an->annotation * f_ci->object->annotation * g_mi->approximate * g_mii1->annotation * g_pi->annotation;
          }
        }
        f_ci = f_ci->next;
      }
    }
  }
}

void DynaGoxCEB3::insert_into_pi(unsigned long id, unsigned long person_id) {
  k_pi.f0 = person_id;
  apv_pi.update_add(k_pi, 1);
  k_an.f0 = person_id;
  auto g_an = apv_an.get(k_an);
  if (g_an != nullptr) {
    k_ci.f0 = person_id;
    auto g_ci = apv_ci.get(k_ci);
    if (g_ci != nullptr) {
      result += g_ci->annotation * g_an->annotation;
    }
  }
}

void DynaGoxCEB3::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}