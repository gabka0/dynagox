#ifndef APPLICATION_JOB
#define APPLICATION_JOB

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
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long person_id = std::stoul(field);

                data.on_insert_AN(id, person_id);
            } else if (relation_id == 1) {
                getline(stream, field, '|');
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long person_id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long movie_id = std::stoul(field);

                data.on_insert_CI(person_id, movie_id);
            } else if (relation_id == 2) {
                getline(stream, field, '|');
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long movie_id = std::stoul(field);

                data.on_insert_MC(id, movie_id);
            } else {
                exit(-1);
            }
        }
    }
}

#endif