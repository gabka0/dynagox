#ifndef DYNAGOX_DYNAGOX_LSQB6_H
#define DYNAGOX_DYNAGOX_LSQB6_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class DynaGoxLSQB6 : public DynaGoxBase {
private:
  inline void insert_into_pkp1(unsigned long person1_id,
                               unsigned long person2_id);
  inline void insert_into_pkp2(unsigned long person1_id,
                               unsigned long person2_id);
  inline void insert_into_phit(unsigned long person_id, unsigned long tag_id);
  inline void print();

  Tuple1 pkp1k;
  Tuple1 pkp2k;
  Tuple1 phitk;
  Tuple2 pkp2t;
  double result;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_pkp1;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_pkp2;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
               SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>
      pkp2;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_phit;

public:
  explicit DynaGoxLSQB6(std::ostream &output, bool print_result, double b)
      : DynaGoxBase(output, print_result, b), result(0) {}

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
