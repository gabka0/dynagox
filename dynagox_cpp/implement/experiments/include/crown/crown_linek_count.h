#ifndef DYNAGOX_CROWN_LINEK_COUNT_H
#define DYNAGOX_CROWN_LINEK_COUNT_H

#include "crown_base.h"
#include "fmt/core.h"
#include "fmt/ostream.h"
#include "multi_hash_map.h"
#include "tuples.h"
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class CrownLinekCount : public CrownBase {
private:
  enum class OutputMode { Raw, Count };
  enum class RootKind { Relation, Variable };

  using RelMap =
      MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
                   SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>,
                   SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>;
  using KeyMap =
      MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>;

  int k;
  int a;
  int b;
  int root;
  int root_variable;
  RootKind root_kind;
  bool boolean_query;
  OutputMode mode;

  std::vector<std::unique_ptr<RelMap>> r;
  std::vector<std::unique_ptr<RelMap>> vs;
  std::vector<std::unique_ptr<KeyMap>> pv;

  std::vector<int> parent;
  std::vector<int> parent_key_side;
  std::vector<std::vector<int>> children;

  std::unordered_map<std::string, long long> out_counts;

  inline void build_tree();
  inline void insert_into_relation(int rid, unsigned long left,
                                   unsigned long right);
  inline void delete_from_relation(int rid, unsigned long left,
                                   unsigned long right);
  inline double compute_tuple_annotation(int node, const Tuple2 &tuple);
  inline void apply_vs_delta(int node, const Tuple2 &tuple, double old_ann,
                             double new_ann);
  inline void propagate_pv_delta(int node, const Tuple2 &tuple, double delta);
  inline void update_parent_for_key(int parent_node, int child_node,
                                    unsigned long key_value);
  inline bool exists_join();
  inline long long to_count(double value) const;
  inline void enumerate_assignments(
      std::vector<unsigned long> &assignment, const std::function<void()> &emit);
  inline bool full_enum();
  inline void enumerate_with_variable_root(
      std::vector<unsigned long> &assignment, const std::function<void()> &emit);
  inline void enumerate_left(int rel_index, unsigned long join_value,
                             std::vector<unsigned long> &assignment,
                             const std::function<void()> &emit);
  inline void enumerate_right(int rel_index, unsigned long join_value,
                              std::vector<unsigned long> &assignment,
                              const std::function<void()> &emit);
  inline void accumulate_output(const std::vector<unsigned long> &assignment,
                                std::unordered_map<std::string, long long> &out,
                                long long delta);
  inline void print_aggregated();

public:
  explicit CrownLinekCount(std::ostream &output, bool print_result, int k,
                           int a, int b, bool raw_mode,
                           const std::string &root_kind = "auto",
                           int root_pos = 0);

  void process(const std::string &line) override;
  void milestone() override;
};

#endif
