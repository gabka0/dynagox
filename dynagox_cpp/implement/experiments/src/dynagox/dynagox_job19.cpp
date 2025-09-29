#include "dynagox_job19.h"

void DynaGoxJOB19::process(const std::string &line) {
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

void DynaGoxJOB19::milestone() { print(); }

void DynaGoxJOB19::insert_into_an(unsigned long id, unsigned long person_id) {
  ank.f0 = person_id;
  apv_an.update_add(ank, 1);

  cik.f0 = person_id;
  auto kci = apv_ci.get(cik);
  if (kci != nullptr) {
    result += kci->annotation;
  }
}

void DynaGoxJOB19::insert_into_ci(unsigned long id, unsigned long person_id,
                                  unsigned long movie_id) {
  cit.f0 = person_id;
  cit.f1 = movie_id;
  ci.update_add(cit, 1);

  mck.f0 = movie_id;
  auto kmc = apv_mc.get(mck);
  if (kmc != nullptr) {
    mik.f0 = movie_id;
    auto kmi = apv_mi.get(mik);
    if (kmi != nullptr) {
      cik.f0 = person_id;
      apv_ci.update_add(cik, kmc->approximate * kmi->approximate);

      ank.f0 = person_id;
      auto kan = apv_an.get(ank);
      if (kan != nullptr) {
        result += (kan->annotation * kmc->approximate * kmi->approximate);
      }
    }
  }
}

void DynaGoxJOB19::insert_into_mc(unsigned long id, unsigned long movie_id) {
  mck.f0 = movie_id;
  auto delta_pv_mc = apv_mc.update_add_approx(mck, 1, b);
  if (delta_pv_mc != 0) {
    mik.f0 = movie_id;
    auto kmi = apv_mi.get(mik);
    if (kmi != nullptr) {
      cit.f1 = movie_id;
      auto tuple_ci = ci.find(0, cit);
      while (tuple_ci != nullptr) {
        cik.f0 = tuple_ci->object->f0;
        apv_ci.update_add(cik, tuple_ci->object->annotation * delta_pv_mc *
                                   kmi->approximate);

        ank.f0 = tuple_ci->object->f0;
        auto kan = apv_an.get(ank);
        if (kan != nullptr) {
          result += (kan->annotation * tuple_ci->object->annotation *
                     delta_pv_mc * kmi->approximate);
        }

        tuple_ci = tuple_ci->next;
      }
    }
  }
}

void DynaGoxJOB19::insert_into_mi(unsigned long id, unsigned long movie_id) {
  mik.f0 = movie_id;
  auto delta_pv_mi = apv_mi.update_add_approx(mik, 1, b);
  if (delta_pv_mi != 0) {
    mck.f0 = movie_id;
    auto kmc = apv_mc.get(mck);
    if (kmc != nullptr) {
      cit.f1 = movie_id;
      auto tuple_ci = ci.find(0, cit);
      while (tuple_ci != nullptr) {
        cik.f0 = tuple_ci->object->f0;
        apv_ci.update_add(cik, tuple_ci->object->annotation * kmc->approximate *
                                   delta_pv_mi);

        ank.f0 = tuple_ci->object->f0;
        auto kan = apv_an.get(ank);
        if (kan != nullptr) {
          result += (kan->annotation * tuple_ci->object->annotation *
                     kmc->approximate * delta_pv_mi);
        }

        tuple_ci = tuple_ci->next;
      }
    }
  }
}

void DynaGoxJOB19::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}