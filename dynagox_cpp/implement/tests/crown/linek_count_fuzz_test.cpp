#include "crown_linek_count.h"
#include <catch2/catch_test_macros.hpp>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Pair {
  unsigned long left;
  unsigned long right;
};

struct PairHash {
  size_t operator()(const Pair &p) const {
    size_t h1 = std::hash<unsigned long>()(p.left);
    size_t h2 = std::hash<unsigned long>()(p.right);
    return h1 ^ (h2 << 1);
  }
};

struct PairEq {
  bool operator()(const Pair &a, const Pair &b) const {
    return a.left == b.left && a.right == b.right;
  }
};

class NaiveLinek {
public:
  NaiveLinek(int k, int a, int b) : k(k), a(a), b(b), rels(k + 1) {}

  void insert(int rid, unsigned long left, unsigned long right) {
    rels[rid].insert(Pair{left, right});
  }

  void erase(int rid, unsigned long left, unsigned long right) {
    rels[rid].erase(Pair{left, right});
  }

  std::unordered_map<std::string, long long> compute_counts() const {
    std::unordered_map<std::string, long long> out;
    if (k == 0) {
      return out;
    }

    std::vector<std::unordered_map<unsigned long, std::vector<unsigned long>>> index(k + 1);
    for (int i = 1; i <= k; ++i) {
      for (const auto &p : rels[i]) {
        index[i][p.left].push_back(p.right);
      }
    }

    std::vector<unsigned long> assignment(k + 2, 0);

    auto emit = [&]() {
      std::string key;
      for (int i = a; i <= b; ++i) {
        if (i != a) {
          key.push_back('|');
        }
        key += std::to_string(assignment[i]);
      }
      out[key] += 1;
    };

    std::function<void(int, unsigned long)> dfs = [&](int rel, unsigned long join_value) {
      if (rel > k) {
        emit();
        return;
      }
      auto it = index[rel].find(join_value);
      if (it == index[rel].end()) {
        return;
      }
      for (unsigned long next : it->second) {
        assignment[rel + 1] = next;
        dfs(rel + 1, next);
      }
    };

    if (k == 1) {
      for (const auto &p : rels[1]) {
        assignment[1] = p.left;
        assignment[2] = p.right;
        emit();
      }
      return out;
    }

    for (const auto &p : rels[1]) {
      assignment[1] = p.left;
      assignment[2] = p.right;
      dfs(2, p.right);
    }

    return out;
  }

private:
  int k;
  int a;
  int b;
  std::vector<std::unordered_set<Pair, PairHash, PairEq>> rels;
};

static std::unordered_map<std::string, long long> parse_counts(const std::string &text) {
  std::unordered_map<std::string, long long> out;
  std::istringstream stream(text);
  std::string line;
  while (std::getline(stream, line)) {
    if (line.empty()) {
      continue;
    }
    auto pos = line.rfind('|');
    if (pos == std::string::npos) {
      continue;
    }
    std::string key = line.substr(0, pos);
    long long value = std::stoll(line.substr(pos + 1));
    out[key] += value;
  }
  return out;
}

static std::string format_map(const std::unordered_map<std::string, long long> &m) {
  std::vector<std::pair<std::string, long long>> items(m.begin(), m.end());
  std::sort(items.begin(), items.end(),
            [](const auto &a, const auto &b) { return a.first < b.first; });
  std::ostringstream out;
  for (const auto &entry : items) {
    out << entry.first << " => " << entry.second << "\n";
  }
  return out.str();
}

static void require_counts_equal(
    const std::unordered_map<std::string, long long> &actual,
    const std::unordered_map<std::string, long long> &expected,
    const std::string &context) {
  INFO(context);
  INFO("Expected:\n" + format_map(expected));
  INFO("Actual:\n" + format_map(actual));
  CHECK(actual.size() == expected.size());
  for (const auto &entry : expected) {
    auto it = actual.find(entry.first);
    CHECK(it != actual.end());
    if (it != actual.end()) {
      CHECK(it->second == entry.second);
    }
  }
}

struct Op {
  bool insert;
  int rid;
  unsigned long left;
  unsigned long right;
};

static std::string format_ops(const std::vector<Op> &ops) {
  std::ostringstream out;
  for (size_t i = 0; i < ops.size(); ++i) {
    const auto &op = ops[i];
    out << i << ": " << (op.insert ? "+" : "-") << " r" << op.rid << " "
        << op.left << " " << op.right << "\n";
  }
  return out.str();
}

