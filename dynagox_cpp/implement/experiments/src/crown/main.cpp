#include "crown_base.h"
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
#include "crown_linek_count.h"
#include "utils.h"
#include "fmt/core.h"
#include <chrono>
#ifdef _WIN32
// Simple getopt replacement for Windows
#include <cstring>
char* optarg = nullptr;
int optind = 1;
#define required_argument 1
#define no_argument 0
struct option {
    const char* name;
    int has_arg;
    int* flag;
    int val;
};
int getopt_long(int argc, char** argv, const char* optstring, const struct option* longopts, int* longindex) {
    static int current_arg = 1;
    static char* current_opt = nullptr;
    
    if (current_opt && *current_opt) {
        char opt = *current_opt++;
        const char* p = strchr(optstring, opt);
        if (p && p[1] == ':') {
            if (*current_opt) {
                optarg = current_opt;
                current_opt = nullptr;
            } else if (current_arg < argc) {
                optarg = argv[current_arg++];
            }
        }
        if (!*current_opt) current_opt = nullptr;
        return opt;
    }
    
    if (current_arg >= argc) return -1;
    
    char* arg = argv[current_arg];
    if (arg[0] != '-') {
        current_arg++;
        return getopt_long(argc, argv, optstring, longopts, longindex);
    }
    
    if (arg[1] == '-') {
        // Long option
        char* longopt = arg + 2;
        char* eq = strchr(longopt, '=');
        if (eq) *eq = '\0';
        
        for (int i = 0; longopts[i].name != nullptr; i++) {
            if (strcmp(longopt, longopts[i].name) == 0) {
                if (longopts[i].has_arg == required_argument) {
                    if (eq) {
                        optarg = eq + 1;
                    } else if (current_arg + 1 < argc) {
                        optarg = argv[++current_arg];
                    } else {
                        return '?';
                    }
                } else if (longopts[i].flag) {
                    *longopts[i].flag = longopts[i].val;
                }
                current_arg++;
                return longopts[i].flag ? 0 : longopts[i].val;
            }
        }
        return '?';
    } else {
        // Short option
        current_opt = arg + 1;
        current_arg++;
        return getopt_long(argc, argv, optstring, longopts, longindex);
    }
}
#else
#include <getopt.h>
#endif
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

std::string input;
std::string query;
bool delta_mode;
int print_result = 0;
int linek_k = 0;
int linek_a = 0;
int linek_b = 0;
std::string linek_mode = "count";
std::string linek_root_kind = "auto";
int linek_root_pos = 0;

const option long_options[] = {{"input",        required_argument, 0,             'i'},
                               {"query",        required_argument, 0,             'q'},
                               {"algorithm",    required_argument, 0,             'a'},
                               {"mode",         required_argument, 0,             'm'},
                               {"print-result", no_argument,       &print_result, 1},
                               {"k",            required_argument, 0,             'k'},
                               {"a",            required_argument, 0,             'x'},
                               {"b",            required_argument, 0,             'y'},
                               {"linek-mode",   required_argument, 0,             'z'},
                               {"linek-root-kind", required_argument, 0,          'u'},
                               {"linek-root-pos", required_argument, 0,           'v'},
                               {"help",         no_argument,       0,             'h'},
                               {0, 0,                              0,             0}};

void parse_args(int argc, char **argv) {
  std::string mode;

  while (true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:q:a:m:pk:x:y:z:u:v:h", long_options, &option_index);

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
      case 'k':
        linek_k = std::stoi(optarg);
        break;
      case 'x':
        linek_a = std::stoi(optarg);
        break;
      case 'y':
        linek_b = std::stoi(optarg);
        break;
      case 'z':
        linek_mode = optarg;
        break;
      case 'u':
        linek_root_kind = optarg;
        break;
      case 'v':
        linek_root_pos = std::stoi(optarg);
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
  } else if (query == "linek_count") {
    if (linek_k == 0 || linek_a == 0 || linek_b == 0) {
      fmt::println("linek_count requires --k, --a, and --b.");
      exit(1);
    }
    bool raw_mode = (linek_mode == "raw");
    auto linek = CrownLinekCount(std::cout, print_result, linek_k, linek_a,
                                 linek_b, raw_mode, linek_root_kind,
                                 linek_root_pos);
    execute(linek);
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
