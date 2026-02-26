#include "crown_linek_count.h"
#include "fmt/core.h"
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <random>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

struct Edge {
  unsigned long src;
  unsigned long dst;
};

struct EdgeHash {
  size_t operator()(const Edge &e) const {
    size_t h1 = std::hash<unsigned long>()(e.src);
    size_t h2 = std::hash<unsigned long>()(e.dst);
    return h1 ^ (h2 << 1);
  }
};

struct EdgeEq {
  bool operator()(const Edge &a, const Edge &b) const {
    return a.src == b.src && a.dst == b.dst;
  }
};

struct Config {
  std::string dataset;
  std::string system = "dynagox";
  int k = 3;
  int a = 2;
  int b = 3;
  int update_count = 100000;
  int milestone_every = 10000;
  uint32_t seed = 42;
  double initial_ratio = 0.5;
  std::string root_kind = "auto";
  int root_pos = 0;
  bool csv_header = false;
};

static bool starts_with(const std::string &s, const std::string &prefix) {
  return s.rfind(prefix, 0) == 0;
}

static Config parse_args(int argc, char **argv) {
  Config cfg;
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    auto next = [&](const std::string &name) {
      if (i + 1 >= argc) {
        fmt::println("Missing value for {}", name);
        std::exit(1);
      }
      return std::string(argv[++i]);
    };

    if (arg == "--dataset") {
      cfg.dataset = next(arg);
    } else if (arg == "--system") {
      cfg.system = next(arg);
    } else if (arg == "--k") {
      cfg.k = std::stoi(next(arg));
    } else if (arg == "--a") {
      cfg.a = std::stoi(next(arg));
    } else if (arg == "--b") {
      cfg.b = std::stoi(next(arg));
    } else if (arg == "--update-count") {
      cfg.update_count = std::stoi(next(arg));
    } else if (arg == "--milestone-every") {
      cfg.milestone_every = std::stoi(next(arg));
    } else if (arg == "--seed") {
      cfg.seed = static_cast<uint32_t>(std::stoul(next(arg)));
    } else if (arg == "--initial-ratio") {
      cfg.initial_ratio = std::stod(next(arg));
    } else if (arg == "--root-kind") {
      cfg.root_kind = next(arg);
    } else if (arg == "--root-pos") {
      cfg.root_pos = std::stoi(next(arg));
    } else if (arg == "--csv-header") {
      cfg.csv_header = true;
    } else if (arg == "--help") {
      fmt::println("Usage: bench_linek_count --dataset PATH [options]");
      std::exit(0);
    } else {
      fmt::println("Unknown arg: {}", arg);
      std::exit(1);
    }
  }

  if (cfg.dataset.empty()) {
    fmt::println("dataset is required");
    std::exit(1);
  }
  if (cfg.k < 1 || cfg.a < 1 || cfg.b < 1 || cfg.a > cfg.k + 1 ||
      cfg.b > cfg.k + 1) {
    fmt::println("Invalid k/a/b");
    std::exit(1);
  }
  if (cfg.update_count < 1 || cfg.milestone_every < 1) {
    fmt::println("update-count and milestone-every must be >=1");
    std::exit(1);
  }
  if (cfg.initial_ratio < 0.0 || cfg.initial_ratio > 1.0) {
    fmt::println("initial-ratio must be in [0,1]");
    std::exit(1);
  }
  return cfg;
}

static std::vector<Edge> load_snap_edges(const std::string &path) {
  std::ifstream in(path);
  if (!in.is_open()) {
    fmt::println("Cannot open dataset: {}", path);
    std::exit(1);
  }

  std::unordered_map<Edge, bool, EdgeHash, EdgeEq> seen;
  std::vector<Edge> edges;
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty() || starts_with(line, "#")) {
      continue;
    }
    std::istringstream ss(line);
    unsigned long u = 0;
    unsigned long v = 0;
    if (!(ss >> u >> v)) {
      continue;
    }
    Edge e{u, v};
    if (seen.find(e) == seen.end()) {
      seen.emplace(e, true);
      edges.push_back(e);
    }
  }
  return edges;
}

static void print_csv_row(const Config &cfg, const std::string &phase,
                          int phase_idx, int ops, double ms, uint64_t hash,
                          uint64_t rows) {
  double throughput = ms > 0.0 ? (static_cast<double>(ops) * 1000.0 / ms) : 0.0;
  fmt::println("{},{},{},{},{},{},{},{},{:.3f},{:.3f},{},{}", cfg.system,
               cfg.dataset, cfg.k, cfg.a, cfg.b, phase, phase_idx, ops, ms,
               throughput, hash, rows);
}

