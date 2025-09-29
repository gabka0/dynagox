#include "crown_ceb1.h"

void CrownCEB1::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 1) {
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

void CrownCEB1::milestone() { print(); }

void CrownCEB1::insert_into_ci(unsigned long person_id, unsigned long movie_id) {
  t_ci.f0 = person_id;
  t_ci.f1 = movie_id;
  r_ci.update_add(t_ci, 1);

  k_mi.f0 = movie_id;
  auto g_mi = pv_mi.get(k_mi);
  if (g_mi != nullptr) {
    k_ci.f0 = person_id;
    pv_ci.update_add(k_ci, g_mi->annotation);

    k_pi.f0 = person_id;
    auto g_pi = pv_pi.get(k_pi);
    if (g_pi != nullptr) {
      result += g_pi->annotation * g_mi->annotation;
    }
  }
}

void CrownCEB1::insert_into_mi(unsigned long id, unsigned long movie_id) {
  k_mi.f0 = movie_id;
  pv_mi.update_add(k_mi, 1);

  t_ci.f1 = movie_id;
  auto f_ci = r_ci.find(0, t_ci);
  while (f_ci != nullptr) {
    k_ci.f0 = f_ci->object->f0;
    pv_ci.update_add(k_ci, f_ci->object->annotation);

    k_pi.f0 = f_ci->object->f0;
    auto g_pi = pv_pi.get(k_pi);
    if (g_pi != nullptr) {
      result += g_pi->annotation * f_ci->object->annotation;
    }

    f_ci = f_ci->next;
  }
}

void CrownCEB1::insert_into_pi(unsigned long id, unsigned long person_id) {
  k_pi.f0 = person_id;
  pv_pi.update_add(k_pi, 1);

  k_ci.f0 = person_id;
  auto g_ci = pv_ci.get(k_ci);
  if (g_ci != nullptr) {
    result += g_ci->annotation;
  }
}

void CrownCEB1::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}