static void run_fuzz_case(int k, int a, int b, unsigned int seed, int steps,
                          int domain, int milestone_every) {
  std::ostringstream output;
  CrownLinekCount crown(output, true, k, a, b, false);
  NaiveLinek naive(k, a, b);

  std::mt19937 rng(seed);
  std::uniform_int_distribution<int> rel_dist(1, k);
  std::uniform_int_distribution<int> val_dist(1, domain);
  std::bernoulli_distribution insert_dist(0.6);
  std::vector<Op> ops;
  ops.reserve(static_cast<size_t>(steps));

  for (int step = 1; step <= steps; ++step) {
    int rid = rel_dist(rng);
    unsigned long left = static_cast<unsigned long>(val_dist(rng));
    unsigned long right = static_cast<unsigned long>(val_dist(rng));
    bool insert = insert_dist(rng);
    ops.push_back(Op{insert, rid, left, right});

    if (insert) {
      naive.insert(rid, left, right);
    } else {
      naive.erase(rid, left, right);
    }

    std::string line = std::string(insert ? "+" : "-") + "|" +
                       std::to_string(rid) + "|" + std::to_string(left) + "|" +
                       std::to_string(right);
    crown.process(line);

    if (step % milestone_every == 0) {
      crown.milestone();
      auto actual = parse_counts(output.str());
      auto expected = naive.compute_counts();
      std::ostringstream ctx;
      ctx << "seed=" << seed << " step=" << step << " k=" << k << " a=" << a
          << " b=" << b << "\n"
          << "recent ops:\n" << format_ops(ops);
      require_counts_equal(actual, expected, ctx.str());
      output.str("");
      output.clear();
    }
  }

  crown.milestone();
  auto actual = parse_counts(output.str());
  auto expected = naive.compute_counts();
  std::ostringstream ctx;
  ctx << "seed=" << seed << " step=" << steps << " k=" << k << " a=" << a
      << " b=" << b << "\n"
      << "recent ops:\n" << format_ops(ops);
  require_counts_equal(actual, expected, ctx.str());
}

TEST_CASE("CrownLinekCount: fuzz k=2 a=1 b=2") {
  run_fuzz_case(2, 1, 2, 42, 200, 5, 5);
}

TEST_CASE("CrownLinekCount: fuzz k=3 a=2 b=3") {
  run_fuzz_case(3, 2, 3, 1337, 250, 6, 7);
}

TEST_CASE("CrownLinekCount: fuzz k=4 a=1 b=3") {
  run_fuzz_case(4, 1, 3, 2026, 250, 6, 6);
}

TEST_CASE("CrownLinekCount: fuzz k=3 a=1 b=1") {
  run_fuzz_case(3, 1, 1, 99, 200, 5, 5);
}

TEST_CASE("CrownLinekCount: Qichen example k=3 a=3 b=4 gives 4") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 3, 3, 4, false);

  crown.process("+|1|1|1");
  crown.process("+|1|1|2");
  crown.process("+|1|2|1");
  crown.process("+|1|2|2");

  crown.process("+|2|1|1");
  crown.process("+|2|2|1");

  crown.process("+|3|1|1");

  crown.milestone();
  auto actual = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected{{"1|1", 4}};
  require_counts_equal(actual, expected, "Qichen running example");
}

TEST_CASE("CrownLinekCount: deletions decrease counts and remove stale keys") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 3, 3, 4, false);

  crown.process("+|1|1|1");
  crown.process("+|1|1|2");
  crown.process("+|1|2|1");
  crown.process("+|1|2|2");
  crown.process("+|2|1|1");
  crown.process("+|2|2|1");
  crown.process("+|3|1|1");

  crown.milestone();
  auto before = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected_before{{"1|1", 4}};
  require_counts_equal(before, expected_before, "before deletion");

  output.str("");
  output.clear();
  crown.process("-|1|1|1");
  crown.milestone();
  auto after_one = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected_after_one{{"1|1", 3}};
  require_counts_equal(after_one, expected_after_one, "after one deletion");

  output.str("");
  output.clear();
  crown.process("-|1|1|2");
  crown.process("-|1|2|1");
  crown.process("-|1|2|2");
  crown.milestone();
  auto after_all = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected_after_all{};
  require_counts_equal(after_all, expected_after_all,
                       "after deleting all contributing tuples");
}

TEST_CASE("CrownLinekCount: a==b single-key group-by count correctness") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 3, 3, 3, false);

  crown.process("+|1|1|1");
  crown.process("+|1|1|2");
  crown.process("+|1|2|1");
  crown.process("+|1|2|2");
  crown.process("+|2|1|1");
  crown.process("+|2|2|1");
  crown.process("+|3|1|1");

  crown.milestone();
  auto actual = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected{{"1", 4}};
  require_counts_equal(actual, expected, "a==b example");
}

