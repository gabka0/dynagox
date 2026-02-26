#include "crown_linek_count.h"
#include "fmt/core.h"
#include <chrono>
#include <fstream>
#include <sstream>
#include <string>

struct Config {
  std::string stream;
  std::string dataset_label;
  std::string system = "dynagox";
  int k = 3;
  int a = 2;
  int b = 3;
  std::string root_kind = "auto";
  int root_pos = 0;
  bool csv_header = false;
  bool ignore_deletes = false;
};

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

    if (arg == "--stream") {
      cfg.stream = next(arg);
    } else if (arg == "--dataset-label") {
      cfg.dataset_label = next(arg);
    } else if (arg == "--system") {
      cfg.system = next(arg);
    } else if (arg == "--k") {
      cfg.k = std::stoi(next(arg));
    } else if (arg == "--a") {
      cfg.a = std::stoi(next(arg));
    } else if (arg == "--b") {
      cfg.b = std::stoi(next(arg));
    } else if (arg == "--root-kind") {
      cfg.root_kind = next(arg);
    } else if (arg == "--root-pos") {
      cfg.root_pos = std::stoi(next(arg));
    } else if (arg == "--ignore-deletes") {
      cfg.ignore_deletes = true;
    } else if (arg == "--csv-header") {
      cfg.csv_header = true;
    } else if (arg == "--help") {
      fmt::println(
          "Usage: bench_linek_count_stream --stream PATH --dataset-label LABEL "
          "[options]");
      std::exit(0);
    } else {
      fmt::println("Unknown arg: {}", arg);
      std::exit(1);
    }
  }

  if (cfg.stream.empty() || cfg.dataset_label.empty()) {
    fmt::println("--stream and --dataset-label are required");
    std::exit(1);
  }
  if (cfg.k < 1 || cfg.a < 1 || cfg.b < 1 || cfg.a > cfg.k + 1 ||
      cfg.b > cfg.k + 1) {
    fmt::println("Invalid k/a/b");
    std::exit(1);
  }
  return cfg;
}

static void print_csv_row(const Config &cfg, const std::string &phase,
                          int phase_idx, int ops, double ms, uint64_t hash,
                          uint64_t rows) {
  double throughput = ms > 0.0 ? (static_cast<double>(ops) * 1000.0 / ms) : 0.0;
  fmt::println("{},{},{},{},{},{},{},{},{:.3f},{:.3f},{},{}", cfg.system,
               cfg.dataset_label, cfg.k, cfg.a, cfg.b, phase, phase_idx, ops, ms,
               throughput, hash, rows);
}

int main(int argc, char **argv) {
  Config cfg = parse_args(argc, argv);
  if (cfg.csv_header) {
    fmt::println(
        "system,dataset,k,a,b,phase,phase_idx,ops,time_ms,throughput,hash,rows");
  }

  std::ifstream in(cfg.stream);
  if (!in.is_open()) {
    fmt::println("Cannot open stream: {}", cfg.stream);
    return 1;
  }

  std::ostringstream sink;
  CrownLinekCount crown(sink, false, cfg.k, cfg.a, cfg.b, false, cfg.root_kind,
                        cfg.root_pos, true);

  int phase_idx = 0;
  int window_ops = 0;
  bool first = true;
  auto window_start = std::chrono::steady_clock::now();

  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }

    if (line == "*") {
      crown.milestone();
      auto now = std::chrono::steady_clock::now();
      double ms = std::chrono::duration<double, std::milli>(now - window_start).count();
      if (first) {
        print_csv_row(cfg, "initial_load", 0, window_ops, ms,
                      crown.get_last_benchmark_hash(), crown.get_last_benchmark_rows());
        first = false;
      } else {
        ++phase_idx;
        print_csv_row(cfg, "updates", phase_idx, window_ops, ms,
                      crown.get_last_benchmark_hash(), crown.get_last_benchmark_rows());
      }
      window_ops = 0;
      window_start = std::chrono::steady_clock::now();
      continue;
    }

    if (cfg.ignore_deletes && !line.empty() && line[0] == '-') {
      ++window_ops;
      continue;
    }

    crown.process(line);
    ++window_ops;
  }

  if (window_ops > 0) {
    crown.milestone();
    auto now = std::chrono::steady_clock::now();
    double ms = std::chrono::duration<double, std::milli>(now - window_start).count();
    ++phase_idx;
    print_csv_row(cfg, "updates", phase_idx, window_ops, ms,
                  crown.get_last_benchmark_hash(), crown.get_last_benchmark_rows());
  }

  return 0;
}
