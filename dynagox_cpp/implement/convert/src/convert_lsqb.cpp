#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/os.h"
#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>

std::string input;
std::string output;

std::random_device rd;
std::mt19937 rng(rd());

std::vector<std::tuple<std::string, std::string, std::string>> rows;

const option long_options[] = {{"input", required_argument, 0, 'i'},
                               {"output", required_argument, 0, 'o'},
                               {0, 0, 0, 0}};

void check() {
  if (input.empty()) {
    fmt::println("Missing input directory. Please provide the input path using "
                 "the '-i' option.");
    exit(1);
  }

  if (output.empty()) {
    fmt::println("Missing output directory. Please provide the output path "
                 "using the '-o' option.");
    exit(1);
  }
}

void print_info() {
  fmt::println("Input: {}", input);
  fmt::println("Output: {}", output);
}

void parse_args(int argc, char **argv) {
  while (true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:o:", long_options, &option_index);

    if (c == -1)
      break;

    switch (c) {
    case 0:
      break;
    case 'i':
      input = optarg;
      if (!input.ends_with('/')) {
        input.append("/");
      }
      break;
    case 'o':
      output = optarg;
      if (!output.ends_with('/')) {
        output.append("/");
      }
      break;
    default:
      break;
    }
  }
  check();
  print_info();
}

void read_person_knows_person(const std::string &file) {
  std::ifstream in(file);
  std::string line;

  // PersonKnowsPerson:
  // creationDate|deletionDate|explicitlyDeleted|Person1Id|Person2Id
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, '|');
    getline(stream, field, '|');
    getline(stream, field, '|');
    getline(stream, field, '|');
    std::string person1_id = field;
    getline(stream, field, '|');
    std::string person2_id = field;

    // person_knows_person(person1_id, person2_id)
    rows.emplace_back(std::make_tuple("0", person1_id, person2_id));
  }
}

void read_person_has_interest_tag(const std::string &file) {
  std::ifstream in(file);
  std::string line;

  // Person_hasInterest_Tag: creationDate|deletionDate|PersonId|TagId
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, '|');
    getline(stream, field, '|');
    getline(stream, field, '|');
    std::string person_id = field;
    getline(stream, field, '|');
    std::string tag_id = field;

    // person_has_interest_tag(person_id, tag_id)
    rows.emplace_back(std::make_tuple("1", person_id, tag_id));
  }
}

void read_input() {
  auto person_knows_person_path = input + "Person_knows_Person";
  for (const auto &entry :
       std::filesystem::directory_iterator(person_knows_person_path)) {
    std::string path = entry.path().string();
    if (path.ends_with(".csv")) {
      read_person_knows_person(path);
    }
  }

  auto person_has_interest_tag_path = input + "Person_hasInterest_Tag";
  for (const auto &entry :
       std::filesystem::directory_iterator(person_has_interest_tag_path)) {
    std::string path = entry.path().string();
    if (path.ends_with(".csv")) {
      read_person_has_interest_tag(path);
    }
  }

  std::shuffle(rows.begin(), rows.end(), rng);
}

void convert() {
  auto out_a = fmt::output_file(output + "lsqb6_all.csv");
  auto out_pkp = fmt::output_file(output + "lsqb6_person_knows_person.txt");
  auto out_phit = fmt::output_file(output + "lsqb6_person_has_interest_tag.txt");
  for (const auto &row : rows) {
    if (std::get<0>(row) == "0") {
      out_a.print("0|{}|{}\n", std::get<1>(row), std::get<2>(row));
      out_a.print("0|{}|{}\n", std::get<2>(row), std::get<1>(row));
      out_pkp.print("{{\"insert\":{{\"person1_id\":{},\"person2_id\":{}}}}}\n", std::get<1>(row), std::get<2>(row));
      out_pkp.print("{{\"insert\":{{\"person1_id\":{},\"person2_id\":{}}}}}\n", std::get<2>(row), std::get<1>(row));
    } else if (std::get<0>(row) == "1") {
      out_a.print("1|{}|{}\n", std::get<1>(row), std::get<2>(row));
      out_phit.print("{{\"insert\":{{\"person_id\":{},\"tag_id\":{}}}}}\n", std::get<1>(row), std::get<2>(row));
    }
  }

  out_a.print("*\n");

  out_a.flush();
  out_pkp.flush();
  out_phit.flush();

  out_a.close();
  out_pkp.close();
  out_phit.close();
}

int main(int argc, char **argv) {
  parse_args(argc, argv);
  read_input();
  convert();
  return 0;
}