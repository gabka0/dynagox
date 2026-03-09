#include "crown_linek_count.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <unordered_set>

namespace {
inline size_t mix_hash(size_t seed, size_t value) {
  seed ^= value + 0x9e3779b97f4a7c15ULL + (seed << 6) + (seed >> 2);
  return seed;
}
} // namespace

size_t CrownLinekCount::NodeTupleKeyHash::operator()(
    const NodeTupleKey &key) const {
  size_t h = std::hash<int>()(key.node);
  h = mix_hash(h, std::hash<unsigned long>()(key.f0));
  h = mix_hash(h, std::hash<unsigned long>()(key.f1));
  return h;
}

bool CrownLinekCount::NodeTupleKeyEqual::operator()(const NodeTupleKey &lhs,
                                                    const NodeTupleKey &rhs) const {
  return lhs.node == rhs.node && lhs.f0 == rhs.f0 && lhs.f1 == rhs.f1;
}

size_t CrownLinekCount::OutputTupleKeyHash::operator()(
    const OutputTupleKey &key) const {
  size_t h = 1469598103934665603ULL;
  for (unsigned long value : key.values) {
    h = mix_hash(h, std::hash<unsigned long>()(value));
  }
  return h;
}

bool CrownLinekCount::OutputTupleKeyEqual::operator()(
    const OutputTupleKey &lhs, const OutputTupleKey &rhs) const {
  return lhs.values == rhs.values;
}

CrownLinekCount::CrownLinekCount(std::ostream &output, bool print_result, int k,
                                 int a, int b, bool raw_mode,
                                 const std::string &root_kind_param,
                                 int root_pos, bool benchmark_hash_only,
                                 bool delta_mode)
    : CrownBase(output, print_result), k(k), a(a), b(b),
      root(1), root_variable(0), root_kind(RootKind::Relation),
      boolean_query(a > b),
      mode(raw_mode ? (delta_mode ? OutputMode::DeltaRaw : OutputMode::FullRaw)
                    : (delta_mode ? OutputMode::DeltaCount
                                  : OutputMode::FullCount)),
      benchmark_hash_only(benchmark_hash_only), benchmark_last_hash(0),
      benchmark_last_rows(0) {
  if (k < 1) {
    fmt::println("Invalid k '{}'. k must be >= 1.", k);
    exit(1);
  }
  if (a < 1 || b < 1 || a > k + 1 || b > k + 1) {
    fmt::println("Invalid output range a={}, b={} for k={}.", a, b, k);
    exit(1);
  }

  if (boolean_query) {
    // Boolean queries keep full checkpoint semantics.
    mode = OutputMode::FullCount;
  }

  std::string kind = root_kind_param;
  if (kind != "auto" && kind != "relation" && kind != "variable") {
    fmt::println("Invalid linek root kind '{}'. Use auto|relation|variable.",
                 kind);
    exit(1);
  }

  if (kind == "relation") {
    if (root_pos < 1 || root_pos > k) {
      fmt::println("Invalid relation root position {} for k={}.", root_pos, k);
      exit(1);
    }
    root_kind = RootKind::Relation;
    root = root_pos;
  } else if (kind == "variable") {
    if (root_pos < 1 || root_pos > k + 1) {
      fmt::println("Invalid variable root position {} for k={}.", root_pos, k);
      exit(1);
    }
    root_kind = RootKind::Variable;
    root_variable = root_pos;
  } else {
    if (boolean_query) {
      root_kind = RootKind::Relation;
      root = (k + 1) / 2;
    } else if (a == b) {
      root_kind = RootKind::Variable;
      root_variable = a;
    } else if (b == a + 1) {
      root_kind = RootKind::Relation;
      root = a;
    } else {
      root_kind = RootKind::Variable;
      root_variable = (a + b) / 2;
    }
  }

  if (root_kind == RootKind::Relation) {
    if (root < 1 || root > k) {
      fmt::println("Invalid root {} for k={}.", root, k);
      exit(1);
    }
  } else {
    if (root_variable < 1 || root_variable > k + 1) {
      fmt::println("Invalid variable root {} for k={}.", root_variable, k);
      exit(1);
    }
  }

  // For grouped COUNT over an output interval [a..b] with a<b, force an
  // interior relation root so boundary weights are exactly pv[a-1] and pv[b].
  if ((is_full_count_mode() || is_delta_count_mode()) && !boolean_query &&
      a < b) {
    root_kind = RootKind::Relation;
    root = (a + (b - 1)) / 2;
  }

  r.resize(k + 1);
  vs.resize(k + 1);
  pv.resize(k + 1);
  parent.resize(k + 1, 0);
  parent_key_side.resize(k + 1, -1);
  children.resize(k + 1);

  vl.resize(k + 1);
  vl_projection_delta.resize(k + 1);
  old_only_by_left.resize(k + 1);
  old_only_by_right.resize(k + 1);

  for (int i = 1; i <= k; ++i) {
    r[i] = std::make_unique<RelMap>();
    vs[i] = std::make_unique<RelMap>();
    pv[i] = std::make_unique<KeyMap>();
  }

  build_tree();
}

