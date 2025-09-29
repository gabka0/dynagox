#ifndef DYNAGOX_DYNAGOX_EXAMPLE_H
#define DYNAGOX_DYNAGOX_EXAMPLE_H

#include "dynagox_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <sstream>

class DynaGoxExample : public DynaGoxBase {
  inline void insert_into_r1(unsigned long x1, unsigned long x2);
  inline void insert_into_r2(unsigned long x2, unsigned long x3);
  inline void insert_into_r3(unsigned long x3, unsigned long x4);

  inline void print(unsigned long x3, double value);

  bool delta_mode;

  Tuple1 k_r1;
  Tuple1 k_r2;
  Tuple1 k_r3;
  Tuple2 t_r1;
  Tuple2 t_r2;
  Tuple2 t_r3;
  Tuple1 t_gr2;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_r1;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>> r_r1;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_r2;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
      SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>> r_r2;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>> h_r2;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> apv_r3;
  MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>> r_r3;

  MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>> r_gr2;

public:
  explicit DynaGoxExample(std::ostream &output, bool print_result, double b, bool delta_mode)
      : DynaGoxBase(output, print_result, b), delta_mode(delta_mode) {}

  void run();
  void print_state();
  void process(const std::string &line) override;
  void milestone() override;
};


#endif
