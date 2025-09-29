#include "crown_lsqb6.h"

void CrownLSQB6::process(const std::string &line) {
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

void CrownLSQB6::milestone() { print(); }

void CrownLSQB6::insert_into_pkp1(unsigned long person1_id,
                                  unsigned long person2_id) {
  pkp1k.f0 = person2_id;
  pv_pkp1.update_add(pkp1k, 1);

  pkp2k.f0 = person2_id;
  auto kpkp2 = pv_pkp2.get(pkp2k);
  if (kpkp2 != nullptr) {
    result += kpkp2->annotation;
  }
}

void CrownLSQB6::insert_into_pkp2(unsigned long person1_id,
                                  unsigned long person2_id) {
  pkp2t.f0 = person1_id;
  pkp2t.f1 = person2_id;
  pkp2.update_add(pkp2t, 1);

  phitk.f0 = person2_id;
  auto kphit = pv_phit.get(phitk);
  if (kphit != nullptr) {
    pkp2k.f0 = person1_id;
    pv_pkp2.update_add(pkp2k, kphit->annotation);

    pkp1k.f0 = person1_id;
    auto kpkp1 = pv_pkp1.get(pkp1k);
    if (kpkp1 != nullptr) {
      result += kpkp1->annotation * kphit->annotation;
    }
  }
}

void CrownLSQB6::insert_into_phit(unsigned long person_id,
                                  unsigned long tag_id) {
  phitk.f0 = person_id;
  pv_phit.update_add(phitk, 1);

  pkp2t.f1 = person_id;
  auto tuple_pkp2 = pkp2.find(0, pkp2t);
  while (tuple_pkp2 != nullptr) {
    pkp2k.f0 = tuple_pkp2->object->f0;
    pv_pkp2.update_add(pkp2k, 1);

    pkp1k.f0 = tuple_pkp2->object->f0;
    auto kpkp1 = pv_pkp1.get(pkp1k);
    if (kpkp1 != nullptr) {
      result += kpkp1->annotation;
    }

    tuple_pkp2 = tuple_pkp2->next;
  }
}

void CrownLSQB6::print() {
  if (print_result) {
    fmt::println(output, "{:.4f}", result);
  }
}