bool CrownLinekCount::is_full_raw_mode() const {
  return mode == OutputMode::FullRaw;
}

bool CrownLinekCount::is_delta_raw_mode() const {
  return mode == OutputMode::DeltaRaw;
}

bool CrownLinekCount::is_full_count_mode() const {
  return mode == OutputMode::FullCount;
}

bool CrownLinekCount::is_delta_count_mode() const {
  return mode == OutputMode::DeltaCount;
}

void CrownLinekCount::build_tree() {
  for (int i = 1; i <= k; ++i) {
    parent[i] = 0;
    parent_key_side[i] = -1;
    children[i].clear();
  }

  if (root_kind == RootKind::Relation) {
    for (int i = 1; i <= k; ++i) {
      if (i < root) {
        parent[i] = i + 1;
        parent_key_side[i] = 1;
        children[i + 1].push_back(i);
      } else if (i > root) {
        parent[i] = i - 1;
        parent_key_side[i] = 0;
        children[i - 1].push_back(i);
      }
    }
    return;
  }

  int v = root_variable;

  if (v > 1) {
    parent[v - 1] = 0;
    parent_key_side[v - 1] = -1;
    for (int i = v - 2; i >= 1; --i) {
      parent[i] = i + 1;
      parent_key_side[i] = 1;
      children[i + 1].push_back(i);
    }
  }

  if (v <= k) {
    parent[v] = 0;
    parent_key_side[v] = -1;
    for (int i = v + 1; i <= k; ++i) {
      parent[i] = i - 1;
      parent_key_side[i] = 0;
      children[i - 1].push_back(i);
    }
  }
}

void CrownLinekCount::begin_update_delta_tracking() {
  vs_delta_by_tuple.clear();
  vs_delta_events.clear();
  witness_tuples.clear();
  raw_delta_output_buffer.clear();
  count_delta_output_buffer.clear();
  affected_group_keys.clear();
  for (int i = 1; i <= k; ++i) {
    vl_projection_delta[i].clear();
    old_only_by_left[i].clear();
    old_only_by_right[i].clear();
  }
}

void CrownLinekCount::record_vs_delta_event(int node, const Tuple2 &tuple,
                                            double old_ann,
                                            double new_ann) {
  long long old_count = to_count(old_ann);
  long long new_count = to_count(new_ann);

  NodeTupleKey key{node, tuple.f0, tuple.f1};
  auto it = vs_delta_by_tuple.find(key);
  if (it == vs_delta_by_tuple.end()) {
    VsDeltaEvent ev{};
    ev.node = node;
    ev.tuple = tuple;
    ev.old_ann = old_count;
    ev.new_ann = new_count;
    ev.delta_ann = new_count - old_count;
    ev.membership_changed = ((old_count > 0) != (new_count > 0));
    vs_delta_by_tuple.emplace(key, ev);
    return;
  }

  it->second.new_ann = new_count;
  it->second.delta_ann = it->second.new_ann - it->second.old_ann;
  it->second.membership_changed =
      ((it->second.old_ann > 0) != (it->second.new_ann > 0));
}

void CrownLinekCount::finalize_update_delta_tracking() {
  vs_delta_events.clear();
  witness_tuples.clear();

  vs_delta_events.reserve(vs_delta_by_tuple.size());
  for (const auto &entry : vs_delta_by_tuple) {
    const VsDeltaEvent &ev = entry.second;
    if (ev.old_ann == ev.new_ann) {
      continue;
    }
    vs_delta_events.push_back(ev);

    if (ev.old_ann > 0 && ev.new_ann == 0) {
      old_only_by_left[ev.node].emplace(ev.tuple.f0, ev.tuple);
      old_only_by_right[ev.node].emplace(ev.tuple.f1, ev.tuple);
    }

    if (is_vl_node(ev.node)) {
      std::string pkey = project_vl_key(ev.node, ev.tuple);
      if (!pkey.empty()) {
        long long d = (ev.new_ann > 0 ? 1 : 0) - (ev.old_ann > 0 ? 1 : 0);
        if (d != 0) {
          vl_projection_delta[ev.node][pkey] += d;
          if (vl_projection_delta[ev.node][pkey] == 0) {
            vl_projection_delta[ev.node].erase(pkey);
          }
        }
      }
    }

    if (ev.membership_changed) {
      WitnessTuple witness{};
      witness.node = ev.node;
      witness.tuple = ev.tuple;
      witness.inserted = (ev.old_ann == 0 && ev.new_ann > 0);
      witness_tuples.push_back(witness);
    }
  }
}

bool CrownLinekCount::is_vl_node(int node) const {
  if (a <= b) {
    if (a > 1 && node == a - 1) {
      return true;
    }
    if (node >= a && node <= b) {
      return true;
    }
  }

  if (root_kind == RootKind::Relation) {
    return node == root;
  }

  if (root_variable > 1 && node == root_variable - 1) {
    return true;
  }
  if (root_variable <= k && node == root_variable) {
    return true;
  }
  return false;
}