TEST_CASE("CrownLinekCount: a==b internal key group-by count correctness") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 3, 2, 2, false);

  crown.process("+|1|1|1");
  crown.process("+|1|2|1");
  crown.process("+|1|3|2");

  crown.process("+|2|1|10");
  crown.process("+|2|1|11");
  crown.process("+|2|2|12");

  crown.process("+|3|10|100");
  crown.process("+|3|11|101");
  crown.process("+|3|12|102");
  crown.process("+|3|12|103");

  crown.milestone();
  auto actual = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected{
      {"1", 4}, // left count 2, right expansions 2
      {"2", 2}  // left count 1, right expansions 2
  };
  require_counts_equal(actual, expected, "a==b internal key");
}

TEST_CASE("CrownLinekCount: a==b first key group-by count correctness") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 3, 1, 1, false);

  crown.process("+|1|1|1");
  crown.process("+|1|1|2");
  crown.process("+|1|2|1");
  crown.process("+|1|2|2");
  crown.process("+|2|1|1");
  crown.process("+|2|2|1");
  crown.process("+|3|1|1");

  crown.milestone();
  auto actual = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected{{"1", 2}, {"2", 2}};
  require_counts_equal(actual, expected, "a==b first key");
}

TEST_CASE("CrownLinekCount: a==b last key group-by count correctness") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 3, 4, 4, false);

  crown.process("+|1|1|1");
  crown.process("+|1|1|2");
  crown.process("+|1|2|1");
  crown.process("+|1|2|2");
  crown.process("+|2|1|1");
  crown.process("+|2|2|1");
  crown.process("+|3|1|1");

  crown.milestone();
  auto actual = parse_counts(output.str());
  std::unordered_map<std::string, long long> expected{{"1", 4}};
  require_counts_equal(actual, expected, "a==b last key");
}

TEST_CASE("CrownLinekCount: explicit relation root for Line(4,2,3)") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 4, 2, 3, false, "relation", 2);
  NaiveLinek naive(4, 2, 3);

  const std::vector<Op> ops = {
      {true, 1, 10, 20}, {true, 1, 11, 20}, {true, 2, 20, 30},
      {true, 2, 20, 31}, {true, 3, 30, 40}, {true, 3, 31, 40},
      {true, 4, 40, 50}, {true, 4, 40, 51}};

  for (const auto &op : ops) {
    naive.insert(op.rid, op.left, op.right);
    crown.process("+" + std::string("|") + std::to_string(op.rid) + "|" +
                  std::to_string(op.left) + "|" + std::to_string(op.right));
  }

  crown.milestone();
  require_counts_equal(parse_counts(output.str()), naive.compute_counts(),
                       "explicit relation root R2");
}

TEST_CASE("CrownLinekCount: explicit variable root for Line(4,2,4)") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 4, 2, 4, false, "variable", 3);
  NaiveLinek naive(4, 2, 4);

  const std::vector<Op> ops = {
      {true, 1, 1, 2}, {true, 1, 3, 2}, {true, 2, 2, 5},
      {true, 2, 2, 6}, {true, 3, 5, 8}, {true, 3, 6, 9},
      {true, 4, 8, 10}, {true, 4, 9, 11}};

  for (const auto &op : ops) {
    naive.insert(op.rid, op.left, op.right);
    crown.process("+" + std::string("|") + std::to_string(op.rid) + "|" +
                  std::to_string(op.left) + "|" + std::to_string(op.right));
  }

  crown.milestone();
  require_counts_equal(parse_counts(output.str()), naive.compute_counts(),
                       "explicit variable root x3");
}

TEST_CASE("CrownLinekCount: explicit relation root for Line(4,2,4)") {
  std::ostringstream output;
  CrownLinekCount crown(output, true, 4, 2, 4, false, "relation", 3);
  NaiveLinek naive(4, 2, 4);

  const std::vector<Op> ops = {
      {true, 1, 1, 2}, {true, 1, 3, 2}, {true, 2, 2, 5},
      {true, 2, 2, 6}, {true, 3, 5, 8}, {true, 3, 6, 9},
      {true, 4, 8, 10}, {true, 4, 9, 11}};

  for (const auto &op : ops) {
    naive.insert(op.rid, op.left, op.right);
    crown.process("+" + std::string("|") + std::to_string(op.rid) + "|" +
                  std::to_string(op.left) + "|" + std::to_string(op.right));
  }

  crown.milestone();
  require_counts_equal(parse_counts(output.str()), naive.compute_counts(),
                       "explicit relation root R3");
}
