#ifndef APPLICATION_TPCH
#define APPLICATION_TPCH

#include "../application.hpp"
#include "types.hpp"
#include <fstream>

void Application::process_streams(dbtoaster::data_t &data, Stopwatch &timer, std::string input) {
    std::ifstream in(input);
    std::string line;

    while (getline(in, line)) {
        if (line.at(0) == '*') {
            continue;
        } else {
            std::stringstream stream(line);
            std::string field;

            getline(stream, field, '|');
            bool is_insert = (field == "+");
            getline(stream, field, '|');
            unsigned long relation_id = std::stoul(field);
            if (relation_id == 3) {
                getline(stream, field, '|');
                int c_custkey = std::stoi(field);
                getline(stream, field, '|');
                STRING_TYPE c_name = STRING_TYPE(field);
                getline(stream, field, '|');
                STRING_TYPE c_address = STRING_TYPE(field);
                getline(stream, field, '|');
                int c_nationkey = std::stoi(field);
                getline(stream, field, '|');
                STRING_TYPE c_phone = STRING_TYPE(field);
                getline(stream, field, '|');
                double c_acctbal = std::stod(field);
                getline(stream, field, '|');
                STRING_TYPE c_mktsegment = STRING_TYPE(field);
                getline(stream, field, '|');
                STRING_TYPE c_comment = STRING_TYPE(field);

                if (is_insert) {
                    data.on_insert_CUSTOMER(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment,
                                            c_comment);
                } else {
                    data.on_delete_CUSTOMER(c_custkey, c_name, c_address, c_nationkey, c_phone, c_acctbal, c_mktsegment,
                                            c_comment);
                }
            } else if (relation_id == 4) {
                getline(stream, field, '|');
                int o_orderkey = std::stoi(field);
                getline(stream, field, '|');
                int o_custkey = std::stoi(field);
                getline(stream, field, '|');
                char o_orderstatus = field.at(0);
                getline(stream, field, '|');
                double o_totalprice = std::stod(field);
                getline(stream, field, '|');
                long o_orderdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                   std::stoul(field.substr(8, 10));
                getline(stream, field, '|');
                STRING_TYPE o_orderpriority = STRING_TYPE(field);
                getline(stream, field, '|');
                STRING_TYPE o_clerk = STRING_TYPE(field);
                getline(stream, field, '|');
                int o_shippriority = std::stoi(field);
                getline(stream, field, '|');
                STRING_TYPE o_comment = STRING_TYPE(field);

                if (is_insert) {
                    data.on_insert_ORDERS(o_orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate,
                                          o_orderpriority,
                                          o_clerk, o_shippriority, o_comment);
                } else {
                    data.on_delete_ORDERS(o_orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate,
                                          o_orderpriority,
                                          o_clerk, o_shippriority, o_comment);
                }
            } else if (relation_id == 5) {
                getline(stream, field, '|');
                int l_orderkey = std::stoi(field);
                getline(stream, field, '|');
                int l_partkey = std::stoi(field);
                getline(stream, field, '|');
                int l_suppkey = std::stoi(field);
                getline(stream, field, '|');
                int l_linenumber = std::stoi(field);
                getline(stream, field, '|');
                double l_quantity = std::stod(field);
                getline(stream, field, '|');
                double l_extendedprice = std::stod(field);
                getline(stream, field, '|');
                double l_discount = std::stod(field);
                getline(stream, field, '|');
                double l_tax = std::stod(field);
                getline(stream, field, '|');
                char l_returnflag = field.at(0);
                getline(stream, field, '|');
                char l_linestatus = field.at(0);
                getline(stream, field, '|');
                long l_shipdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                  std::stoul(field.substr(8, 10));
                getline(stream, field, '|');
                long l_commitdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                    std::stoul(field.substr(8, 10));
                getline(stream, field, '|');
                long l_receiptdate = std::stoul(field.substr(0, 4)) * 10000 + std::stoul(field.substr(5, 7)) * 100 +
                                     std::stoul(field.substr(8, 10));
                getline(stream, field, '|');
                STRING_TYPE l_shipinstruct = STRING_TYPE(field);
                getline(stream, field, '|');
                STRING_TYPE l_shipmode = STRING_TYPE(field);
                getline(stream, field, '|');
                STRING_TYPE l_comment = STRING_TYPE(field);

                if (is_insert) {
                    data.on_insert_LINEITEM(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice,
                                            l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate,
                                            l_receiptdate, l_shipinstruct, l_shipmode, l_comment);
                } else {
                    data.on_delete_LINEITEM(l_orderkey, l_partkey, l_suppkey, l_linenumber, l_quantity, l_extendedprice,
                                            l_discount, l_tax, l_returnflag, l_linestatus, l_shipdate, l_commitdate,
                                            l_receiptdate, l_shipinstruct, l_shipmode, l_comment);
                }
            } else if (relation_id == 6) {
                getline(stream, field, '|');
                int n_nationkey = std::stoi(field);
                getline(stream, field, '|');
                STRING_TYPE n_name = STRING_TYPE(field);
                getline(stream, field, '|');
                int n_regionkey = std::stoi(field);
                getline(stream, field, '|');
                STRING_TYPE n_comment = STRING_TYPE(field);

                if (is_insert) {
                    data.on_insert_NATION(n_nationkey, n_name, n_regionkey, n_comment);
                } else {
                    data.on_delete_NATION(n_nationkey, n_name, n_regionkey, n_comment);
                }
            } else {
                exit(-1);
            }
        }
    }
}

#endif