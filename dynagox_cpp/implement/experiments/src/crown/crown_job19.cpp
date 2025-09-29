#include "crown_job19.h"

void CrownJOB19::process(const std::string &line) {
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
  } else if (relation_id == 3) {
    getline(stream, field, '|');
    unsigned long id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long movie_id = std::stoul(field);

    insert_into_mi(id, movie_id);
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void CrownJOB19::milestone() { print(); }

void CrownJOB19::insert_into_an(unsigned long id, unsigned long person_id) {
  ank.f0 = person_id;
  pv_an.update_add(ank, 1);

  cik.f0 = person_id;
  auto kci = pv_ci.get(cik);
  if (kci != nullptr) {
    result += kci->annotation;
  }
}

void CrownJOB19::insert_into_ci(unsigned long id, unsigned long person_id,
                                unsigned long movie_id) {
  cit.f0 = person_id;
  cit.f1 = movie_id;
  ci.update_add(cit, 1);

  mck.f0 = movie_id;
  auto kmc = pv_mc.get(mck);
  if (kmc != nullptr) {
    mik.f0 = movie_id;
    auto kmi = pv_mi.get(mik);
    if (kmi != nullptr) {
      cik.f0 = person_id;
      pv_ci.update_add(cik, kmc->annotation * kmi->annotation);

      ank.f0 = person_id;
      auto kan = pv_an.get(ank);
      if (kan != nullptr) {
        result += (kan->annotation * kmc->annotation * kmi->annotation);
      }
    }
  }
}

void CrownJOB19::insert_into_mc(unsigned long id, unsigned long movie_id) {
  mck.f0 = movie_id;
  pv_mc.update_add(mck, 1);

  mik.f0 = movie_id;
  auto kmi = pv_mi.get(mik);
  if (kmi != nullptr) {
    cit.f1 = movie_id;
    auto tuple_ci = ci.find(0, cit);
    while (tuple_ci != nullptr) {
      cik.f0 = tuple_ci->object->f0;
      pv_ci.update_add(cik, tuple_ci->object->annotation * kmi->annotation);

      ank.f0 = tuple_ci->object->f0;
      auto kan = pv_an.get(ank);
      if (kan != nullptr) {
        result +=
            (kan->annotation * tuple_ci->object->annotation * kmi->annotation);
      }

      tuple_ci = tuple_ci->next;
    }
  }
}

void CrownJOB19::insert_into_mi(unsigned long id, unsigned long movie_id) {
  mik.f0 = movie_id;
  pv_mi.update_add(mik, 1);

  mck.f0 = movie_id;
  auto kmc = pv_mc.get(mck);
  if (kmc != nullptr) {
    cit.f1 = movie_id;
    auto tuple_ci = ci.find(0, cit);
    while (tuple_ci != nullptr) {
      cik.f0 = tuple_ci->object->f0;
      pv_ci.update_add(cik, tuple_ci->object->annotation * kmc->annotation);

      ank.f0 = tuple_ci->object->f0;
      auto kan = pv_an.get(ank);
      if (kan != nullptr) {
        result +=
            (kan->annotation * tuple_ci->object->annotation * kmc->annotation);
      }

      tuple_ci = tuple_ci->next;
    }
  }
}

void CrownJOB19::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}