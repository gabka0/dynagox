#include "fmt/core.h"
#include "fmt/format.h"
#include "fmt/os.h"
#include <cassert>
#include <fstream>
#include <getopt.h>
#include <random>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_set>
#include <vector>
#include <algorithm>

std::string input;
std::string output;
unsigned long factor = 10;
std::string query = "all";

std::random_device rd;
std::mt19937 rng(rd());

std::vector<std::vector<unsigned long>> rows;

std::vector<std::tuple<unsigned long, unsigned long>> aka_name;
std::vector<std::tuple<unsigned long, unsigned long, unsigned long,
                       unsigned long, unsigned long>>
    cast_info;
std::unordered_set<unsigned long> char_name;
std::unordered_set<unsigned long> company_name;
std::unordered_set<unsigned long> info_type;
std::unordered_set<unsigned long> keyword;
std::vector<std::tuple<unsigned long, unsigned long, unsigned long>>
    movie_companies;
std::vector<std::tuple<unsigned long, unsigned long, unsigned long>> movie_info;
std::vector<std::tuple<unsigned long, unsigned long, unsigned long>>
    movie_keyword;
std::unordered_set<unsigned long> name;
std::unordered_set<unsigned long> role_type;
std::unordered_set<unsigned long> title;

unsigned long aka_name_id_max = 0;
unsigned long cast_info_id_max = 0;
unsigned long movie_companies_id_max = 0;
unsigned long movie_info_id_max = 0;
unsigned long movie_keyword_id_max = 0;

const option long_options[] = {{"input", required_argument, 0, 'i'},
                               {"output", required_argument, 0, 'o'},
                               {"factor", required_argument, 0, 'f'},
                               {"query", required_argument, 0, 'q'},
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

  if (factor == 0) {
    fmt::println("Invalid factor. Please provide a value greater than 0 using "
                 "the '-f' option.");
    exit(1);
  }

  if (query != "all" && query != "8" && query != "16" && query != "19") {
    fmt::println("Unknown query '{}'.", query);
    exit(1);
  }
}

void print_info() {
  fmt::println("Input: {}", input);
  fmt::println("Output: {}", output);
  fmt::println("Factor: {}", factor);
}

void parse_args(int argc, char **argv) {
  while (true) {
    int option_index = 0;
    int c = getopt_long(argc, argv, "i:o:f:q:", long_options, &option_index);

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
    case 'f':
      factor = strtoul(optarg, nullptr, 0);
      break;
    case 'q':
      query = optarg;
      break;
    default:
      break;
    }
  }
  check();
  print_info();
}

void read_aka_name(const std::string &file) {
  std::ifstream in(file);
  std::string line;

  // id,person_id,name,imdb_index,name_pcode_cf,name_pcode_nf,surname_pcode,md5sum
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, ',');
    unsigned long id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long person_id = std::stoul(field);

    if (name.contains(person_id)) {
      // aka_name(id,person_id)
      aka_name.emplace_back(std::make_tuple(id, person_id));
      aka_name_id_max = std::max(aka_name_id_max, id);
    }
  }
}

void read_cast_info(const std::string &file, const std::string &q) {
  std::ifstream in(file);
  std::string line;

  // id,person_id,movie_id,person_role_id,role_id
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, ',');
    unsigned long id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long person_id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long movie_id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long person_role_id = field.empty() ? 0 : std::stoul(field);
    getline(stream, field, ',');
    unsigned long role_id = field.empty() ? 0 : std::stoul(field);

    if ((q == "8" && name.contains(person_id) && role_type.contains(role_id) &&
         title.contains(movie_id)) ||
        (q == "16" && name.contains(person_id) && title.contains(movie_id)) ||
        (q == "19" && char_name.contains(person_role_id) &&
         name.contains(person_id) && role_type.contains(role_id) &&
         title.contains(movie_id))) {
      // cast_info(id,person_id,movie_id,person_role_id,role_id)
      cast_info.emplace_back(
          std::make_tuple(id, person_id, movie_id, person_role_id, role_id));
      cast_info_id_max = std::max(cast_info_id_max, id);
    }
  }
}

