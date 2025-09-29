#ifndef DYNAGOX_DYNAGOX_JOB8M2_H
#define DYNAGOX_DYNAGOX_JOB8M2_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class DynaGoxJOB8m2 : public DynaGoxBase {
private:
  inline void insert_into_an(unsigned long id, unsigned long person_id);
  inline void insert_into_ci(unsigned long id, unsigned long person_id,
                             unsigned long movie_id);
  inline void insert_into_mc(unsigned long id, unsigned long movie_id);
  inline void print();

  Tuple1 ank;
  Tuple1 cik;
  Tuple1 mck;
  Tuple2 cit;
  double result;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_an;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_ci;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
               SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>
      ci;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_mc;

public:
  explicit DynaGoxJOB8m2(std::ostream &output, bool print_result, double b)
      : DynaGoxBase(output, print_result, b), result(0) {}

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
