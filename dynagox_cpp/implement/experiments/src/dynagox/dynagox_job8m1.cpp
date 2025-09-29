#include "dynagox_job8m1.h"

void DynaGoxJOB8m1::process(const std::string &line) {
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

void DynaGoxJOB8m1::milestone() { print(); }

void DynaGoxJOB8m1::insert_into_an(unsigned long id, unsigned long person_id) {
  ank.f0 = person_id;
  auto d_an = apv_an.update_max(ank, static_cast<double>(id));
  if (d_an) {
    cik.f0 = person_id;
    auto kci = apv_ci.get(cik);
    if (kci != nullptr) {
      result = std::max(result, static_cast<double>(id) * kci->annotation);
    }
  }
}

void DynaGoxJOB8m1::insert_into_ci(unsigned long id, unsigned long person_id,
                                 unsigned long movie_id) {
  cit.f0 = person_id;
  cit.f1 = movie_id;
  ci.update_add(cit, 1);

  mck.f0 = movie_id;
  auto kmc = apv_mc.get(mck);
  if (kmc != nullptr) {
    cik.f0 = person_id;
    auto d_ci = apv_ci.update_max(cik, kmc->approximate);
    if (d_ci) {
      ank.f0 = person_id;
      auto kan = apv_an.get(ank);
      if (kan != nullptr) {
        result = std::max(result, kan->annotation * kmc->approximate);
      }
    }
  }
}

void DynaGoxJOB8m1::insert_into_mc(unsigned long id, unsigned long movie_id) {
  mck.f0 = movie_id;
  auto d_mc = apv_mc.update_max_approx(mck, static_cast<double>(id), b);
  if (d_mc != 0) {
    cit.f1 = movie_id;
    auto tuple_ci = ci.find(0, cit);
    while (tuple_ci != nullptr) {
      cik.f0 = tuple_ci->object->f0;
      auto d_ci = apv_ci.update_max(cik, d_mc);
      if (d_ci) {
        ank.f0 = tuple_ci->object->f0;
        auto kan = apv_an.get(ank);
        if (kan != nullptr) {
          result = std::max(result, kan->annotation * d_mc);
        }
      }

      tuple_ci = tuple_ci->next;
    }
  }
}

void DynaGoxJOB8m1::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}