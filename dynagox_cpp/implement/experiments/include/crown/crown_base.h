#ifndef DYNAGOX_CROWN_BASE_H
#define DYNAGOX_CROWN_BASE_H

#include "utils.h"
#include <fstream>
#include <string>

class CrownBase {
protected:
  bool print_result;
  std::ostream &output;

public:
  virtual void process(const std::string &line) = 0;
  virtual void milestone() = 0;

  explicit CrownBase(std::ostream &output, bool print_result)
      : output(output), print_result(print_result) {}
};

#endif
