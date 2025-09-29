#ifndef DYNAGOX_DYNAGOX_CEB4_H
#define DYNAGOX_DYNAGOX_CEB4_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class DynaGoxCEB4 : public DynaGoxBase {
private:
  inline void insert_into_an(unsigned long id, unsigned long person_id);
  inline void insert_into_ci(unsigned long person_id, unsigned long movie_id);
  inline void insert_into_mi(unsigned long id, unsigned long movie_id);
  inline void insert_into_mii1(unsigned long id, unsigned long movie_id);
  inline void insert_into_mii2(unsigned long id, unsigned long movie_id);
  inline void insert_into_mk(unsigned long id, unsigned long movie_id);
  inline void insert_into_pi(unsigned long id, unsigned long person_id);
  inline void print();

  Tuple1 k_an;
  Tuple2 t_ci;
  Tuple1 k_ci;
  Tuple1 k_mi;
  Tuple1 k_mii1;
  Tuple1 k_mii2;
  Tuple1 k_mk;
  Tuple1 k_pi;
  double result;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_an;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_ci;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>> r_ci;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_mi;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_mii1;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_mii2;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_mk;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_pi;

public:
  explicit DynaGoxCEB4(std::ostream &output, bool print_result, double b)
  : DynaGoxBase(output, print_result, b), result(0) {}

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