void read_movie_companies(const std::string &file) {
  std::ifstream in(file);
  std::string line;

  // id,movie_id,company_id,company_type_id,note
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, ',');
    unsigned long id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long movie_id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long company_id = std::stoul(field);

    if (company_name.contains(company_id) && title.contains(movie_id)) {
      // movie_companies(id,movie_id,company_id)
      movie_companies.emplace_back(std::make_tuple(id, movie_id, company_id));
      movie_companies_id_max = std::max(movie_companies_id_max, id);
    }
  }
}

void read_movie_info(const std::string &file) {
  std::ifstream in(file);
  std::string line;

  // id,movie_id,info_type_id,info,note
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, ',');
    unsigned long id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long movie_id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long info_type_id = std::stoul(field);

    if (info_type.contains(info_type_id) && title.contains(movie_id)) {
      // movie_info(id,movie_id,info_type_id)
      movie_info.emplace_back(std::make_tuple(id, movie_id, info_type_id));
      movie_info_id_max = std::max(movie_info_id_max, id);
    }
  }
}

void read_movie_keyword(const std::string &file) {
  std::ifstream in(file);
  std::string line;

  // id,movie_id,keyword_id
  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, ',');
    unsigned long id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long movie_id = std::stoul(field);
    getline(stream, field, ',');
    unsigned long keyword_id = std::stoul(field);

    if (keyword.contains(keyword_id) && title.contains(movie_id)) {
      // movie_keyword(id,movie_id,keyword_id)
      movie_keyword.emplace_back(std::make_tuple(id, movie_id, keyword_id));
      movie_keyword_id_max = std::max(movie_keyword_id_max, id);
    }
  }
}

void read_id(std::unordered_set<unsigned long> &table,
             const std::string &file) {
  std::ifstream in(file);
  std::string line;

  getline(in, line);

  while (getline(in, line)) {
    std::stringstream stream(line);
    std::string field;

    getline(stream, field, ',');
    table.emplace(std::stoul(field));
  }
}

void read_from_files(const std::string &q) {
  if (q == "8") {
    read_id(company_name, input + "company_name.csv");
    read_id(name, input + "name.csv");
    read_id(role_type, input + "role_type.csv");
    read_id(title, input + "title.csv");

    read_aka_name(input + "aka_name.csv");
    read_cast_info(input + "cast_info.csv", "8");
    read_movie_companies(input + "movie_companies.csv");
  } else if (q == "16") {
    read_id(company_name, input + "company_name.csv");
    read_id(keyword, input + "keyword.csv");
    read_id(name, input + "name.csv");
    read_id(title, input + "title.csv");

    read_aka_name(input + "aka_name.csv");
    read_cast_info(input + "cast_info.csv", "16");
    read_movie_companies(input + "movie_companies.csv");
    read_movie_keyword(input + "movie_keyword.csv");
  } else if (q == "19") {
    read_id(char_name, input + "char_name.csv");
    read_id(company_name, input + "company_name.csv");
    read_id(info_type, input + "info_type.csv");
    read_id(name, input + "name.csv");
    read_id(role_type, input + "role_type.csv");
    read_id(title, input + "title.csv");

    read_aka_name(input + "aka_name.csv");
    read_cast_info(input + "cast_info.csv", "19");
    read_movie_companies(input + "movie_companies.csv");
    read_movie_info(input + "movie_info.csv");
  }
}

