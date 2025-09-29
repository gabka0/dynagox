#ifndef DYNAGOX_DYNAGOX_LINE5M_H
#define DYNAGOX_DYNAGOX_LINE5M_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class DynaGoxLine5m : public DynaGoxBase {
private:
  inline void insert_into_r1(unsigned long src, unsigned long dst);
  inline void insert_into_r2(unsigned long src, unsigned long dst);
  inline void insert_into_r3(unsigned long src, unsigned long dst);
  inline void insert_into_r4(unsigned long src, unsigned long dst);
  inline void insert_into_r5(unsigned long src, unsigned long dst);
  inline void print();

  Tuple1 r1k;
  Tuple1 r2k;
  Tuple1 r3k;
  Tuple1 r4k;
  Tuple1 r5k;
  Tuple2 r2t;
  Tuple2 r3t;
  Tuple2 r4t;
  double result;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_r1;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_r2;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
               SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r2;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_r3;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
               SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>
      r3;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_r4;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
               SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>
      r4;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_r5;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      delta_pv_r4s;

public:
  explicit DynaGoxLine5m(std::ostream &output, bool print_result, double b)
      : DynaGoxBase(output, print_result, b), result(0) {}

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
