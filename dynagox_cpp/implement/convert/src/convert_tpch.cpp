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
int window_enable = 0;
int dynamic_enable = 0;

std::random_device rd;
std::mt19937 rng(rd());

enum RelationID {
  PART_ID = 0,
  SUPPLIER_ID = 1,
  PARTSUPP_ID = 2,
  CUSTOMER_ID = 3,
  ORDERS_ID = 4,
  LINEITEM_ID = 5,
  NATION_ID = 6,
  REGION_ID = 7
};

const option long_options[] = {{"input",      required_argument, 0,               'i'},
                               {"output",     required_argument, 0,               'o'},
                               {"window",     no_argument,       &window_enable,  1},
                               {"dynamic",    no_argument,       &dynamic_enable, 1},
                               {0, 0,                            0,               0}};

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
    int c = getopt_long(argc, argv, "i:o:wd", long_options, &option_index);

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
      case 'w':
        window_enable = 1;
        break;
      case 'd':
        dynamic_enable = 1;
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

void write_part(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"p_partkey\": {}, \"p_name\": \"{}\", \"p_mfgr\": \"{}\", "
            "\"p_brand\": \"{}\", \"p_type\": \"{}\", \"p_size\": {}, "
            "\"p_container\": \"{}\", \"p_retailprice\": {}, \"p_comment\": \"{}\"}}}}\n",
            action, std::stoi(fields[1]), fields[2], fields[3], fields[4],
            fields[5], std::stoi(fields[6]), fields[7], std::stod(fields[8]), fields[9]);
}

void write_supplier(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"s_suppkey\": {}, \"s_name\": \"{}\", \"s_address\": \"{}\", "
            "\"s_nationkey\": {}, \"s_phone\": \"{}\", \"s_acctbal\": {}, "
            "\"s_comment\": \"{}\"}}}}\n",
            action, std::stoi(fields[1]), fields[2], fields[3], std::stoi(fields[4]),
            fields[5], std::stod(fields[6]), fields[7]);
}

void write_partsupp(fmt::ostream &out, const std::string &row, const std::string &action) {
  // ps_partkey|ps_suppkey|ps_availqty|ps_supplycost|ps_comment
  // 1|2|3325|771.64|blithely regular theodolites sleep slyly across the sometimes bold dependencies. even accounts among the slyly final sauternes cajole quickly about the doggedly even platelets. carefully final
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"ps_partkey\": {}, \"ps_suppkey\": {}, \"ps_availqty\": {}, "
            "\"ps_supplycost\": {}, \"ps_comment\": \"{}\"}}}}\n",
            action, std::stoi(fields[1]), std::stoi(fields[2]), std::stoi(fields[3]), std::stod(fields[4]),
            fields[5]);
}

void write_customer(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"c_custkey\": {}, \"c_name\": \"{}\", \"c_address\": \"{}\", "
            "\"c_nationkey\": {}, \"c_phone\": \"{}\", \"c_acctbal\": {}, "
            "\"c_mktsegment\": \"{}\", \"c_comment\": \"{}\"}}}}\n",
            action, std::stoul(fields[1]), fields[2], fields[3], std::stoul(fields[4]),
            fields[5], std::stod(fields[6]), fields[7], fields[8]);
}

void write_orders(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"o_orderkey\": {}, \"o_custkey\": {}, \"o_orderstatus\": \"{}\", "
            "\"o_totalprice\": {}, \"o_orderdate\": \"{}\", \"o_orderpriority\": \"{}\", "
            "\"o_clerk\": \"{}\", \"o_shippriority\": {}, \"o_comment\": \"{}\"}}}}\n",
            action, std::stoul(fields[1]), std::stoul(fields[2]), fields[3], std::stod(fields[4]),
            fields[5], fields[6], fields[7], std::stoul(fields[8]), fields[9]);
}

void write_lineitem(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"l_orderkey\": {}, \"l_partkey\": {}, \"l_suppkey\": {}, "
            "\"l_linenumber\": {}, \"l_quantity\": {}, \"l_extendedprice\": {}, "
            "\"l_discount\": {}, \"l_tax\": {}, \"l_returnflag\": \"{}\", "
            "\"l_linestatus\": \"{}\", \"l_shipdate\": \"{}\", \"l_commitdate\": \"{}\", "
            "\"l_receiptdate\": \"{}\", \"l_shipinstruct\": \"{}\", \"l_shipmode\": \"{}\", "
            "\"l_comment\": \"{}\"}}}}\n",
            action, std::stoul(fields[1]), std::stoul(fields[2]), std::stoul(fields[3]),
            std::stoul(fields[4]), std::stod(fields[5]), std::stod(fields[6]),
            std::stod(fields[7]), std::stod(fields[8]), fields[9], fields[10],
            fields[11], fields[12], fields[13], fields[14], fields[15], fields[16]);
}

void write_nation(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"n_nationkey\": {}, \"n_name\": \"{}\", "
            "\"n_regionkey\": {}, \"n_comment\": \"{}\"}}}}\n",
            action, std::stoul(fields[1]), fields[2], std::stoul(fields[3]), fields[4]);
}

