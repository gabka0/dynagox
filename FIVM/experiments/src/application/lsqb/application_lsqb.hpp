#ifndef APPLICATION_LSQB
#define APPLICATION_LSQB

#include "../application.hpp"
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
                unsigned long person1_id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long person2_id = std::stoul(field);

                data.on_insert_PKP1(person1_id, person2_id);
                data.on_insert_PKP2(person1_id, person2_id);
            } else if (relation_id == 1) {
                getline(stream, field, '|');
                unsigned long person_id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long tag_id = std::stoul(field);

                data.on_insert_PHIT(person_id, tag_id);
            } else {
                exit(-1);
            }
        }
    }
}

#endif