std::string CrownLinekCount::project_vl_key(int node, const Tuple2 &tuple) const {
  if (a <= b) {
    if (a > 1 && node == a - 1) {
      return std::to_string(tuple.f1);
    }
    if (node >= a && node <= b - 1) {
      return std::to_string(tuple.f0) + "|" + std::to_string(tuple.f1);
    }
    if (b <= k && node == b) {
      return std::to_string(tuple.f0);
    }
  }

  if (root_kind == RootKind::Relation && node == root) {
    return std::to_string(tuple.f0) + "|" + std::to_string(tuple.f1);
  }

  if (root_kind == RootKind::Variable) {
    if (root_variable > 1 && node == root_variable - 1) {
      return std::to_string(tuple.f0) + "|" + std::to_string(tuple.f1);
    }
    if (root_variable <= k && node == root_variable) {
      return std::to_string(tuple.f0) + "|" + std::to_string(tuple.f1);
    }
  }

  return "";
}

void CrownLinekCount::apply_vl_membership_delta(int node, const Tuple2 &tuple,
                                                 long long delta_membership) {
  if (delta_membership == 0 || !is_vl_node(node)) {
    return;
  }

  std::string key = project_vl_key(node, tuple);
  if (key.empty()) {
    return;
  }

  vl[node][key] += delta_membership;
  if (vl[node][key] == 0) {
    vl[node].erase(key);
  }
}

bool CrownLinekCount::tuple_in_vl_state(int node, const Tuple2 &tuple,
                                        EvalState state) const {
  if (!is_vl_node(node)) {
    return tuple_live_in_state(node, tuple, state);
  }

  std::string key = project_vl_key(node, tuple);
  if (key.empty()) {
    return tuple_live_in_state(node, tuple, state);
  }

  long long new_count = 0;
  auto vl_it = vl[node].find(key);
  if (vl_it != vl[node].end()) {
    new_count = vl_it->second;
  }

  if (state == EvalState::New) {
    return new_count > 0;
  }

  long long delta = 0;
  auto delta_it = vl_projection_delta[node].find(key);
  if (delta_it != vl_projection_delta[node].end()) {
    delta = delta_it->second;
  }

  long long old_count = new_count - delta;
  return old_count > 0;
}

long long CrownLinekCount::get_vs_annotation_state(int node, const Tuple2 &tuple,
                                                   EvalState state) const {
  NodeTupleKey key{node, tuple.f0, tuple.f1};
  auto it = vs_delta_by_tuple.find(key);
  if (it != vs_delta_by_tuple.end()) {
    return (state == EvalState::Old) ? it->second.old_ann : it->second.new_ann;
  }

  auto existing = vs[node]->get(tuple);
  if (existing == nullptr) {
    return 0;
  }
  return to_count(existing->annotation);
}

bool CrownLinekCount::tuple_live_in_state(int node, const Tuple2 &tuple,
                                          EvalState state) const {
  return get_vs_annotation_state(node, tuple, state) > 0;
}

void CrownLinekCount::for_each_tuple_state(
    int node, int key_side, unsigned long key_value, EvalState state,
    const std::function<void(const Tuple2 &)> &emit) const {
  Tuple2 probe{};
  if (key_side == 0) {
    probe.f0 = key_value;
  } else {
    probe.f1 = key_value;
  }

  auto it = vs[node]->find(static_cast<size_t>(key_side), probe);
  while (it != nullptr) {
    const Tuple2 &candidate = *(it->object);
    if (tuple_live_in_state(node, candidate, state) &&
        tuple_in_vl_state(node, candidate, state)) {
      emit(candidate);
    }
    it = it->next;
  }

  if (state != EvalState::Old) {
    return;
  }

  const auto &old_only_index =
      (key_side == 0) ? old_only_by_left[node] : old_only_by_right[node];
  auto range = old_only_index.equal_range(key_value);
  for (auto extra = range.first; extra != range.second; ++extra) {
    const Tuple2 &candidate = extra->second;
    if (tuple_live_in_state(node, candidate, state) &&
        tuple_in_vl_state(node, candidate, state)) {
      emit(candidate);
    }
  }
}

void CrownLinekCount::process(const std::string &line) {
  std::stringstream stream(line);
  std::string field;

  getline(stream, field, '|');
  std::string type = field;
  if (!getline(stream, field, '|')) {
    return;
  }
  int rid = 0;
  if (type == "+" || type == "-") {
    rid = std::stoi(field);
  } else {
    rid = std::stoi(field);
    type = "+";
  }
  if (!getline(stream, field, '|')) {
    return;
  }
  unsigned long left = std::stoul(field);
  if (!getline(stream, field, '|')) {
    return;
  }
  unsigned long right = std::stoul(field);

  if (rid < 1 || rid > k) {
    fmt::println("Invalid relation id '{}'. Expected 1..{}.", rid, k);
    exit(1);
  }

  bool track_delta = is_delta_raw_mode() || is_delta_count_mode();
  if (track_delta) {
    begin_update_delta_tracking();
  }

  if (type == "-") {
    delete_from_relation(rid, left, right);
  } else {
    insert_into_relation(rid, left, right);
  }

  if (!track_delta) {
    return;
  }

  finalize_update_delta_tracking();
  if (is_delta_raw_mode()) {
    run_raw_delta_enumeration();
  } else {
    run_count_delta_enumeration();
  }
}

