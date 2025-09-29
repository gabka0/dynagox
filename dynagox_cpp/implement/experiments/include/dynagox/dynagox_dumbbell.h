#ifndef DYNAGOX_DYNAGOX_DUMBBELL_H
#define DYNAGOX_DYNAGOX_DUMBBELL_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class DynaGoxDumbbell : public DynaGoxBase {
private:
  inline void insert_into_r1(unsigned long src, unsigned long dst);

  inline void insert_into_r2(unsigned long src, unsigned long dst);

  inline void insert_into_r3(unsigned long src, unsigned long dst);

  inline void insert_into_r4(unsigned long src, unsigned long dst);

  inline void insert_into_r5(unsigned long src, unsigned long dst);

  inline void insert_into_r6(unsigned long src, unsigned long dst);

  inline void insert_into_r7(unsigned long src, unsigned long dst);

  inline void insert_into_b1(unsigned long f0, unsigned long f1,
                             unsigned long f2);

  inline void insert_into_b2(unsigned long f0, unsigned long f1,
                             unsigned long f2);

  inline void print();

  Tuple1 b1k;
  Tuple1 r4k;
  Tuple1 b2k;
  Tuple2 r1t;
  Tuple2 r2t;
  Tuple2 r3t;
  Tuple2 r4t;
  Tuple2 r5t;
  Tuple2 r6t;
  Tuple2 r7t;
  double result;

  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r1;

  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r2;

  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r3;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> pv_r4;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>
      r4;

  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r5;

  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r6;

  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>>
      r7;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_b1;
  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>
      apv_b2;

public:
  explicit DynaGoxDumbbell(std::ostream &output, bool print_result, double b)
      : DynaGoxBase(output, print_result, b), result(0) {}

  void process(const std::string &line) override;

  void milestone() override;
};

#endif
