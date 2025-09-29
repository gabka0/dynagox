#ifndef DYNAGOX_CROWN_CEB2_H
#define DYNAGOX_CROWN_CEB2_H

#include "crown_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class CrownCEB2 : public CrownBase {
private:
  inline void insert_into_ci(unsigned long person_id, unsigned long movie_id);
  inline void insert_into_mc(unsigned long id, unsigned long movie_id);
  inline void insert_into_mi(unsigned long id, unsigned long movie_id);
  inline void insert_into_pi(unsigned long id, unsigned long person_id);
  inline void print();

  Tuple2 t_ci;
  Tuple1 k_ci;
  Tuple1 k_mc;
  Tuple1 k_mi;
  Tuple1 k_pi;
  double result;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> pv_ci;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>> r_ci;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> pv_mc;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> pv_mi;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> pv_pi;

public:
  explicit CrownCEB2(std::ostream &output, bool print_result) : CrownBase(output, print_result), result(0) {}

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
