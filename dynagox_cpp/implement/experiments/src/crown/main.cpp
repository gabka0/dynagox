#include "crown_dumbbell.h"
#include "crown_job16.h"
#include "crown_job19.h"
#include "crown_job8.h"
#include "crown_ceb1.h"
#include "crown_ceb2.h"
#include "crown_ceb3.h"
#include "crown_ceb4.h"
#include "crown_line5c.h"
#include "crown_line5cd.h"
#include "crown_line5gc.h"
#include "crown_line5m.h"
#include "crown_line5s.h"
#include "crown_lsqb6.h"
#include "crown_tpch3.h"
#include "crown_tpch5.h"
#include "crown_tpch8.h"
#include "crown_tpch9.h"
#include "crown_tpch10.h"
#include "crown_tpcds15.h"
#include "crown_tpcds19.h"
#include "crown_line5s_d.h"
#include "crown_job8s.h"
#include "crown_job8m1.h"
#include "crown_job8m2.h"
#include "fmt/core.h"
#include <getopt.h>
#include <iostream>
#include <string>

std::string input;
std::string query;
bool delta_mode;
int print_result = 0;

const option long_options[] = {{"input",        required_argument, 0,             'i'},
                               {"query",        required_argument, 0,             'q'},
                               {"algorithm",    required_argument, 0,             'a'},
                               {"mode",         required_argument, 0,             'm'},
                               {"print-result", no_argument,       &print_result, 1},
                               {"help",         no_argument,       0,             'h'},
                               {0, 0,                              0,             0}};

void parse_args(int argc, char **argv) {
  std::string mode;

  while (true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:q:a:m:ph", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
      case 0:
        break;
      case 'i':
        input = optarg;
        break;
      case 'q':
        query = optarg;
        break;
      case 'm':
        mode = optarg;
        if (mode == "delta") {
          delta_mode = true;
        } else if (mode == "full") {
          delta_mode = false;
        } else {
          fmt::println(
              "Invalid mode: '{}'. Mode must be either 'full' or 'delta'.", mode);
          exit(1);
        }
        break;
      case 'p':
        print_result = 1;
        break;
      default:
        break;
    }
  }
}

void execute(CrownBase &crown) {
  std::ifstream in(input);
  std::string line;

  auto start = std::chrono::system_clock::now();
  while (getline(in, line)) {
    if (line.starts_with("*")) {
      crown.milestone();
      auto current = std::chrono::system_clock::now();
      Utils::print_execution_time(std::cout, start, current);
      Utils::print_memory_usage(std::cout);
      continue;
    } else {
      crown.process(line);
    }
  }
}

void run() {
  fmt::println("Query: {}", query);
  fmt::println("Algorithm: {}", "Crown");
  if (query == "line5c") {
    auto line5c = CrownLine5c(std::cout, print_result);
    execute(line5c);
  } else if (query == "line5s") {
    auto line5s = CrownLine5s(std::cout, print_result);
    execute(line5s);
  } else if (query == "line5m") {
    auto line5m = CrownLine5m(std::cout, print_result);
    execute(line5m);
  } else if (query == "line5cd") {
    auto line5cd = CrownLine5cd(std::cout, print_result);
    execute(line5cd);
  } else if (query == "line5gc") {
    auto line5gc = CrownLine5gc(std::cout, print_result, delta_mode);
    execute(line5gc);
  } else if (query == "dumbbell") {
    auto dumbbell = CrownDumbbell(std::cout, print_result);
    execute(dumbbell);
  } else if (query == "lsqb6") {
    auto lsqb6 = CrownLSQB6(std::cout, print_result);
    execute(lsqb6);
  } else if (query == "job8") {
    auto job8 = CrownJOB8(std::cout, print_result);
    execute(job8);
  } else if (query == "job16") {
    auto job16 = CrownJOB16(std::cout, print_result);
    execute(job16);
  } else if (query == "job19") {
    auto job19 = CrownJOB19(std::cout, print_result);
    execute(job19);
  } else if (query == "ceb1") {
    auto ceb1 = CrownCEB1(std::cout, print_result);
    execute(ceb1);
  } else if (query == "ceb2") {
    auto ceb2 = CrownCEB2(std::cout, print_result);
    execute(ceb2);
  } else if (query == "ceb3") {
    auto ceb3 = CrownCEB3(std::cout, print_result);
    execute(ceb3);
  } else if (query == "ceb4") {
    auto ceb4 = CrownCEB4(std::cout, print_result);
    execute(ceb4);
  } else if (query == "tpch3") {
    auto tpch3 = CrownTPCH3(std::cout, print_result, delta_mode);
    execute(tpch3);
  } else if (query == "tpch5") {
    auto tpch5 = CrownTPCH5(std::cout, print_result, delta_mode);
    execute(tpch5);
  } else if (query == "tpch8") {
    auto tpch8 = CrownTPCH8(std::cout, print_result, delta_mode);
    execute(tpch8);
  } else if (query == "tpch9") {
    auto tpch9 = CrownTPCH9(std::cout, print_result, delta_mode);
    execute(tpch9);
  } else if (query == "tpch10") {
    auto tpch10 = CrownTPCH10(std::cout, print_result, delta_mode);
    execute(tpch10);
  } else if (query == "tpcds15") {
    auto tpcds15 = CrownTPCDS15(std::cout, print_result, delta_mode);
    execute(tpcds15);
  } else if (query == "tpcds19") {
    auto tpcds19 = CrownTPCDS19(std::cout, print_result, delta_mode);
    execute(tpcds19);
  } else if (query == "line5sd") {
    auto line5sd = CrownLine5sd(std::cout, print_result);
    execute(line5sd);
  } else if (query == "job8s") {
    auto job8s = CrownJOB8s(std::cout, print_result);
    execute(job8s);
  } else if (query == "job8m1") {
    auto job8m1 = CrownJOB8m1(std::cout, print_result);
    execute(job8m1);
  } else if (query == "job8m2") {
    auto job8m2 = CrownJOB8m2(std::cout, print_result);
    execute(job8m2);
  } else {
    fmt::println("Unknown query '{}'.", query);
    exit(1);
  }
  fmt::println("");
}

int main(int argc, char **argv) {
  parse_args(argc, argv);
  run();
  return 0;
}