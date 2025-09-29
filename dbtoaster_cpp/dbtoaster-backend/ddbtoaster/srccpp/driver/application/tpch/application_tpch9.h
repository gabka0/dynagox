#ifndef APPLICATION_TPCH
#define APPLICATION_TPCH

#include "../../application.hpp"
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
            if (relation_id == 0) {
                getline(stream, field, '|');
                int p_partkey = std::stoi(field);
                getline(stream, field, '|');
                StringType p_name = StringType(field);
                getline(stream, field, '|');
                StringType p_mfgr = StringType(field);
                getline(stream, field, '|');
                StringType p_brand = StringType(field);
                getline(stream, field, '|');
                StringType p_type = StringType(field);
                getline(stream, field, '|');
                int p_size = std::stoi(field);
                getline(stream, field, '|');
                StringType p_container = StringType(field);
                getline(stream, field, '|');
                double p_retailprice = std::stod(field);
                getline(stream, field, '|');
                StringType p_comment = StringType(field);

                if (is_insert) {
                    data.on_insert_PART(p_partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice,
                                        p_comment);
                } else {
                    data.on_delete_PART(p_partkey, p_name, p_mfgr, p_brand, p_type, p_size, p_container, p_retailprice,
                                        p_comment);
                }
            } else if (relation_id == 1) {
                getline(stream, field, '|');
                int s_suppkey = std::stoi(field);
                getline(stream, field, '|');
                StringType s_name = StringType(field);
                getline(stream, field, '|');
                StringType s_address = StringType(field);
                getline(stream, field, '|');
                int s_nationkey = std::stoi(field);
                getline(stream, field, '|');
                StringType s_phone = StringType(field);
                getline(stream, field, '|');
                double s_acctbal = std::stod(field);
                getline(stream, field, '|');
                StringType s_comment = StringType(field);

                if (is_insert) {
                    data.on_insert_SUPPLIER(s_suppkey, s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment);
                } else {
                    data.on_delete_SUPPLIER(s_suppkey, s_name, s_address, s_nationkey, s_phone, s_acctbal, s_comment);
                }
            } else if (relation_id == 2) {
                getline(stream, field, '|');
                int ps_partkey = std::stoi(field);
                getline(stream, field, '|');
                int ps_suppkey = std::stoi(field);
                getline(stream, field, '|');
                int ps_availqty = std::stoi(field);
                getline(stream, field, '|');
                double ps_supplycost = std::stod(field);
                getline(stream, field, '|');
                StringType ps_comment = StringType(field);

                if (is_insert) {
                    data.on_insert_PARTSUPP(ps_partkey, ps_suppkey, ps_availqty, ps_supplycost, ps_comment);
                } else {
                    data.on_delete_PARTSUPP(ps_partkey, ps_suppkey, ps_availqty, ps_supplycost, ps_comment);
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
                int o_year = std::stoi(field.substr(0, 4));
                DateType o_orderdate = DateType(std::stoi(field.substr(0, 4)), std::stoi(field.substr(5, 7)),
                                                std::stoi(field.substr(8, 10)));
                getline(stream, field, '|');
                StringType o_orderpriority = StringType(field);
                getline(stream, field, '|');
                StringType o_clerk = StringType(field);
                getline(stream, field, '|');
                int o_shippriority = std::stoi(field);
                getline(stream, field, '|');
                StringType o_comment = StringType(field);

                if (is_insert) {
                    data.on_insert_ORDERS(o_orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate,
                                          o_orderpriority,
                                          o_clerk, o_shippriority, o_comment, o_year);
                } else {
                    data.on_delete_ORDERS(o_orderkey, o_custkey, o_orderstatus, o_totalprice, o_orderdate,
                                          o_orderpriority,
                                          o_clerk, o_shippriority, o_comment, o_year);
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
                DateType l_shipdate = DateType(std::stoi(field.substr(0, 4)), std::stoi(field.substr(5, 7)),
                                               std::stoi(field.substr(8, 10)));
                getline(stream, field, '|');
                DateType l_commitdate = DateType(std::stoi(field.substr(0, 4)), std::stoi(field.substr(5, 7)),
                                                 std::stoi(field.substr(8, 10)));
                getline(stream, field, '|');
                DateType l_receiptdate = DateType(std::stoi(field.substr(0, 4)), std::stoi(field.substr(5, 7)),
                                                  std::stoi(field.substr(8, 10)));
                getline(stream, field, '|');
                StringType l_shipinstruct = StringType(field);
                getline(stream, field, '|');
                StringType l_shipmode = StringType(field);
                getline(stream, field, '|');
                StringType l_comment = StringType(field);

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
                StringType n_name = StringType(field);
                getline(stream, field, '|');
                int n_regionkey = std::stoi(field);
                getline(stream, field, '|');
                StringType n_comment = StringType(field);

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