void CrownLinekCount::materialize_full_counts(
    std::unordered_map<std::string, long long> &out) {
  out.clear();

  std::unordered_map<unsigned long, long long> left_weights;
  std::unordered_map<unsigned long, long long> right_weights;

  if (a > 1) {
    if (a < b) {
      if (!(parent[a - 1] != 0 && parent_key_side[a - 1] == 1)) {
        fmt::println("Invalid COUNT orientation at left boundary (a={}).", a);
        exit(1);
      }
      auto key_it = pv[a - 1]->get_keys();
      while (key_it != nullptr) {
        long long w = to_count(key_it->annotation);
        if (w != 0) {
          left_weights[key_it->f0] = w;
        }
        key_it = key_it->next;
      }
    } else {
      if (parent[a - 1] != 0 && parent_key_side[a - 1] == 1) {
        auto key_it = pv[a - 1]->get_keys();
        while (key_it != nullptr) {
          long long w = to_count(key_it->annotation);
          if (w != 0) {
            left_weights[key_it->f0] = w;
          }
          key_it = key_it->next;
        }
      } else {
        auto tuple_it = vs[a - 1]->get_keys();
        while (tuple_it != nullptr) {
          long long w = to_count(tuple_it->annotation);
          if (w != 0) {
            left_weights[tuple_it->f1] += w;
          }
          tuple_it = tuple_it->next;
        }
      }
    }
  }

  if (b <= k) {
    if (a < b) {
      if (!(parent[b] != 0 && parent_key_side[b] == 0)) {
        fmt::println("Invalid COUNT orientation at right boundary (b={}).", b);
        exit(1);
      }
      auto key_it = pv[b]->get_keys();
      while (key_it != nullptr) {
        long long w = to_count(key_it->annotation);
        if (w != 0) {
          right_weights[key_it->f0] = w;
        }
        key_it = key_it->next;
      }
    } else {
      if (parent[b] != 0 && parent_key_side[b] == 0) {
        auto key_it = pv[b]->get_keys();
        while (key_it != nullptr) {
          long long w = to_count(key_it->annotation);
          if (w != 0) {
            right_weights[key_it->f0] = w;
          }
          key_it = key_it->next;
        }
      } else {
        auto tuple_it = vs[b]->get_keys();
        while (tuple_it != nullptr) {
          long long w = to_count(tuple_it->annotation);
          if (w != 0) {
            right_weights[tuple_it->f0] += w;
          }
          tuple_it = tuple_it->next;
        }
      }
    }
  }

  if (a == b) {
    if (a == 1) {
      auto it = vs[1]->get_keys();
      while (it != nullptr) {
        long long w = to_count(it->annotation);
        if (w != 0) {
          out[std::to_string(it->f0)] += w;
        }
        it = it->next;
      }
    } else if (a == k + 1) {
      auto it = vs[k]->get_keys();
      while (it != nullptr) {
        long long w = to_count(it->annotation);
        if (w != 0) {
          out[std::to_string(it->f1)] += w;
        }
        it = it->next;
      }
    } else {
      std::unordered_set<unsigned long> keys;
      for (const auto &entry : left_weights) {
        keys.insert(entry.first);
      }
      for (const auto &entry : right_weights) {
        keys.insert(entry.first);
      }

      for (unsigned long key : keys) {
        auto lw_it = left_weights.find(key);
        if (lw_it == left_weights.end()) {
          continue;
        }
        auto rw_it = right_weights.find(key);
        if (rw_it == right_weights.end()) {
          continue;
        }
        long long total = lw_it->second * rw_it->second;
        if (total != 0) {
          out[std::to_string(key)] += total;
        }
      }
    }
    return;
  }

  std::vector<unsigned long> assignment(k + 2, 0);

  std::function<void(int, unsigned long)> dfs_output =
      [&](int rel_index, unsigned long join_value) {
        if (rel_index > b - 1) {
          long long lw = 1;
          if (a > 1) {
            auto it = left_weights.find(assignment[a]);
            if (it == left_weights.end()) {
              return;
            }
            lw = it->second;
          }

          long long rw = 1;
          if (b <= k) {
            auto it = right_weights.find(assignment[b]);
            if (it == right_weights.end()) {
              return;
            }
            rw = it->second;
          }

          long long total = lw * rw;
          if (total != 0) {
            accumulate_output(assignment, out, total);
          }
          return;
        }

        Tuple2 probe{};
        probe.f0 = join_value;
        auto it = vs[rel_index]->find(0, probe);
        while (it != nullptr) {
          assignment[rel_index] = it->object->f0;
          assignment[rel_index + 1] = it->object->f1;
          dfs_output(rel_index + 1, it->object->f1);
          it = it->next;
        }
      };

  auto start = vs[a]->get_keys();
  while (start != nullptr) {
    assignment[a] = start->f0;
    assignment[a + 1] = start->f1;
    dfs_output(a + 1, start->f1);
    start = start->next;
  }
}

