#include "crown_linek_count.h"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <unordered_set>

CrownLinekCount::CrownLinekCount(std::ostream &output, bool print_result, int k,
                                 int a, int b, bool raw_mode,
                                 const std::string &root_kind_param,
                                 int root_pos)
    : CrownBase(output, print_result), k(k), a(a), b(b),
      root(1), root_variable(0), root_kind(RootKind::Relation),
      boolean_query(a > b),
      mode(raw_mode ? OutputMode::Raw : OutputMode::Count) {
  if (k < 1) {
    fmt::println("Invalid k '{}'. k must be >= 1.", k);
    exit(1);
  }
  if (a < 1 || b < 1 || a > k + 1 || b > k + 1) {
    fmt::println("Invalid output range a={}, b={} for k={}.", a, b, k);
    exit(1);
  }

  if (boolean_query) {
    mode = OutputMode::Count;
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
  if (mode == OutputMode::Count && !boolean_query && a < b) {
    root_kind = RootKind::Relation;
    root = (a + (b - 1)) / 2;
  }

  r.resize(k + 1);
  vs.resize(k + 1);
  pv.resize(k + 1);
  parent.resize(k + 1, 0);
  parent_key_side.resize(k + 1, -1);
  children.resize(k + 1);

  for (int i = 1; i <= k; ++i) {
    r[i] = std::make_unique<RelMap>();
    vs[i] = std::make_unique<RelMap>();
    pv[i] = std::make_unique<KeyMap>();
  }

  build_tree();
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

  if (type == "-") {
    delete_from_relation(rid, left, right);
  } else {
    insert_into_relation(rid, left, right);
  }
}

void CrownLinekCount::milestone() {
  if (print_result) {
    if (mode == OutputMode::Count) {
      if (boolean_query) {
        fmt::println(output, "{}", exists_join() ? 1 : 0);
        return;
      }

      out_counts.clear();
      std::unordered_map<unsigned long, long long> left_weights;
      std::unordered_map<unsigned long, long long> right_weights;

      if (a > 1) {
        if (a < b) {
          if (!(parent[a - 1] != 0 && parent_key_side[a - 1] == 1)) {
            fmt::println("Invalid COUNT orientation at left boundary (a={}).",
                         a);
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
            fmt::println("Invalid COUNT orientation at right boundary (b={}).",
                         b);
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
              out_counts[std::to_string(it->f0)] += w;
            }
            it = it->next;
          }
        } else if (a == k + 1) {
          auto it = vs[k]->get_keys();
          while (it != nullptr) {
            long long w = to_count(it->annotation);
            if (w != 0) {
              out_counts[std::to_string(it->f1)] += w;
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
              out_counts[std::to_string(key)] += total;
            }
          }
        }
      } else {
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
                  accumulate_output(assignment, out_counts, total);
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

      print_aggregated();
      return;
    }

    if (mode == OutputMode::Raw) {
      full_enum();
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

bool CrownLinekCount::full_enum() {
  std::vector<unsigned long> assignment(k + 2, 0);
  bool found = false;
  enumerate_assignments(assignment, [&]() {
    found = true;
    if (!boolean_query && mode == OutputMode::Raw) {
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
