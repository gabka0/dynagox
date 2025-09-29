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
            if (relation_id == 1) {
                getline(stream, field, '|');
                unsigned long cs_sold_date_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long cs_bill_customer_sk = std::stoul(field);
                getline(stream, field, '|');
                double cs_sales_price = std::stod(field);

                data.on_insert_CATALOG_SALES(cs_sold_date_sk, cs_bill_customer_sk, cs_sales_price);
            } else if (relation_id == 3) {
                getline(stream, field, '|');
                unsigned long c_customer_sk = std::stoul(field);
                getline(stream, field, '|');
                unsigned long c_current_addr_sk = std::stoul(field);

                data.on_insert_CUSTOMER(c_customer_sk, c_current_addr_sk);
            } else if (relation_id == 4) {
                getline(stream, field, '|');
                unsigned long ca_address_sk = std::stoul(field);
                getline(stream, field, '|');
                StringType ca_zip = StringType(field);

                data.on_insert_CUSTOMER_ADDRESS(ca_address_sk, ca_zip);
            } else if (relation_id == 5) {
                getline(stream, field, '|');
                unsigned long d_date_sk = std::stoul(field);

                data.on_insert_DATE_DIM(d_date_sk);
            } else {
                exit(-1);
            }
        }
    }
}

#endif