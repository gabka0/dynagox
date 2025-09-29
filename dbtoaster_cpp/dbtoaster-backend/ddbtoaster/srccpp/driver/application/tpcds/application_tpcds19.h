#ifndef APPLICATION_TPCDS
#define APPLICATION_TPCDS

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
            unsigned long relation_id = std::stoul(field);
            if (relation_id == 0) {
                getline(stream, field, '|');
                unsigned long ss_sold_date_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long ss_item_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long ss_customer_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long ss_store_sk = std::stoul(field);
                getline(stream, field, '|');
                double ss_ext_sales_price = std::stod(field);

                data.on_insert_STORE_SALES(ss_sold_date_sk, ss_item_sk, ss_customer_sk, ss_store_sk, ss_ext_sales_price);
            } else if (relation_id == 2) {
                getline(stream, field, '|');
                unsigned long s_store_sk = std::stoul(field);

                data.on_insert_STORE(s_store_sk);
            } else if (relation_id == 3) {
                getline(stream, field, '|');
                unsigned long c_customer_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long c_current_addr_sk = std::stoul(field);

                data.on_insert_CUSTOMER(c_customer_sk, c_current_addr_sk);
            } else if (relation_id == 4) {
                getline(stream, field, '|');
                unsigned long ca_address_sk = std::stoul(field);

                data.on_insert_CUSTOMER_ADDRESS(ca_address_sk);
            } else if (relation_id == 5) {
                getline(stream, field, '|');
                unsigned long d_date_sk = std::stoul(field);

                data.on_insert_DATE_DIM(d_date_sk);
            } else if (relation_id == 6) {
                getline(stream, field, '|');
                unsigned long i_item_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long i_brand_id = std::stoul(field);
                getline(stream, field, '|');
                StringType i_brand = StringType(field);
                getline(stream, field, '|');
                unsigned long i_manufact_id = std::stoul(field);
                getline(stream, field, '|');
                StringType i_manufact = StringType(field);

                data.on_insert_ITEM(i_item_sk, i_brand_id, i_brand, i_manufact_id, i_manufact);
            } else {
                exit(-1);
            }
        }
    }
}

#endif