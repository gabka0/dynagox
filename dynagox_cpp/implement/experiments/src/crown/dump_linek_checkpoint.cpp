#include "crown_linek_count.h"
#include "fmt/core.h"
#include <fstream>
#include <map>
#include <sstream>
#include <string>

struct Config {
  std::string stream;
  int k = 0;
  int a = 0;
  int b = 0;
  int phase_idx = 0;
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
    } else if (arg == "--k") {
      cfg.k = std::stoi(next(arg));
    } else if (arg == "--a") {
      cfg.a = std::stoi(next(arg));
    } else if (arg == "--b") {
      cfg.b = std::stoi(next(arg));
    } else if (arg == "--phase-idx") {
      cfg.phase_idx = std::stoi(next(arg));
    } else if (arg == "--ignore-deletes") {
      cfg.ignore_deletes = true;
    } else if (arg == "--help") {
      fmt::println("Usage: dump_linek_checkpoint --stream PATH --k K --a A --b B "
                   "--phase-idx N [--ignore-deletes]");
      std::exit(0);
    } else {
      fmt::println("Unknown arg: {}", arg);
      std::exit(1);
    }
  }
  if (cfg.stream.empty() || cfg.k < 1 || cfg.a < 1 || cfg.b < 1) {
    fmt::println("Missing required args");
    std::exit(1);
  }
  return cfg;
}

static std::map<std::string, long long> parse_checkpoint_block(
    const std::string &chunk) {
  std::map<std::string, long long> out;
  std::istringstream in(chunk);
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    auto pos = line.rfind('|');
    if (pos == std::string::npos) {
      continue;
    }
    std::string key = line.substr(0, pos);
    long long cnt = std::stoll(line.substr(pos + 1));
    out[key] += cnt;
    if (out[key] == 0) {
      out.erase(key);
    }
  }
  return out;
}

int main(int argc, char **argv) {
  Config cfg = parse_args(argc, argv);

  std::ifstream in(cfg.stream);
  if (!in.is_open()) {
    fmt::println("Cannot open stream: {}", cfg.stream);
    return 1;
  }

  std::ostringstream sink;
  CrownLinekCount crown(sink, true, cfg.k, cfg.a, cfg.b, false, "auto", 0, false);

  int current_phase = 0;
  std::string line;
  std::streampos prev_pos = 0;
  while (std::getline(in, line)) {
    if (line.empty()) {
      continue;
    }
    if (line == "*") {
      crown.milestone();
      std::string buf = sink.str();
      std::string new_chunk = buf.substr(static_cast<size_t>(prev_pos));
      prev_pos = static_cast<std::streampos>(buf.size());

      if (current_phase == cfg.phase_idx) {
        auto rows = parse_checkpoint_block(new_chunk);
        for (const auto &it : rows) {
          fmt::println("{}|{}", it.first, it.second);
        }
        return 0;
      }
      ++current_phase;
      continue;
    }
    if (cfg.ignore_deletes && !line.empty() && line[0] == '-') {
      continue;
    }
    crown.process(line);
  }

  fmt::println("Phase {} not reached", cfg.phase_idx);
  return 1;
}
