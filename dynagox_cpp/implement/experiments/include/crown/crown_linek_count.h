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
#include <unordered_set>
#include <vector>
#include <cstdint>

class CrownLinekCount : public CrownBase {
private:
  enum class OutputMode { FullRaw, DeltaRaw, FullCount, DeltaCount };
  enum class RootKind { Relation, Variable };
  enum class EvalState { Old, New };

  using RelMap =
      MultiHashMap<Tuple2, PrimaryHashIndex<Tuple2, Tuple2Hash, Tuple2Equal>,
                   SecondaryHashIndex<Tuple2, Tuple2Hash0, Tuple2Equal0>,
                   SecondaryHashIndex<Tuple2, Tuple2Hash1, Tuple2Equal1>>;
  using KeyMap =
      MultiHashMap<Tuple1, PrimaryHashIndex<Tuple1, Tuple1Hash, Tuple1Equal>>;

  struct NodeTupleKey {
    int node;
    unsigned long f0;
    unsigned long f1;
  };

  struct NodeTupleKeyHash {
    size_t operator()(const NodeTupleKey &key) const;
  };

  struct NodeTupleKeyEqual {
    bool operator()(const NodeTupleKey &lhs, const NodeTupleKey &rhs) const;
  };

  struct OutputTupleKey {
    std::vector<unsigned long> values;
  };

  struct OutputTupleKeyHash {
    size_t operator()(const OutputTupleKey &key) const;
  };

  struct OutputTupleKeyEqual {
    bool operator()(const OutputTupleKey &lhs, const OutputTupleKey &rhs) const;
  };

  using GroupKey = OutputTupleKey;

  struct VsDeltaEvent {
    int node;
    Tuple2 tuple;
    long long old_ann;
    long long new_ann;
    long long delta_ann;
    bool membership_changed;
  };

  struct WitnessTuple {
    int node;
    Tuple2 tuple;
    bool inserted;
  };

  int k;
  int a;
  int b;
  int root;
  int root_variable;
  RootKind root_kind;
  bool boolean_query;
  OutputMode mode;
  bool benchmark_hash_only;
  uint64_t benchmark_last_hash;
  uint64_t benchmark_last_rows;

  std::vector<std::unique_ptr<RelMap>> r;
  std::vector<std::unique_ptr<RelMap>> vs;
  std::vector<std::unique_ptr<KeyMap>> pv;

  std::vector<int> parent;
  std::vector<int> parent_key_side;
  std::vector<std::vector<int>> children;

  std::vector<std::unordered_map<std::string, long long>> vl;
  std::vector<std::unordered_map<std::string, long long>> vl_projection_delta;

  std::unordered_map<NodeTupleKey, VsDeltaEvent, NodeTupleKeyHash,
                     NodeTupleKeyEqual>
      vs_delta_by_tuple;
  std::vector<VsDeltaEvent> vs_delta_events;
  std::vector<WitnessTuple> witness_tuples;
  std::vector<std::unordered_multimap<unsigned long, Tuple2>> old_only_by_left;
  std::vector<std::unordered_multimap<unsigned long, Tuple2>> old_only_by_right;

  std::unordered_set<OutputTupleKey, OutputTupleKeyHash, OutputTupleKeyEqual>
      raw_live_output_keys;
  std::unordered_set<GroupKey, OutputTupleKeyHash, OutputTupleKeyEqual>
      affected_group_keys;
  std::unordered_map<GroupKey, long long, OutputTupleKeyHash, OutputTupleKeyEqual>
      last_count;
  std::vector<std::pair<OutputTupleKey, int>> raw_delta_output_buffer;
  std::vector<std::pair<GroupKey, long long>> count_delta_output_buffer;

  std::unordered_map<std::string, long long> out_counts;

  inline bool is_full_raw_mode() const;
  inline bool is_delta_raw_mode() const;
  inline bool is_full_count_mode() const;
  inline bool is_delta_count_mode() const;

  inline void begin_update_delta_tracking();
  inline void record_vs_delta_event(int node, const Tuple2 &tuple, double old_ann,
                                    double new_ann);
  inline void finalize_update_delta_tracking();

  inline bool is_vl_node(int node) const;
  inline std::string project_vl_key(int node, const Tuple2 &tuple) const;
  inline void apply_vl_membership_delta(int node, const Tuple2 &tuple,
                                        long long delta_membership);
  inline bool tuple_in_vl_state(int node, const Tuple2 &tuple,
                                EvalState state) const;

  inline long long get_vs_annotation_state(int node, const Tuple2 &tuple,
                                           EvalState state) const;
  inline bool tuple_live_in_state(int node, const Tuple2 &tuple,
                                  EvalState state) const;
  inline void for_each_tuple_state(
      int node, int key_side, unsigned long key_value, EvalState state,
      const std::function<void(const Tuple2 &)> &emit) const;

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
  inline long long left_weight_for_key(unsigned long value) const;
  inline long long right_weight_for_key(unsigned long value) const;
  inline long long compute_current_count_for_key(const GroupKey &key) const;
  inline OutputTupleKey make_output_key(
      const std::vector<unsigned long> &assignment) const;
  inline std::string format_output_key(const OutputTupleKey &key) const;
  inline void enumerate_output_keys_from_anchor(
      int anchor_node, const Tuple2 &anchor_tuple, EvalState state,
      const std::function<void(const OutputTupleKey &)> &emit) const;
  inline void run_raw_delta_enumeration();
  inline void run_count_delta_enumeration();
  inline void emit_raw_delta_row(const OutputTupleKey &key, int sign);
  inline void emit_count_delta_row(const GroupKey &key, long long delta);
  inline void materialize_full_counts(
      std::unordered_map<std::string, long long> &out);
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
  inline void update_benchmark_hash();

public:
  explicit CrownLinekCount(std::ostream &output, bool print_result, int k,
                           int a, int b, bool raw_mode,
                           const std::string &root_kind = "auto",
                           int root_pos = 0,
                           bool benchmark_hash_only = false,
                           bool delta_mode = false);

  void process(const std::string &line) override;
  void milestone() override;
  inline uint64_t get_last_benchmark_hash() const;
  inline uint64_t get_last_benchmark_rows() const;
};

inline uint64_t CrownLinekCount::get_last_benchmark_hash() const {
  return benchmark_last_hash;
}

inline uint64_t CrownLinekCount::get_last_benchmark_rows() const {
  return benchmark_last_rows;
}

#endif
