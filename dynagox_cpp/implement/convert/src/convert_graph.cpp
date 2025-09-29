#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/os.h"
#include <cassert>
#include <deque>
#include <fstream>
#include <getopt.h>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

std::string input;
std::string output;
int progress_enable = 0;
int window_enable = 0;
int degenerate_enable = 0;

std::random_device rd;
std::mt19937 rng(rd());

std::vector<std::tuple<unsigned long, unsigned long>> rows;

const option long_options[] = {{"input",      required_argument, 0,                  'i'},
                               {"output",     required_argument, 0,                  'o'},
                               {"progress",   no_argument,       &progress_enable,   1},
                               {"window",     no_argument,       &window_enable,     1},
                               {"degenerate", no_argument,       &degenerate_enable, 1},
                               {0, 0,                            0,                  0}};

void check() {
  if (input.empty()) {
    fmt::println("Missing input file. Please provide the input path using the "
                 "'-i' option.");
    exit(1);
  }

  if (output.empty()) {
    fmt::println("Missing output directory. Please provide the output path "
                 "using the '-o' option.");
    exit(1);
  }

  if (progress_enable && window_enable) {
    fmt::println("Invalid configuration. Options '-w' and '-p' cannot be used "
                 "together.");
    exit(1);
  }
}

void print_info() {
  fmt::println("Input: {}", input);
  fmt::println("Output: {}", output);
  fmt::println("Progress: {}", (progress_enable ? "enabled" : "disabled"));
  fmt::println("Window: {}", (window_enable ? "enabled" : "disabled"));
  fmt::println("Degenerate: {}", (degenerate_enable ? "enabled" : "disabled"));
}

void parse_args(int argc, char **argv) {
  while (true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:o:pwd", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
      case 0:
        break;
      case 'i':
        input = optarg;
        break;
      case 'o':
        output = optarg;
        if (!output.ends_with('/')) {
          output.append("/");
        }
        break;
      case 'p':
        progress_enable = 1;
        break;
      case 'w':
        window_enable = 1;
        break;
      case 'd':
        degenerate_enable = 1;
        break;
      default:
        break;
    }
  }
  check();
  print_info();
}

void read_input() {
  std::ifstream in(input);
  std::string line;

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, '|');
    unsigned long src = std::stoul(field);
    getline(stream, field, '|');
    unsigned long dst = std::stoul(field);

    rows.emplace_back(std::make_tuple(src, dst));
  }

  std::shuffle(rows.begin(), rows.end(), rng);
}

void convert_insert() {
  assert(rows.size() >= 100);
  auto out_c = fmt::output_file(output + "graph.csv");
  auto out_t = fmt::output_file(output + "graph.txt");

  std::vector<std::vector<std::string>> buckets;
  unsigned long step = rows.size() / 10;
  for (unsigned long i = 0; i < 10; ++i) {
    buckets.emplace_back(std::vector<std::string>{});
  }

  for (unsigned long i = 0; i < rows.size(); ++i) {
    auto edge = rows.at(i);
    unsigned long j = i / step;
    j = (j > 9 ? 9 : j);
    buckets.at(j).emplace_back(fmt::format("+|{}|{}", std::get<0>(edge), std::get<1>(edge)));

    out_t.print("{{\"insert\": {{\"src\": {}, \"dst\": {}}}}}\n", std::get<0>(edge), std::get<1>(edge));
  }

  for (unsigned long i = 0; i < 10; ++i) {
    for (const auto &s: buckets.at(i)) {
      out_c.print("{}\n", s);
    }

    if (progress_enable) {
      out_c.print("*\n");
    }
  }

  if (!progress_enable) {
    out_c.print("*\n");
  }

  out_c.flush();
  out_c.close();
  out_t.flush();
  out_t.close();
}

