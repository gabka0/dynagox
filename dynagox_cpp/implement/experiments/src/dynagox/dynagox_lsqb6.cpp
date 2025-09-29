#include "dynagox_lsqb6.h"

void DynaGoxLSQB6::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  unsigned long relation_id = std::stoul(field);
  if (relation_id == 0) {
    getline(stream, field, '|');
    unsigned long person1_id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long person2_id = std::stoul(field);

    insert_into_pkp1(person1_id, person2_id);
    insert_into_pkp2(person1_id, person2_id);
  } else if (relation_id == 1) {
    getline(stream, field, '|');
    unsigned long person_id = std::stoul(field);
    getline(stream, field, '|');
    unsigned long tag_id = std::stoul(field);

    insert_into_phit(person_id, tag_id);
  } else {
    fmt::println("Unknown relation ID '{}'.", relation_id);
    exit(1);
  }
}

void DynaGoxLSQB6::milestone() { print(); }

void DynaGoxLSQB6::insert_into_pkp1(unsigned long person1_id,
                                    unsigned long person2_id) {
  pkp1k.f0 = person2_id;
  apv_pkp1.update_add(pkp1k, 1);

  pkp2k.f0 = person2_id;
  auto kpkp2 = apv_pkp2.get(pkp2k);
  if (kpkp2 != nullptr) {
    result += kpkp2->annotation;
  }
}

void DynaGoxLSQB6::insert_into_pkp2(unsigned long person1_id,
                                    unsigned long person2_id) {
  pkp2t.f0 = person1_id;
  pkp2t.f1 = person2_id;
  pkp2.update_add(pkp2t, 1);

  phitk.f0 = person2_id;
  auto kphit = apv_phit.get(phitk);
  if (kphit != nullptr) {
    pkp2k.f0 = person1_id;
    apv_pkp2.update_add(pkp2k, kphit->approximate);

    pkp1k.f0 = person1_id;
    auto kpkp1 = apv_pkp1.get(pkp1k);
    if (kpkp1 != nullptr) {
      result += kpkp1->annotation * kphit->approximate;
    }
  }
}

void DynaGoxLSQB6::insert_into_phit(unsigned long person_id,
                                    unsigned long tag_id) {
  phitk.f0 = person_id;
  auto delta_pv_phit = apv_phit.update_add_approx(phitk, 1, b);
  if (delta_pv_phit != 0) {
    pkp2t.f1 = person_id;
    auto tuple_pkp2 = pkp2.find(0, pkp2t);
    while (tuple_pkp2 != nullptr) {
      pkp2k.f0 = tuple_pkp2->object->f0;
      apv_pkp2.update_add(pkp2k, delta_pv_phit);

      pkp1k.f0 = tuple_pkp2->object->f0;
      auto kpkp1 = apv_pkp1.get(pkp1k);
      if (kpkp1 != nullptr) {
        result += kpkp1->annotation * delta_pv_phit;
      }

      tuple_pkp2 = tuple_pkp2->next;
    }
  }
}

void DynaGoxLSQB6::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}