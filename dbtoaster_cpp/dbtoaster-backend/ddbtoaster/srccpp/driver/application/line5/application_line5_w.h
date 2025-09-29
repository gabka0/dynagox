#ifndef APPLICATION_LINE5_H
#define APPLICATION_LINE5_H

#include "../../application.hpp"
#include <fstream>

void Application::process_streams(dbtoaster::data_t &data, Stopwatch &timer, std::string input) {
    std::ifstream in(input);
    std::string line;

    while (getline(in, line)) {
        if (line.at(0) == '*') {
            print_running_time(timer);
            print_memory_usage();
            continue;
        } else {
            std::stringstream stream(line);
            std::string field;

            getline(stream, field, '|');
            if (field == "+") {
                getline(stream, field, '|');
                unsigned long src = std::stoul(field);
                getline(stream, field, '|');
                unsigned long dst = std::stoul(field);

                data.on_insert_R1(src, dst);
                data.on_insert_R2(src, dst);
                data.on_insert_R3(src, dst);
                data.on_insert_R4(src, dst);
                data.on_insert_R5(src, dst);
            } else {
                getline(stream, field, '|');
                unsigned long src = std::stoul(field);
                getline(stream, field, '|');
                unsigned long dst = std::stoul(field);

                data.on_delete_R1(src, dst);
                data.on_delete_R2(src, dst);
                data.on_delete_R3(src, dst);
                data.on_delete_R4(src, dst);
                data.on_delete_R5(src, dst);
            }
        }
    }
}

#endif