void scale_up(const std::string &q) {
  for (auto &t : aka_name) {
    rows.emplace_back(std::vector<unsigned long>{0, std::get<0>(t), std::get<1>(t)});
  }

  unsigned long aka_name_id = aka_name_id_max;
  for (unsigned long i = 0; i < factor - 1; i++) {
    for (auto &t : aka_name) {
      aka_name_id += 1;
      rows.emplace_back(std::vector<unsigned long>{0, aka_name_id, std::get<1>(t)});
    }
  }

  for (auto &t : cast_info) {
    rows.emplace_back(std::vector<unsigned long>{1, std::get<0>(t), std::get<1>(t), std::get<2>(t)});
  }

  unsigned long cast_info_id = cast_info_id_max;
  for (int i = 0; i < factor - 1; i++) {
    for (auto &t : cast_info) {
      cast_info_id += 1;
      rows.emplace_back(std::vector<unsigned long>{1, cast_info_id, std::get<1>(t), std::get<2>(t)});
    }
  }

  for (auto &t : movie_companies) {
    rows.emplace_back(std::vector<unsigned long>{2, std::get<0>(t), std::get<1>(t), std::get<2>(t)});
  }

  unsigned long movie_companies_id = movie_companies_id_max;
  for (int i = 0; i < factor - 1; i++) {
    for (auto &t : movie_companies) {
      movie_companies_id += 1;
      rows.emplace_back(std::vector<unsigned long>{2, movie_companies_id, std::get<1>(t), std::get<2>(t)});
    }
  }

  if (q == "16") {
    for (auto &t : movie_keyword) {
      rows.emplace_back(std::vector<unsigned long>{4, std::get<0>(t), std::get<1>(t), std::get<2>(t)});
    }

    unsigned long movie_keyword_id = movie_keyword_id_max;
    for (int i = 0; i < factor - 1; i++) {
      for (auto &t : movie_keyword) {
        movie_keyword_id += 1;
        rows.emplace_back(std::vector<unsigned long>{4, movie_keyword_id, std::get<1>(t), std::get<2>(t)});
      }
    }
  } else if (q == "19") {
    for (auto &t : movie_info) {
      rows.emplace_back(std::vector<unsigned long>{3, std::get<0>(t), std::get<1>(t), std::get<2>(t)});
    }

    unsigned long movie_info_id = movie_info_id_max;
    for (int i = 0; i < factor - 1; i++) {
      for (auto &t : movie_info) {
        movie_info_id += 1;
        rows.emplace_back(std::vector<unsigned long>{3, movie_info_id, std::get<1>(t), std::get<2>(t)});
      }
    }
  }
}

void shuffle_rows() { std::shuffle(rows.begin(), rows.end(), rng); }