int main(int argc, char **argv) {
  Config cfg = parse_args(argc, argv);
  if (cfg.csv_header) {
    fmt::println(
        "system,dataset,k,a,b,phase,phase_idx,ops,time_ms,throughput,hash,rows");
  }

  std::vector<Edge> edges = load_snap_edges(cfg.dataset);
  if (edges.empty()) {
    fmt::println("No edges loaded from {}", cfg.dataset);
    return 1;
  }

  std::mt19937 rng(cfg.seed);
  std::shuffle(edges.begin(), edges.end(), rng);

  int initial_count =
      std::min(static_cast<int>(edges.size()),
               static_cast<int>(cfg.update_count * cfg.initial_ratio));
  if (initial_count < 1) {
    initial_count = std::min(static_cast<int>(edges.size()), 1);
  }

  std::ostringstream sink;
  CrownLinekCount crown(sink, false, cfg.k, cfg.a, cfg.b, false, cfg.root_kind,
                        cfg.root_pos, true);

  std::unordered_map<Edge, size_t, EdgeHash, EdgeEq> active_pos;
  std::vector<Edge> active_list;
  active_list.reserve(edges.size());

  std::uniform_int_distribution<int> rel_dist(1, cfg.k);

  auto start_initial = std::chrono::steady_clock::now();
  for (int i = 0; i < initial_count; ++i) {
    int rid = rel_dist(rng);
    const auto &e = edges[static_cast<size_t>(i)];
    std::string op = "+|" + std::to_string(rid) + "|" + std::to_string(e.src) +
                     "|" + std::to_string(e.dst);
    crown.process(op);
    if (active_pos.find(e) == active_pos.end()) {
      active_pos[e] = active_list.size();
      active_list.push_back(e);
    }
  }
  crown.milestone();
  auto end_initial = std::chrono::steady_clock::now();
  double initial_ms = std::chrono::duration<double, std::milli>(end_initial -
                                                                 start_initial)
                          .count();
  print_csv_row(cfg, "initial_load", 0, initial_count, initial_ms,
                crown.get_last_benchmark_hash(), crown.get_last_benchmark_rows());

  std::uniform_int_distribution<int> edge_idx_dist(
      0, static_cast<int>(edges.size() - 1));
  std::bernoulli_distribution insert_bias(0.5);
  int window_ops = 0;
  int phase_idx = 0;
  auto window_start = std::chrono::steady_clock::now();

  for (int i = 0; i < cfg.update_count; ++i) {
    bool do_insert = insert_bias(rng);
    if (active_list.empty()) {
      do_insert = true;
    } else if (active_list.size() == edges.size()) {
      do_insert = false;
    }

    int rid = rel_dist(rng);
    if (do_insert) {
      Edge chosen{};
      bool found = false;
      for (int tries = 0; tries < 8; ++tries) {
        const auto &cand = edges[static_cast<size_t>(edge_idx_dist(rng))];
        if (active_pos.find(cand) == active_pos.end()) {
          chosen = cand;
          found = true;
          break;
        }
      }
      if (!found) {
        for (const auto &cand : edges) {
          if (active_pos.find(cand) == active_pos.end()) {
            chosen = cand;
            found = true;
            break;
          }
        }
      }
      if (found) {
        std::string op =
            "+|" + std::to_string(rid) + "|" + std::to_string(chosen.src) +
            "|" + std::to_string(chosen.dst);
        crown.process(op);
        active_pos[chosen] = active_list.size();
        active_list.push_back(chosen);
      }
    } else {
      std::uniform_int_distribution<int> active_idx_dist(
          0, static_cast<int>(active_list.size() - 1));
      size_t idx = static_cast<size_t>(active_idx_dist(rng));
      Edge chosen = active_list[idx];
      std::string op =
          "-|" + std::to_string(rid) + "|" + std::to_string(chosen.src) + "|" +
          std::to_string(chosen.dst);
      crown.process(op);
      auto pos_it = active_pos.find(chosen);
      if (pos_it != active_pos.end()) {
        size_t last = active_list.size() - 1;
        if (idx != last) {
          Edge moved = active_list[last];
          active_list[idx] = moved;
          active_pos[moved] = idx;
        }
        active_list.pop_back();
        active_pos.erase(pos_it);
      }
    }

    ++window_ops;
    if (window_ops == cfg.milestone_every || i == cfg.update_count - 1) {
      crown.milestone();
      auto window_end = std::chrono::steady_clock::now();
      double ms =
          std::chrono::duration<double, std::milli>(window_end - window_start)
              .count();
      ++phase_idx;
      print_csv_row(cfg, "updates", phase_idx, window_ops, ms, crown.get_last_benchmark_hash(),
                    crown.get_last_benchmark_rows());
      window_ops = 0;
      window_start = std::chrono::steady_clock::now();
    }
  }

  return 0;
}