void CrownLinekCount::milestone() {
  bool should_materialize = print_result || benchmark_hash_only;

  if (is_full_count_mode()) {
    if (boolean_query) {
      if (print_result) {
        fmt::println(output, "{}", exists_join() ? 1 : 0);
      }
      if (benchmark_hash_only) {
        benchmark_last_rows = 1;
        benchmark_last_hash = exists_join() ? 1 : 0;
      }
      return;
    }

    if (should_materialize) {
      materialize_full_counts(out_counts);
      if (print_result) {
        print_aggregated();
      }
      if (benchmark_hash_only) {
        update_benchmark_hash();
      }
    }
    return;
  }

  if (is_full_raw_mode() && print_result) {
    full_enum();
    return;
  }

  // Delta modes emit per update. Keep milestone output empty by default.
  if (benchmark_hash_only) {
    if (boolean_query) {
      benchmark_last_rows = 1;
      benchmark_last_hash = exists_join() ? 1 : 0;
    } else {
      materialize_full_counts(out_counts);
      update_benchmark_hash();
    }
  }
}

long long CrownLinekCount::to_count(double value) const {
  double rounded = std::llround(value);
  if (std::fabs(value - rounded) > 1e-9) {
    fmt::println("Non-integer annotation encountered: {}", value);
    exit(1);
  }
  return static_cast<long long>(rounded);
}

bool CrownLinekCount::exists_join() {
  if (root_kind == RootKind::Relation) {
    return vs[root]->get_keys() != nullptr;
  }

  int v = root_variable;
  if (v == 1) {
    return vs[1]->get_keys() != nullptr;
  }
  if (v == k + 1) {
    return vs[k]->get_keys() != nullptr;
  }

  auto left_it = vs[v - 1]->get_keys();
  if (left_it == nullptr || vs[v]->get_keys() == nullptr) {
    return false;
  }

  std::unordered_set<unsigned long> left_keys;
  while (left_it != nullptr) {
    left_keys.insert(left_it->f1);
    left_it = left_it->next;
  }

  auto right_it = vs[v]->get_keys();
  while (right_it != nullptr) {
    if (left_keys.find(right_it->f0) != left_keys.end()) {
      return true;
    }
    right_it = right_it->next;
  }
  return false;
}

double CrownLinekCount::compute_tuple_annotation(int node, const Tuple2 &tuple) {
  if (children[node].empty()) {
    return 1;
  }

  long double product = 1.0L;
  for (int child : children[node]) {
    Tuple1 key{};
    if (child == node - 1) {
      key.f0 = tuple.f0;
    } else {
      key.f0 = tuple.f1;
    }
    auto child_sum = pv[child]->get(key);
    if (child_sum == nullptr) {
      return 0;
    }
    product *= static_cast<long double>(to_count(child_sum->annotation));
    if (product == 0.0L) {
      return 0;
    }
  }
  double ret = static_cast<double>(product);
  to_count(ret);
  return ret;
}

void CrownLinekCount::enumerate_assignments(
    std::vector<unsigned long> &assignment, const std::function<void()> &emit) {
  if (root_kind == RootKind::Variable) {
    enumerate_with_variable_root(assignment, emit);
    return;
  }

  auto it = vs[root]->get_keys();
  while (it != nullptr) {
    assignment[root] = it->f0;
    assignment[root + 1] = it->f1;
    enumerate_left(root - 1, assignment[root], assignment, [&]() {
      enumerate_right(root + 1, assignment[root + 1], assignment, emit);
    });
    it = it->next;
  }
}

void CrownLinekCount::insert_into_relation(int rid, unsigned long left,
                                           unsigned long right) {
  Tuple2 tuple{};
  tuple.f0 = left;
  tuple.f1 = right;

  if (r[rid]->get(tuple) != nullptr) {
    return;
  }

  r[rid]->update_add(tuple, 1);

  double ann = compute_tuple_annotation(rid, tuple);
  if (ann > 0) {
    apply_vs_delta(rid, tuple, 0, ann);
  }
}

void CrownLinekCount::delete_from_relation(int rid, unsigned long left,
                                           unsigned long right) {
  Tuple2 tuple{};
  tuple.f0 = left;
  tuple.f1 = right;

  auto base = r[rid]->get(tuple);
  if (base == nullptr) {
    return;
  }

  auto vs_tuple = vs[rid]->get(tuple);
  if (vs_tuple != nullptr) {
    apply_vs_delta(rid, tuple, vs_tuple->annotation, 0);
  }

  r[rid]->update_subtract(tuple, 1);
}

void CrownLinekCount::apply_vs_delta(int node, const Tuple2 &tuple,
                                     double old_ann, double new_ann) {
  double delta = new_ann - old_ann;
  if (delta == 0) {
    return;
  }

  record_vs_delta_event(node, tuple, old_ann, new_ann);

  long long old_live = to_count(old_ann) > 0 ? 1 : 0;
  long long new_live = to_count(new_ann) > 0 ? 1 : 0;
  if (old_live != new_live) {
    apply_vl_membership_delta(node, tuple, new_live - old_live);
  }

  if (old_ann == 0) {
    Tuple2 temp = tuple;
    vs[node]->update_add(temp, new_ann);
  } else if (new_ann == 0) {
    Tuple2 temp = tuple;
    vs[node]->update_subtract(temp, old_ann);
  } else {
    Tuple2 temp = tuple;
    vs[node]->update_add(temp, delta);
  }

  propagate_pv_delta(node, tuple, delta);
}