void convert_window() {
  assert(rows.size() >= 100);
  auto out_c = fmt::output_file(output + "graph.csv");
  auto out_t = fmt::output_file(output + "graph.txt");

  std::vector<std::vector<std::tuple<unsigned long, unsigned long>>> buckets;
  unsigned long step = rows.size() / 10;
  for (unsigned long i = 0; i < 10; ++i) {
    buckets.emplace_back(std::vector<std::tuple<unsigned long, unsigned long>>{});
  }

  for (unsigned long i = 0; i < rows.size(); ++i) {
    auto edge = rows.at(i);
    unsigned long j = i / step;
    j = (j > 9 ? 9 : j);
    buckets.at(j).emplace_back(edge);
  }

  for (unsigned long i = 0; i < 2; ++i) {
    for (auto e: buckets.at(i)) {
      out_c.print("+|{}|{}\n", std::get<0>(e), std::get<1>(e));
      out_t.print("{{\"insert\": {{\"src\": {}, \"dst\": {}}}}}\n", std::get<0>(e), std::get<1>(e));
    }

    out_c.print("*\n");
  }

  for (unsigned long i = 2; i < 10; ++i) {
    unsigned long j = 0;
    while (j < buckets.at(i).size()) {
      if (j < buckets.at(i - 2).size()) {
        auto e = buckets.at(i - 2).at(j);
        out_c.print("-|{}|{}\n", std::get<0>(e), std::get<1>(e));
        out_t.print("{{\"delete\": {{\"src\": {}, \"dst\": {}}}}}\n", std::get<0>(e), std::get<1>(e));
      }

      auto edge = buckets.at(i).at(j);
      out_c.print("+|{}|{}\n", std::get<0>(edge), std::get<1>(edge));
      out_t.print("{{\"insert\": {{\"src\": {}, \"dst\": {}}}}}\n", std::get<0>(edge), std::get<1>(edge));
      j++;
    }

    out_c.print("*\n");
  }

  for (unsigned long i = 8; i < 10; ++i) {
    for (const auto &e: buckets.at(i)) {
      out_c.print("-|{}|{}\n", std::get<0>(e), std::get<1>(e));
      out_t.print("{{\"delete\": {{\"src\": {}, \"dst\": {}}}}}\n", std::get<0>(e), std::get<1>(e));
    }

    out_c.print("*\n");
  }

  out_c.flush();
  out_c.close();
  out_t.flush();
  out_t.close();
}

void convert_degenerate() {
  std::ifstream in(input);
  std::string line;
  std::unordered_set<unsigned long> dsts;

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, '|');
    unsigned long src = std::stoul(field);
    getline(stream, field, '|');
    unsigned long dst = std::stoul(field);

    rows.emplace_back(std::make_tuple(src, dst));
    dsts.emplace(dst);
  }

  for (auto dst : dsts) {
    for (int i = 0; i < 50; i++) {
      rows.emplace_back(std::make_tuple(99999, dst));   // placeholder
    }
  }

  std::shuffle(rows.begin(), rows.end(), rng);

  std::unordered_map<unsigned long, unsigned long> current;
  auto out_c = fmt::output_file(output + "graph.csv");
  unsigned long size = rows.size();
  unsigned long step = size / 10 + 1;
  unsigned long next = step;
  unsigned long index = 0;

  for (auto row : rows) {
    if (index >= next) {
      out_c.print("*\n");
      next += step;
    }

    if (std::get<0>(row) == 99999) {
      auto src = std::get<1>(row);
      auto iter = current.find(src);
      if (iter == current.end()) {
        current.emplace(src, 10);
        out_c.print("+|1|{}|{}\n", src, 10);
      } else {
        auto value = iter->second;
        unsigned long new_value = static_cast<unsigned long>(ceil(1.1 * value));
        iter->second += new_value;
        out_c.print("+|1|{}|{}\n", src, new_value);
      }
    } else {
      out_c.print("+|0|{}|{}\n", std::get<0>(row), std::get<1>(row));
    }

    index += 1;
  }

  out_c.print("*\n");

  out_c.flush();
  out_c.close();
}

void convert() {
  if (!window_enable) {
    convert_insert();
  } else {
    convert_window();
  }
}

int main(int argc, char **argv) {
  parse_args(argc, argv);
  if (!degenerate_enable) {
    read_input();
    convert();
  } else {
    convert_degenerate();
  }
  return 0;
}