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

enum RelationID {
  STORE_SALES_ID = 0,
  CATALOG_SALES_ID = 1,
  STORE_ID = 2,
  CUSTOMER_ID = 3,
  CUSTOMER_ADDRESS_ID = 4,
  DATE_DIM_ID = 5,
  ITEM_ID = 6
};

const option long_options[] = {{"input",  required_argument, 0, 'i'},
                               {"output", required_argument, 0, 'o'},
                               {0, 0,                        0, 0}};

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

std::vector<std::string> parse_row(const std::string &row) {
  std::vector<std::string> fields;
  std::stringstream stream(row);
  std::string field;
  while (std::getline(stream, field, '|')) {
    fields.push_back(field);
  }
  return fields;
}

void read_table(const std::string &file, RelationID relation_id, std::vector<std::string> &rows) {
  std::ifstream in(file);
  if (!in.is_open()) {
    throw std::runtime_error("Failed to open file: " + file);
  }

  std::string line;
  std::getline(in, line);
  while (std::getline(in, line)) {
    rows.emplace_back(std::to_string(relation_id) + "|" + line);
  }
}

void write_store_sales(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print(
      "{{\"{}\": {{\"ss_sold_date_sk\": {}, \"ss_item_sk\": {}, \"ss_customer_sk\": {}, \"ss_store_sk\": {}, "
      "\"ss_ext_sales_price\": {}}}}}\n",
      action, std::stoul(fields[1]), std::stoul(fields[2]), std::stoul(fields[3]), std::stoul(fields[4]),
      std::stod(fields[5]));
}

void write_catalog_sales(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print(
      "{{\"{}\": {{\"cs_sold_date_sk\": {}, \"cs_bill_customer_sk\": {}, \"cs_sales_price\": {}}}}}\n",
      action, std::stoul(fields[1]), std::stoul(fields[2]), std::stod(fields[3]));
}

void write_store(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print(
      "{{\"{}\": {{\"s_store_sk\": {}}}}}\n",
      action, std::stoul(fields[1]));
}

void write_customer(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"c_customer_sk\": {}, \"c_current_addr_sk\": {}}}}}\n",
            action, std::stoul(fields[1]), std::stoul(fields[2]));
}

void write_customer_address(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print(
      "{{\"{}\": {{\"ca_address_sk\": {}, \"ca_zip\": \"{}\"}}}}\n",
      action, std::stoul(fields[1]), fields[2]);
}

void write_date_dim(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print(
      "{{\"{}\": {{\"d_date_sk\": {}}}}}\n",
      action, std::stoul(fields[1]));
}

void write_item(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print(
      "{{\"{}\": {{\"i_item_sk\": {}, \"i_brand_id\": {}, \"i_brand\": \"{}\", \"i_manufact_id\": {}, "
      "\"i_manufact\": \"{}\"}}}}\n",
      action, std::stoul(fields[1]), std::stoul(fields[2]), fields[3], std::stoul(fields[4]), fields[5]);
}

void convert(const std::string &query_id) {
  std::vector<std::string> rows;
  if (query_id == "15") {
    auto out_a = fmt::output_file(output + "tpcds" + query_id + "_all.csv");
    auto out_cs = fmt::output_file(output + "tpcds" + query_id + "_catalog_sales.txt");
    auto out_c = fmt::output_file(output + "tpcds" + query_id + "_customer.txt");
    auto out_ca = fmt::output_file(output + "tpcds" + query_id + "_customer_address.txt");
    auto out_d = fmt::output_file(output + "tpcds" + query_id + "_date_dim.txt");

    read_table(input + "catalog_sales.csv", CATALOG_SALES_ID, rows);
    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "customer_address.csv", CUSTOMER_ADDRESS_ID, rows);
    read_table(input + "date_dim.csv", DATE_DIM_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("{}\n", row);
      if (row.starts_with(std::to_string(CATALOG_SALES_ID))) {
        write_catalog_sales(out_cs, row, "insert");
      } else if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(CUSTOMER_ADDRESS_ID))) {
        write_customer_address(out_ca, row, "insert");
      } else if (row.starts_with(std::to_string(DATE_DIM_ID))) {
        write_date_dim(out_d, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_cs.flush();
    out_c.flush();
    out_ca.flush();
    out_d.flush();
    out_a.close();
    out_cs.close();
    out_c.close();
    out_ca.close();
    out_d.close();
  } else if (query_id == "19") {
    auto out_a = fmt::output_file(output + "tpcds" + query_id + "_all.csv");
    auto out_ss = fmt::output_file(output + "tpcds" + query_id + "_store_sales.txt");
    auto out_s = fmt::output_file(output + "tpcds" + query_id + "_store.txt");
    auto out_c = fmt::output_file(output + "tpcds" + query_id + "_customer.txt");
    auto out_ca = fmt::output_file(output + "tpcds" + query_id + "_customer_address.txt");
    auto out_d = fmt::output_file(output + "tpcds" + query_id + "_date_dim.txt");
    auto out_i = fmt::output_file(output + "tpcds" + query_id + "_item.txt");

    read_table(input + "store_sales.csv", STORE_SALES_ID, rows);
    read_table(input + "store.csv", STORE_ID, rows);
    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "customer_address.csv", CUSTOMER_ADDRESS_ID, rows);
    read_table(input + "date_dim.csv", DATE_DIM_ID, rows);
    read_table(input + "item.csv", ITEM_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("{}\n", row);
      if (row.starts_with(std::to_string(STORE_SALES_ID))) {
        write_store_sales(out_ss, row, "insert");
      } else if (row.starts_with(std::to_string(STORE_ID))) {
        write_store(out_s, row, "insert");
      } else if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(CUSTOMER_ADDRESS_ID))) {
        write_customer_address(out_ca, row, "insert");
      } else if (row.starts_with(std::to_string(DATE_DIM_ID))) {
        write_date_dim(out_d, row, "insert");
      } else if (row.starts_with(std::to_string(ITEM_ID))) {
        write_item(out_i, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_ss.flush();
    out_s.flush();
    out_c.flush();
    out_ca.flush();
    out_d.flush();
    out_i.flush();

    out_a.close();
    out_ss.close();
    out_s.close();
    out_c.close();
    out_ca.close();
    out_d.close();
    out_i.close();
  }
}

int main(int argc, char **argv) {
  parse_args(argc, argv);
  convert("15");
  convert("19");
  return 0;
}