void CrownLinekCount::propagate_pv_delta(int node, const Tuple2 &tuple,
                                         double delta) {
  if (parent[node] == 0 || delta == 0) {
    return;
  }

  Tuple1 key{};
  if (parent_key_side[node] == 0) {
    key.f0 = tuple.f0;
  } else {
    key.f0 = tuple.f1;
  }

  if (delta > 0) {
    pv[node]->update_add(key, delta);
  } else {
    pv[node]->update_subtract(key, -delta);
  }

  update_parent_for_key(parent[node], node, key.f0);
}

void CrownLinekCount::update_parent_for_key(int parent_node, int child_node,
                                            unsigned long key_value) {
  Tuple2 probe{};
  if (child_node == parent_node - 1) {
    probe.f0 = key_value;
    auto it = r[parent_node]->find(0, probe);
    while (it != nullptr) {
      Tuple2 key = *(it->object);
      double old_ann = 0;
      auto existing = vs[parent_node]->get(key);
      if (existing != nullptr) {
        old_ann = existing->annotation;
      }
      double new_ann = compute_tuple_annotation(parent_node, key);
      if (old_ann != new_ann) {
        apply_vs_delta(parent_node, key, old_ann, new_ann);
      }
      it = it->next;
    }
  } else {
    probe.f1 = key_value;
    auto it = r[parent_node]->find(1, probe);
    while (it != nullptr) {
      Tuple2 key = *(it->object);
      double old_ann = 0;
      auto existing = vs[parent_node]->get(key);
      if (existing != nullptr) {
        old_ann = existing->annotation;
      }
      double new_ann = compute_tuple_annotation(parent_node, key);
      if (old_ann != new_ann) {
        apply_vs_delta(parent_node, key, old_ann, new_ann);
      }
      it = it->next;
    }
  }
}

long long CrownLinekCount::left_weight_for_key(unsigned long value) const {
  if (a == 1) {
    return 1;
  }

  if (a < b) {
    if (!(parent[a - 1] != 0 && parent_key_side[a - 1] == 1)) {
      fmt::println("Invalid COUNT orientation at left boundary (a={}).", a);
      exit(1);
    }
    Tuple1 key{};
    key.f0 = value;
    auto it = pv[a - 1]->get(key);
    return it == nullptr ? 0 : to_count(it->annotation);
  }

  if (parent[a - 1] != 0 && parent_key_side[a - 1] == 1) {
    Tuple1 key{};
    key.f0 = value;
    auto it = pv[a - 1]->get(key);
    return it == nullptr ? 0 : to_count(it->annotation);
  }

  long long total = 0;
  Tuple2 probe{};
  probe.f1 = value;
  auto it = vs[a - 1]->find(1, probe);
  while (it != nullptr) {
    total += to_count(it->object->annotation);
    it = it->next;
  }
  return total;
}

long long CrownLinekCount::right_weight_for_key(unsigned long value) const {
  if (b == k + 1) {
    return 1;
  }

  if (a < b) {
    if (!(parent[b] != 0 && parent_key_side[b] == 0)) {
      fmt::println("Invalid COUNT orientation at right boundary (b={}).", b);
      exit(1);
    }
    Tuple1 key{};
    key.f0 = value;
    auto it = pv[b]->get(key);
    return it == nullptr ? 0 : to_count(it->annotation);
  }

  if (parent[b] != 0 && parent_key_side[b] == 0) {
    Tuple1 key{};
    key.f0 = value;
    auto it = pv[b]->get(key);
    return it == nullptr ? 0 : to_count(it->annotation);
  }

  long long total = 0;
  Tuple2 probe{};
  probe.f0 = value;
  auto it = vs[b]->find(0, probe);
  while (it != nullptr) {
    total += to_count(it->object->annotation);
    it = it->next;
  }
  return total;
}

long long CrownLinekCount::compute_current_count_for_key(const GroupKey &key) const {
  if (boolean_query || a > b) {
    return 0;
  }

  if (static_cast<int>(key.values.size()) != (b - a + 1)) {
    return 0;
  }

  if (a == b) {
    unsigned long value = key.values[0];
    long long lw = (a == 1) ? 1 : left_weight_for_key(value);
    if (lw == 0) {
      return 0;
    }
    long long rw = (b == k + 1) ? 1 : right_weight_for_key(value);
    if (rw == 0) {
      return 0;
    }
    return lw * rw;
  }

  for (int i = a; i <= b - 1; ++i) {
    Tuple2 probe{};
    probe.f0 = key.values[static_cast<size_t>(i - a)];
    probe.f1 = key.values[static_cast<size_t>(i - a + 1)];
    auto it = vs[i]->get(probe);
    if (it == nullptr || to_count(it->annotation) == 0 ||
        !tuple_in_vl_state(i, probe, EvalState::New)) {
      return 0;
    }
  }

  long long lw = (a == 1) ? 1 : left_weight_for_key(key.values.front());
  if (lw == 0) {
    return 0;
  }
  long long rw = (b == k + 1) ? 1 : right_weight_for_key(key.values.back());
  if (rw == 0) {
    return 0;
  }
  return lw * rw;
}

