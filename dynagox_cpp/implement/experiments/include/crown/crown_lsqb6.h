#ifndef DYNAGOX_CROWN_LSQB6_H
#define DYNAGOX_CROWN_LSQB6_H

#include "crown_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class CrownLSQB6 : public CrownBase {
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
      pv_pkp1;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      pv_pkp2;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
               SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>
      pkp2;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      pv_phit;

public:
  explicit CrownLSQB6(std::ostream &output, bool print_result)
      : CrownBase(output, print_result), result(0) {}

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