void write_to_file(const std::string &q) {
  auto out_a = fmt::output_file(output + "job" + q + "_all.csv");
  auto out_an = fmt::output_file(output + "job" + q + "_aka_name.txt");
  auto out_ci = fmt::output_file(output + "job" + q + "_cast_info.txt");
  auto out_mc = fmt::output_file(output + "job" + q + "_movie_companies.txt");

  if (q == "8") {
    for (const auto &r : rows) {
      if (r.at(0) == 0) {
        out_a.print("{}|{}|{}\n", 0, r.at(1), r.at(2));
        out_an.print("{{\"insert\": {{\"id\": {}, \"person_id\": {}}}}}\n", r.at(1), r.at(2));
      } else if (r.at(0) == 1) {
        out_a.print("{}|{}|{}|{}\n", 1, r.at(1), r.at(2), r.at(3));
        out_ci.print("{{\"insert\": {{\"id\": {}, \"person_id\": {}, \"movie_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      } else if (r.at(0) == 2) {
        out_a.print("{}|{}|{}|{}\n", 2, r.at(1), r.at(2), r.at(3));
        out_mc.print("{{\"insert\": {{\"id\": {}, \"movie_id\": {}, \"company_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      }
    }
  } else if (q == "16") {
    auto out_mk = fmt::output_file(output + "job" + q + "_movie_keyword.txt");
    for (const auto &r : rows) {
      if (r.at(0) == 0) {
        out_a.print("{}|{}|{}\n", 0, r.at(1), r.at(2));
        out_an.print("{{\"insert\": {{\"id\": {}, \"person_id\": {}}}}}\n", r.at(1), r.at(2));
      } else if (r.at(0) == 1) {
        out_a.print("{}|{}|{}|{}\n", 1, r.at(1), r.at(2), r.at(3));
        out_ci.print("{{\"insert\": {{\"id\": {}, \"person_id\": {}, \"movie_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      } else if (r.at(0) == 2) {
        out_a.print("{}|{}|{}|{}\n", 2, r.at(1), r.at(2), r.at(3));
        out_mc.print("{{\"insert\": {{\"id\": {}, \"movie_id\": {}, \"company_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      } else if (r.at(0) == 4) {
        out_a.print("{}|{}|{}|{}\n", 4, r.at(1), r.at(2), r.at(3));
        out_mk.print("{{\"insert\": {{\"id\": {}, \"movie_id\": {}, \"keyword_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      }
    }
    out_mk.flush();
    out_mk.close();
  } else if (q == "19") {
    auto out_mi = fmt::output_file(output + "job" + q + "_movie_info.txt");
    for (const auto &r : rows) {
      if (r.at(0) == 0) {
        out_a.print("{}|{}|{}\n", 0, r.at(1), r.at(2));
        out_an.print("{{\"insert\": {{\"id\": {}, \"person_id\": {}}}}}\n", r.at(1), r.at(2));
      } else if (r.at(0) == 1) {
        out_a.print("{}|{}|{}|{}\n", 1, r.at(1), r.at(2), r.at(3));
        out_ci.print("{{\"insert\": {{\"id\": {}, \"person_id\": {}, \"movie_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      } else if (r.at(0) == 2) {
        out_a.print("{}|{}|{}|{}\n", 2, r.at(1), r.at(2), r.at(3));
        out_mc.print("{{\"insert\": {{\"id\": {}, \"movie_id\": {}, \"company_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      } else if (r.at(0) == 3) {
        out_a.print("{}|{}|{}|{}\n", 3, r.at(1), r.at(2), r.at(3));
        out_mi.print("{{\"insert\": {{\"id\": {}, \"movie_id\": {}, \"info_type_id\": {}}}}}\n", r.at(1), r.at(2), r.at(3));
      }
    }
    out_mi.flush();
    out_mi.close();
  }

  out_a.print("*\n");

  out_a.flush();
  out_an.flush();
  out_ci.flush();
  out_mc.flush();

  out_a.close();
  out_an.close();
  out_ci.close();
  out_mc.close();

  rows.clear();
  fmt::println("Converted job{}.", q);
}

void clear() {
  aka_name.clear();
  cast_info.clear();
  char_name.clear();
  company_name.clear();
  info_type.clear();
  keyword.clear();
  movie_companies.clear();
  movie_info.clear();
  movie_keyword.clear();
  name.clear();
  role_type.clear();
  title.clear();

  aka_name_id_max = 0;
  cast_info_id_max = 0;
  movie_companies_id_max = 0;
  movie_info_id_max = 0;
  movie_keyword_id_max = 0;
}

void convert(const std::string &q) {
  if (q == "8") {
    read_from_files("8");
    scale_up("8");
    shuffle_rows();
    write_to_file("8");
  } else if (q == "16") {
    read_from_files("16");
    scale_up("16");
    shuffle_rows();
    write_to_file("16");
  } else if (q == "19") {
    read_from_files("19");
    scale_up("19");
    shuffle_rows();
    write_to_file("19");
  }
  clear();
}

int main(int argc, char **argv) {
  parse_args(argc, argv);
  if (query == "all") {
    convert("8");
    convert("16");
    convert("19");
  } else if (query == "8" || query == "16" || query == "19") {
    convert(query);
  }
  return 0;
}