CrownLinekCount::OutputTupleKey CrownLinekCount::make_output_key(
    const std::vector<unsigned long> &assignment) const {
  OutputTupleKey key{};
  key.values.reserve(static_cast<size_t>(b - a + 1));
  for (int i = a; i <= b; ++i) {
    key.values.push_back(assignment[i]);
  }
  return key;
}

std::string CrownLinekCount::format_output_key(const OutputTupleKey &key) const {
  std::string out;
  for (size_t i = 0; i < key.values.size(); ++i) {
    if (i != 0) {
      out.push_back('|');
    }
    out += std::to_string(key.values[i]);
  }
  return out;
}

void CrownLinekCount::enumerate_output_keys_from_anchor(
    int anchor_node, const Tuple2 &anchor_tuple, EvalState state,
    const std::function<void(const OutputTupleKey &)> &emit) const {
  if (boolean_query || a > b) {
    return;
  }
  if (anchor_node < 1 || anchor_node > k) {
    return;
  }
  if (!tuple_live_in_state(anchor_node, anchor_tuple, state) ||
      !tuple_in_vl_state(anchor_node, anchor_tuple, state)) {
    return;
  }

  std::vector<unsigned long> assignment(k + 2, 0);
  assignment[anchor_node] = anchor_tuple.f0;
  assignment[anchor_node + 1] = anchor_tuple.f1;

  std::function<void(int, unsigned long)> enumerate_right_state;
  enumerate_right_state = [&](int rel_index, unsigned long join_value) {
    if (rel_index > k) {
      emit(make_output_key(assignment));
      return;
    }

    for_each_tuple_state(rel_index, 0, join_value, state,
                         [&](const Tuple2 &tuple) {
                           assignment[rel_index + 1] = tuple.f1;
                           enumerate_right_state(rel_index + 1, tuple.f1);
                         });
  };

  std::function<void(int, unsigned long)> enumerate_left_state;
  enumerate_left_state = [&](int rel_index, unsigned long join_value) {
    if (rel_index < 1) {
      enumerate_right_state(anchor_node + 1, assignment[anchor_node + 1]);
      return;
    }

    for_each_tuple_state(rel_index, 1, join_value, state,
                         [&](const Tuple2 &tuple) {
                           assignment[rel_index] = tuple.f0;
                           enumerate_left_state(rel_index - 1, tuple.f0);
                         });
  };

  enumerate_left_state(anchor_node - 1, assignment[anchor_node]);
}

void CrownLinekCount::emit_raw_delta_row(const OutputTupleKey &key, int sign) {
  if (!print_result) {
    return;
  }
  fmt::println(output, "{}|{}", sign > 0 ? "+" : "-", format_output_key(key));
}

void CrownLinekCount::emit_count_delta_row(const GroupKey &key, long long delta) {
  if (!print_result) {
    return;
  }
  fmt::println(output, "{}|{:+}", format_output_key(key), delta);
}

void CrownLinekCount::run_raw_delta_enumeration() {
  raw_delta_output_buffer.clear();

  std::unordered_set<OutputTupleKey, OutputTupleKeyHash, OutputTupleKeyEqual>
      candidate_keys;

  for (const WitnessTuple &witness : witness_tuples) {
    EvalState state = witness.inserted ? EvalState::New : EvalState::Old;
    enumerate_output_keys_from_anchor(
        witness.node, witness.tuple, state,
        [&](const OutputTupleKey &key) { candidate_keys.insert(key); });
  }

  for (const auto &key : candidate_keys) {
    bool old_live = (raw_live_output_keys.find(key) != raw_live_output_keys.end());
    bool new_live = (compute_current_count_for_key(key) > 0);
    if (old_live == new_live) {
      continue;
    }

    int sign = new_live ? 1 : -1;
    raw_delta_output_buffer.emplace_back(key, sign);
    if (new_live) {
      raw_live_output_keys.insert(key);
    } else {
      raw_live_output_keys.erase(key);
    }
  }

  std::sort(raw_delta_output_buffer.begin(), raw_delta_output_buffer.end(),
            [&](const auto &lhs, const auto &rhs) {
              return format_output_key(lhs.first) < format_output_key(rhs.first);
            });

  for (const auto &entry : raw_delta_output_buffer) {
    emit_raw_delta_row(entry.first, entry.second);
  }
}

