#include "dynagox_dumbbell.h"
#include "dynagox_job8.h"
#include "dynagox_job16.h"
#include "dynagox_job19.h"
#include "dynagox_ceb1.h"
#include "dynagox_ceb2.h"
#include "dynagox_ceb3.h"
#include "dynagox_ceb4.h"
#include "dynagox_line5c.h"
#include "dynagox_line5cd.h"
#include "dynagox_line5gc.h"
#include "dynagox_line5m.h"
#include "dynagox_line5s.h"
#include "dynagox_lsqb6.h"
#include "dynagox_tpch3.h"
#include "dynagox_tpch5.h"
#include "dynagox_tpch8.h"
#include "dynagox_tpch9.h"
#include "dynagox_tpch10.h"
#include "dynagox_tpcds15.h"
#include "dynagox_tpcds19.h"
#include "dynagox_example.h"
#include "dynagox_line5s_d.h"
#include "dynagox_job8s.h"
#include "dynagox_job8m1.h"
#include "dynagox_job8m2.h"
#include "fmt/core.h"
#include "utils.h"
#include <getopt.h>
#include <iostream>
#include <string>

std::string input;
std::string query;
double epsilon = 0.1;
bool delta_mode;
int print_result = 0;

const option long_options[] = {{"input",        required_argument, 0,             'i'},
                               {"query",        required_argument, 0,             'q'},
                               {"epsilon",      required_argument, 0,             'e'},
                               {"mode",         required_argument, 0,             'm'},
                               {"print-result", no_argument,       &print_result, 1},
                               {0, 0,                              0,             0}};

void parse_args(int argc, char **argv) {
  std::string mode;

  while (true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:q:e:m:p", long_options, &option_index);

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
      case 'e':
        epsilon = std::stod(optarg);
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

void execute(DynaGoxBase &dynagox) {
  std::ifstream in(input);
  std::string line;

  auto start = std::chrono::system_clock::now();
  while (getline(in, line)) {
    if (line.starts_with("*")) {
      dynagox.milestone();
      auto current = std::chrono::system_clock::now();
      Utils::print_execution_time(std::cout, start, current);
      Utils::print_memory_usage(std::cout);
      continue;
    } else {
      dynagox.process(line);
    }
  }
}

void run() {
  fmt::println("Query: {}", query);
  fmt::println("Algorithm: {}", "DynaGox");
  fmt::println("Epsilon: {:.2f}", epsilon);
  if (query == "line5c") {
    double b = Utils::get_b(epsilon, 3);
    auto line5c = DynaGoxLine5c(std::cout, print_result, b);
    execute(line5c);
  } else if (query == "line5s") {
    double b = Utils::get_b(epsilon, 3);
    auto line5s = DynaGoxLine5s(std::cout, print_result, b);
    execute(line5s);
  } else if (query == "line5m") {
    double b = Utils::get_b(epsilon, 3);
    auto line5m = DynaGoxLine5m(std::cout, print_result, b);
    execute(line5m);
  } else if (query == "line5cd") {
    double b = Utils::get_b(epsilon, 1);
    auto line5cd = DynaGoxLine5cd(std::cout, print_result, b);
    execute(line5cd);
  } else if (query == "line5gc") {
    double b = Utils::get_b(epsilon, 2);
    auto line5gc = DynaGoxLine5gc(std::cout, print_result, b, delta_mode);
    execute(line5gc);
  } else if (query == "dumbbell") {
    double b = Utils::get_b(epsilon, 1);
    auto dumbbell = DynaGoxDumbbell(std::cout, print_result, b);
    execute(dumbbell);
  } else if (query == "lsqb6") {
    double b = Utils::get_b(epsilon, 1);
    auto lsqb6 = DynaGoxLSQB6(std::cout, print_result, b);
    execute(lsqb6);
  } else if (query == "job8") {
    double b = Utils::get_b(epsilon, 1);
    auto job8 = DynaGoxJOB8(std::cout, print_result, b);
    execute(job8);
  } else if (query == "job16") {
    double b = Utils::get_b(epsilon, 2);
    auto job16 = DynaGoxJOB16(std::cout, print_result, b);
    execute(job16);
  } else if (query == "job19") {
    double b = Utils::get_b(epsilon, 2);
    auto job19 = DynaGoxJOB19(std::cout, print_result, b);
    execute(job19);
  } else if (query == "ceb1") {
    double b = Utils::get_b(epsilon, 1);
    auto ceb1 = DynaGoxCEB1(std::cout, print_result, b);
    execute(ceb1);
  } else if (query == "ceb2") {
    double b = Utils::get_b(epsilon, 2);
    auto ceb2 = DynaGoxCEB2(std::cout, print_result, b);
    execute(ceb2);
  } else if (query == "ceb3") {
    double b = Utils::get_b(epsilon, 1);
    auto ceb3 = DynaGoxCEB3(std::cout, print_result, b);
    execute(ceb3);
  } else if (query == "ceb4") {
    double b = Utils::get_b(epsilon, 2);
    auto ceb4 = DynaGoxCEB4(std::cout, print_result, b);
    execute(ceb4);
  } else if (query == "tpch3") {
    double b = Utils::get_b(epsilon, 1);
    auto tpch3 = DynaGoxTPCH3(std::cout, print_result, b, delta_mode);
    execute(tpch3);
  } else if (query == "tpch5") {
    double b = Utils::get_b(epsilon, 1);
    auto tpch5 = DynaGoxTPCH5(std::cout, print_result, b, delta_mode);
    execute(tpch5);
  } else if (query == "tpch8") {
    double b = Utils::get_b(epsilon, 1);
    auto tpch8 = DynaGoxTPCH8(std::cout, print_result, b, delta_mode);
    execute(tpch8);
  } else if (query == "tpch9") {
    double b = Utils::get_b(epsilon, 1);
    auto tpch9 = DynaGoxTPCH9(std::cout, print_result, b, delta_mode);
    execute(tpch9);
  } else if (query == "tpch10") {
    double b = Utils::get_b(epsilon, 1);
    auto tpch10 = DynaGoxTPCH10(std::cout, print_result, b, delta_mode);
    execute(tpch10);
  } else if (query == "tpcds15") {
    double b = Utils::get_b(epsilon, 1);
    auto tpcds15 = DynaGoxTPCDS15(std::cout, print_result, b, delta_mode);
    execute(tpcds15);
  } else if (query == "tpcds19") {
    double b = Utils::get_b(epsilon, 1);
    auto tpcds19 = DynaGoxTPCDS19(std::cout, print_result, b, delta_mode);
    execute(tpcds19);
  } else if (query == "example") {
    double b = Utils::get_b(epsilon, 1);
    auto example = DynaGoxExample(std::cout, print_result, b, delta_mode);
    example.run();
  } else if (query == "line5sd") {
    double b = Utils::get_b(epsilon, 1);
    auto line5sd = DynaGoxLine5sd(std::cout, print_result, b);
    execute(line5sd);
  } else if (query == "job8s") {
    double b = Utils::get_b(epsilon, 1);
    auto job8s = DynaGoxJOB8s(std::cout, print_result, b);
    execute(job8s);
  } else if (query == "job8m1") {
    double b = Utils::get_b(epsilon, 1);
    auto job8m1 = DynaGoxJOB8m1(std::cout, print_result, b);
    execute(job8m1);
  } else if (query == "job8m2") {
    double b = epsilon;
    auto job8m2 = DynaGoxJOB8m2(std::cout, print_result, b);
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