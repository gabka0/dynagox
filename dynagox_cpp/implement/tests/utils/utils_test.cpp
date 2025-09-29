#include "utils.h"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <chrono>
#include <cmath>
#include <regex>
#include <sstream>
#include <string>

TEST_CASE("Utils: should print execution time in ms on print_execution_time") {
  constexpr unsigned long expected = 1427;
  auto start = std::chrono::system_clock::time_point{};
  auto end = start + std::chrono::milliseconds(expected);

  std::ostringstream stream;
  Utils::print_execution_time(stream, start, end);
  std::string result = stream.str();

  std::regex pattern(R"((\d+) ms)");
  std::smatch match;
  REQUIRE(std::regex_search(result, match, pattern) == true);
  REQUIRE(match.size() == 2);

  auto actual = std::stoul(match[1].str());
  REQUIRE(actual == expected);
}

TEST_CASE("Utils: should compute b for given epsilon and n on get_b") {
  double epsilon = 0.1;
  double n = 3;
  double expected = 1 + epsilon;
  double b = Utils::get_b(epsilon, n);
  double actual = std::pow(b, n);
  REQUIRE_THAT(actual, Catch::Matchers::WithinRel(expected, 0.000001));
}