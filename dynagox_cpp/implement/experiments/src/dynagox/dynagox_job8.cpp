#include "dynagox_job8.h"

void DynaGoxJOB8::process(const std::string &line) {
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

    insert_into_ci(id, person_id, movie_id);
  } else if (relation_id == 2) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long movie_id = std::stoul(field);

    insert_into_mc(id, movie_id);
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void DynaGoxJOB8::milestone() { print(); }

void DynaGoxJOB8::insert_into_an(unsigned long id, unsigned long person_id) {
  ank.f0 = person_id;
  apv_an.update_add(ank, 1);

  cik.f0 = person_id;
  auto kci = apv_ci.get(cik);
  if (kci != nullptr) {
    result += kci->annotation;
  }
}

void DynaGoxJOB8::insert_into_ci(unsigned long id, unsigned long person_id,
                                 unsigned long movie_id) {
  cit.f0 = person_id;
  cit.f1 = movie_id;
  ci.update_add(cit, 1);

  mck.f0 = movie_id;
  auto kmc = apv_mc.get(mck);
  if (kmc != nullptr) {
    cik.f0 = person_id;
    apv_ci.update_add(cik, kmc->approximate);

    ank.f0 = person_id;
    auto kan = apv_an.get(ank);
    if (kan != nullptr) {
      result += kan->annotation * kmc->approximate;
    }
  }
}

void DynaGoxJOB8::insert_into_mc(unsigned long id, unsigned long movie_id) {
  mck.f0 = movie_id;
  auto delta_pv_mc = apv_mc.update_add_approx(mck, 1, b);
  if (delta_pv_mc != 0) {
    cit.f1 = movie_id;
    auto tuple_ci = ci.find(0, cit);
    while (tuple_ci != nullptr) {
      cik.f0 = tuple_ci->object->f0;
      apv_ci.update_add(cik, tuple_ci->object->annotation * delta_pv_mc);

      ank.f0 = tuple_ci->object->f0;
      auto kan = apv_an.get(ank);
      if (kan != nullptr) {
        result += kan->annotation * tuple_ci->object->annotation * delta_pv_mc;
      }

      tuple_ci = tuple_ci->next;
    }
  }
}

void DynaGoxJOB8::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}