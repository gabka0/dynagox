#ifndef DYNAGOX_UTILS_H
#define DYNAGOX_UTILS_H

#include "fmt/ostream.h"
#include "ostream"
#include <chrono>
#include <cmath>

class Utils {
public:
  static void print_execution_time(std::ostream &output,
                                   std::chrono::time_point<std::chrono::system_clock> start,
                                   std::chrono::time_point<std::chrono::system_clock> end);

  static void print_memory_usage(std::ostream &output);

  static double get_b(double epsilon, double n);
};

#endif