void write_region(fmt::ostream &out, const std::string &row, const std::string &action) {
  auto fields = parse_row(row);
  out.print("{{\"{}\": {{\"r_regionkey\": {}, \"r_name\": \"{}\", "
            "\"r_comment\": \"{}\"}}}}\n",
            action, std::stoi(fields[1]), fields[2], fields[3]);
}

void convert(const std::string &query_id) {
  std::vector<std::string> rows;
  if (query_id == "3") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_c = fmt::output_file(output + "tpch" + query_id + "_customer.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");

    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_c.flush();
    out_o.flush();
    out_l.flush();
    out_a.close();
    out_c.close();
    out_o.close();
    out_l.close();
  } else if (query_id == "5") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_s = fmt::output_file(output + "tpch" + query_id + "_supplier.txt");
    auto out_c = fmt::output_file(output + "tpch" + query_id + "_customer.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");
    auto out_n = fmt::output_file(output + "tpch" + query_id + "_nation.txt");
    auto out_r = fmt::output_file(output + "tpch" + query_id + "_region.txt");

    read_table(input + "supplier.csv", SUPPLIER_ID, rows);
    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    read_table(input + "nation.csv", NATION_ID, rows);
    read_table(input + "region.csv", REGION_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(SUPPLIER_ID))) {
        write_supplier(out_s, row, "insert");
      } else if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      } else if (row.starts_with(std::to_string(NATION_ID))) {
        write_nation(out_n, row, "insert");
      } else if (row.starts_with(std::to_string(REGION_ID))) {
        write_region(out_r, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_s.flush();
    out_c.flush();
    out_o.flush();
    out_l.flush();
    out_n.flush();
    out_r.flush();

    out_a.close();
    out_s.close();
    out_c.close();
    out_o.close();
    out_l.close();
    out_n.close();
    out_r.close();
  } else if (query_id == "8") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_p = fmt::output_file(output + "tpch" + query_id + "_part.txt");
    auto out_s = fmt::output_file(output + "tpch" + query_id + "_supplier.txt");
    auto out_c = fmt::output_file(output + "tpch" + query_id + "_customer.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");
    auto out_n = fmt::output_file(output + "tpch" + query_id + "_nation.txt");
    auto out_r = fmt::output_file(output + "tpch" + query_id + "_region.txt");

    read_table(input + "part.csv", PART_ID, rows);
    read_table(input + "supplier.csv", SUPPLIER_ID, rows);
    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    read_table(input + "nation.csv", NATION_ID, rows);
    read_table(input + "region.csv", REGION_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(PART_ID))) {
        write_part(out_p, row, "insert");
      } else if (row.starts_with(std::to_string(SUPPLIER_ID))) {
        write_supplier(out_s, row, "insert");
      } else if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      } else if (row.starts_with(std::to_string(NATION_ID))) {
        write_nation(out_n, row, "insert");
      } else if (row.starts_with(std::to_string(REGION_ID))) {
        write_region(out_r, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_p.flush();
    out_s.flush();
    out_c.flush();
    out_o.flush();
    out_l.flush();
    out_n.flush();
    out_r.flush();

    out_a.close();
    out_p.close();
    out_s.close();
    out_c.close();
    out_o.close();
    out_l.close();
    out_n.close();
    out_r.close();
  } else if (query_id == "9") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_p = fmt::output_file(output + "tpch" + query_id + "_part.txt");
    auto out_s = fmt::output_file(output + "tpch" + query_id + "_supplier.txt");
    auto out_ps = fmt::output_file(output + "tpch" + query_id + "_partsupp.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");
    auto out_n = fmt::output_file(output + "tpch" + query_id + "_nation.txt");

    read_table(input + "part.csv", PART_ID, rows);
    read_table(input + "supplier.csv", SUPPLIER_ID, rows);
    read_table(input + "partsupp.csv", PARTSUPP_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    read_table(input + "nation.csv", NATION_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(PART_ID))) {
        write_part(out_p, row, "insert");
      } else if (row.starts_with(std::to_string(SUPPLIER_ID))) {
        write_supplier(out_s, row, "insert");
      } else if (row.starts_with(std::to_string(PARTSUPP_ID))) {
        write_partsupp(out_ps, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      } else if (row.starts_with(std::to_string(NATION_ID))) {
        write_nation(out_n, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_p.flush();
    out_s.flush();
    out_ps.flush();
    out_o.flush();
    out_l.flush();
    out_n.flush();

    out_a.close();
    out_p.close();
    out_s.close();
    out_ps.close();
    out_o.close();
    out_l.close();
    out_n.close();
  } else if (query_id == "10") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_c = fmt::output_file(output + "tpch" + query_id + "_customer.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");
    auto out_n = fmt::output_file(output + "tpch" + query_id + "_nation.txt");

    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    read_table(input + "nation.csv", NATION_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      } else if (row.starts_with(std::to_string(NATION_ID))) {
        write_nation(out_n, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_c.flush();
    out_o.flush();
    out_l.flush();
    out_n.flush();

    out_a.close();
    out_c.close();
    out_o.close();
    out_l.close();
    out_n.close();
  }
}

void convert_window(const std::string &query_id) {
  std::vector<std::string> rows;
  if (query_id == "3") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_c = fmt::output_file(output + "tpch" + query_id + "_customer.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");

    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    std::vector<std::vector<std::string>> buckets;
    unsigned long step = rows.size() / 10;
    for (unsigned long i = 0; i < 10; ++i) {
      buckets.emplace_back(std::vector<std::string>{});
    }

    for (unsigned long i = 0; i < rows.size(); ++i) {
      auto row = rows.at(i);
      unsigned long j = i / step;
      j = (j > 9 ? 9 : j);
      buckets.at(j).emplace_back(row);
    }

    for (unsigned long i = 0; i < 2; ++i) {
      for (auto row: buckets.at(i)) {
        out_a.print("+|{}\n", row);
        if (row.starts_with(std::to_string(CUSTOMER_ID))) {
          write_customer(out_c, row, "insert");
        } else if (row.starts_with(std::to_string(ORDERS_ID))) {
          write_orders(out_o, row, "insert");
        } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
          write_lineitem(out_l, row, "insert");
        }
      }
      out_a.print("*\n");
    }

    for (unsigned long i = 2; i < 10; ++i) {
      unsigned long j = 0;
      while (j < buckets.at(i).size()) {
        if (j < buckets.at(i - 2).size()) {
          auto row = buckets.at(i - 2).at(j);
          out_a.print("-|{}\n", row);
          if (row.starts_with(std::to_string(CUSTOMER_ID))) {
            write_customer(out_c, row, "delete");
          } else if (row.starts_with(std::to_string(ORDERS_ID))) {
            write_orders(out_o, row, "delete");
          } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
            write_lineitem(out_l, row, "delete");
          }
        }

        auto row = buckets.at(i).at(j);
        out_a.print("+|{}\n", row);
        if (row.starts_with(std::to_string(CUSTOMER_ID))) {
          write_customer(out_c, row, "insert");
        } else if (row.starts_with(std::to_string(ORDERS_ID))) {
          write_orders(out_o, row, "insert");
        } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
          write_lineitem(out_l, row, "insert");
        }
        j++;
      }
      out_a.print("*\n");
    }

    for (unsigned long i = 8; i < 10; ++i) {
      for (const auto &row: buckets.at(i)) {
        out_a.print("-|{}\n", row);
        if (row.starts_with(std::to_string(CUSTOMER_ID))) {
          write_customer(out_c, row, "delete");
        } else if (row.starts_with(std::to_string(ORDERS_ID))) {
          write_orders(out_o, row, "delete");
        } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
          write_lineitem(out_l, row, "delete");
        }
      }
      out_a.print("*\n");
    }

    out_a.flush();
    out_c.flush();
    out_o.flush();
    out_l.flush();
    out_a.close();
    out_c.close();
    out_o.close();
    out_l.close();
  }
}

