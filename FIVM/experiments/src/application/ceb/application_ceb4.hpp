#ifndef APPLICATION_CEB
#define APPLICATION_CEB

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
            } else if (relation_id == 3) {
                getline(stream, field, '|');
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long movie_id = std::stoul(field);
                data.on_insert_MI(id, movie_id);
            } else if (relation_id == 4) {
                getline(stream, field, '|');
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long movie_id = std::stoul(field);
                data.on_insert_MII1(id, movie_id);
                data.on_insert_MII2(id, movie_id);
            } else if (relation_id == 5) {
                getline(stream, field, '|');
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long movie_id = std::stoul(field);
                data.on_insert_MK(id, movie_id);
            } else if (relation_id == 6) {
                getline(stream, field, '|');
                unsigned long id = std::stoul(field);
                getline(stream, field, '|');
                unsigned long person_id = std::stoul(field);
                data.on_insert_PI(id, person_id);
            } else {
                exit(-1);
            }
        }
    }
}

#endif