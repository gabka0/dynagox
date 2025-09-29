#ifndef DYNAGOX_DYNAGOX_BASE_H
#define DYNAGOX_DYNAGOX_BASE_H

#include "utils.h"
#include <fstream>
#include <string>

class DynaGoxBase {
protected:
  bool print_result;
  std::ostream &output;
  double b;

public:
  virtual void process(const std::string &line) = 0;
  virtual void milestone() = 0;

  explicit DynaGoxBase(std::ostream &output, bool print_result, double b)
      : output(output), print_result(print_result), b(b) {}
};

#endif
