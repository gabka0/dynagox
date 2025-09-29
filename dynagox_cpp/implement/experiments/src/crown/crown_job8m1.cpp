#include "crown_job8m1.h"

void CrownJOB8m1::process(const std::string &line) {
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

void CrownJOB8m1::milestone() { print(); }

void CrownJOB8m1::insert_into_an(unsigned long id, unsigned long person_id) {
  ank.f0 = person_id;
  auto d_an = pv_an.update_max(ank, static_cast<double>(id));
  if (d_an) {
    cik.f0 = person_id;
    auto kci = pv_ci.get(cik);
    if (kci != nullptr) {
      result = std::max(result, static_cast<double>(id) * kci->annotation);
    }
  }
}

void CrownJOB8m1::insert_into_ci(unsigned long id, unsigned long person_id,
                               unsigned long movie_id) {
  cit.f0 = person_id;
  cit.f1 = movie_id;
  ci.update_add(cit, 1);

  mck.f0 = movie_id;
  auto kmc = pv_mc.get(mck);
  if (kmc != nullptr) {
    cik.f0 = person_id;
    auto d_ci = pv_ci.update_max(cik, kmc->annotation);
    if (d_ci) {
      ank.f0 = person_id;
      auto kan = pv_an.get(ank);
      if (kan != nullptr) {
        result = std::max(result, kan->annotation * kmc->annotation);
      }
    }
  }
}

void CrownJOB8m1::insert_into_mc(unsigned long id, unsigned long movie_id) {
  mck.f0 = movie_id;
  auto d_mc = pv_mc.update_max(mck, static_cast<double>(id));
  if (d_mc) {
    cit.f1 = movie_id;
    auto tuple_ci = ci.find(0, cit);
    while (tuple_ci != nullptr) {
      cik.f0 = tuple_ci->object->f0;
      auto d_ci = pv_ci.update_max(cik, static_cast<double>(id));
      if (d_ci) {
        ank.f0 = tuple_ci->object->f0;
        auto kan = pv_an.get(ank);
        if (kan != nullptr) {
          result = std::max(result, kan->annotation * static_cast<double>(id));
        }
      }

      tuple_ci = tuple_ci->next;
    }
  }
}

void CrownJOB8m1::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}