void convert_dynamic(const std::string &query_id) {
  std::vector<std::string> rows;
  if (query_id == "3") {
    auto out_a = fmt::output_file(output + "tpch" + query_id + "_all.csv");
    auto out_c = fmt::output_file(output + "tpch" + query_id + "_customer.txt");
    auto out_o = fmt::output_file(output + "tpch" + query_id + "_orders.txt");
    auto out_l = fmt::output_file(output + "tpch" + query_id + "_lineitem.txt");

    read_table(input + "customer.csv", CUSTOMER_ID, rows);
    read_table(input + "orders.csv", ORDERS_ID, rows);
    read_table(input + "lineitem.csv", LINEITEM_ID, rows);
    std::shuffle(rows.begin(), rows.end(), rng);

    for (const auto &row: rows) {
      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      }
    }

    std::uniform_int_distribution<unsigned long> dist(0, rows.size() / 2);
    for (unsigned long i = 0; i < rows.size(); ++i) {
      unsigned long index = dist(rng);
      auto row = rows.at(index);

      out_a.print("-|{}\n", row);
      if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "delete");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "delete");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "delete");
      }

      out_a.print("+|{}\n", row);
      if (row.starts_with(std::to_string(CUSTOMER_ID))) {
        write_customer(out_c, row, "insert");
      } else if (row.starts_with(std::to_string(ORDERS_ID))) {
        write_orders(out_o, row, "insert");
      } else if (row.starts_with(std::to_string(LINEITEM_ID))) {
        write_lineitem(out_l, row, "insert");
      }
    }

    out_a.print("*\n");
    out_a.flush();
    out_c.flush();
    out_o.flush();
    out_l.flush();
    out_a.close();
    out_c.close();
    out_o.close();
    out_l.close();
  }
}

int main(int argc, char **argv) {
  parse_args(argc, argv);
  if (!window_enable && !dynamic_enable) {
    convert("3");
    convert("5");
    convert("8");
    convert("9");
    convert("10");
  } else if (window_enable) {
    convert_window("3");
  } else if (dynamic_enable) {
    convert_dynamic("3");
  }
  return 0;
}