void CrownLinekCount::run_count_delta_enumeration() {
  count_delta_output_buffer.clear();
  affected_group_keys.clear();

  for (const VsDeltaEvent &ev : vs_delta_events) {
    if (ev.old_ann > 0) {
      enumerate_output_keys_from_anchor(
          ev.node, ev.tuple, EvalState::Old,
          [&](const OutputTupleKey &key) { affected_group_keys.insert(key); });
    }
    if (ev.new_ann > 0) {
      enumerate_output_keys_from_anchor(
          ev.node, ev.tuple, EvalState::New,
          [&](const OutputTupleKey &key) { affected_group_keys.insert(key); });
    }
  }

  for (const auto &key : affected_group_keys) {
    long long current_count = compute_current_count_for_key(key);

    long long previous_count = 0;
    auto it = last_count.find(key);
    if (it != last_count.end()) {
      previous_count = it->second;
    }

    long long delta = current_count - previous_count;
    if (delta != 0) {
      count_delta_output_buffer.emplace_back(key, delta);
    }

    if (current_count == 0) {
      if (it != last_count.end()) {
        last_count.erase(it);
      }
    } else {
      last_count[key] = current_count;
    }
  }

  std::sort(count_delta_output_buffer.begin(), count_delta_output_buffer.end(),
            [&](const auto &lhs, const auto &rhs) {
              return format_output_key(lhs.first) < format_output_key(rhs.first);
            });

  for (const auto &entry : count_delta_output_buffer) {
    emit_count_delta_row(entry.first, entry.second);
  }
}

bool CrownLinekCount::full_enum() {
  std::vector<unsigned long> assignment(k + 2, 0);
  bool found = false;
  enumerate_assignments(assignment, [&]() {
    found = true;
    if (!boolean_query && is_full_raw_mode()) {
      std::string out;
      for (int i = a; i <= b; ++i) {
        if (i != a) {
          out.push_back('|');
        }
        out += std::to_string(assignment[i]);
      }
      fmt::println(output, "{}", out);
    }
  });
  return found;
}

void CrownLinekCount::enumerate_with_variable_root(
    std::vector<unsigned long> &assignment, const std::function<void()> &emit) {
  int v = root_variable;

  if (v == 1) {
    auto it = vs[1]->get_keys();
    while (it != nullptr) {
      assignment[1] = it->f0;
      assignment[2] = it->f1;
      enumerate_right(2, it->f1, assignment, emit);
      it = it->next;
    }
    return;
  }

  if (v == k + 1) {
    auto it = vs[k]->get_keys();
    while (it != nullptr) {
      assignment[k] = it->f0;
      assignment[k + 1] = it->f1;
      enumerate_left(k - 1, it->f0, assignment, emit);
      it = it->next;
    }
    return;
  }

  auto left_root_it = vs[v - 1]->get_keys();
  while (left_root_it != nullptr) {
    assignment[v - 1] = left_root_it->f0;
    assignment[v] = left_root_it->f1;
    enumerate_left(v - 2, left_root_it->f0, assignment, [&]() {
      Tuple2 probe{};
      probe.f0 = assignment[v];
      auto right_root_it = vs[v]->find(0, probe);
      while (right_root_it != nullptr) {
        assignment[v + 1] = right_root_it->object->f1;
        enumerate_right(v + 1, right_root_it->object->f1, assignment, emit);
        right_root_it = right_root_it->next;
      }
    });
    left_root_it = left_root_it->next;
  }
}

void CrownLinekCount::enumerate_left(int rel_index, unsigned long join_value,
                                     std::vector<unsigned long> &assignment,
                                     const std::function<void()> &emit) {
  if (rel_index < 1) {
    emit();
    return;
  }

  Tuple2 probe{};
  probe.f1 = join_value;
  auto it = vs[rel_index]->find(1, probe);
  while (it != nullptr) {
    assignment[rel_index] = it->object->f0;
    enumerate_left(rel_index - 1, it->object->f0, assignment, emit);
    it = it->next;
  }
}

void CrownLinekCount::enumerate_right(int rel_index, unsigned long join_value,
                                      std::vector<unsigned long> &assignment,
                                      const std::function<void()> &emit) {
  if (rel_index > k) {
    emit();
    return;
  }

  Tuple2 probe{};
  probe.f0 = join_value;
  auto it = vs[rel_index]->find(0, probe);
  while (it != nullptr) {
    assignment[rel_index + 1] = it->object->f1;
    enumerate_right(rel_index + 1, it->object->f1, assignment, emit);
    it = it->next;
  }
}

void CrownLinekCount::accumulate_output(
    const std::vector<unsigned long> &assignment,
    std::unordered_map<std::string, long long> &out, long long delta) {
  std::string key;
  for (int i = a; i <= b; ++i) {
    if (i != a) {
      key.push_back('|');
    }
    key += std::to_string(assignment[i]);
  }
  out[key] += delta;
  if (out[key] == 0) {
    out.erase(key);
  }
}

void CrownLinekCount::print_aggregated() {
  for (const auto &entry : out_counts) {
    fmt::println(output, "{}|{}", entry.first, entry.second);
  }
}

void CrownLinekCount::update_benchmark_hash() {
  uint64_t h = 1469598103934665603ULL;
  benchmark_last_rows = static_cast<uint64_t>(out_counts.size());
  std::vector<std::pair<std::string, long long>> items(out_counts.begin(),
                                                        out_counts.end());
  std::sort(items.begin(), items.end(),
            [](const auto &lhs, const auto &rhs) {
              return lhs.first < rhs.first;
            });
  for (const auto &entry : items) {
    for (unsigned char c : entry.first) {
      h ^= static_cast<uint64_t>(c);
      h *= 1099511628211ULL;
    }
    uint64_t v = static_cast<uint64_t>(entry.second);
    h ^= v;
    h *= 1099511628211ULL;
  }
  benchmark_last_hash = h;
}
