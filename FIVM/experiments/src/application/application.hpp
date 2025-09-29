#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <iostream>
#include <sys/time.h>
#include "stopwatch.hpp"

using namespace dbtoaster;

class Application {
protected:
    void on_begin_processing(dbtoaster::data_t &data);

    void process_streams(dbtoaster::data_t &data, Stopwatch &timer, std::string input);

    void on_end_processing(dbtoaster::data_t &data, bool print_result);

    void print_running_time(Stopwatch &timer);

    void print_memory_usage();

    void print_query_result(dbtoaster::data_t &data);

public:
    Application() {}

    ~Application() {}

    void run(std::string input, bool print_result);
};

void Application::run(std::string input, bool print_result) {
    Stopwatch timer;
    dbtoaster::data_t data;

    timer.restart();

    on_begin_processing(data);
    process_streams(data, timer, input);
    on_end_processing(data, print_result);

    print_running_time(timer);
    print_memory_usage();
}

void Application::on_begin_processing(dbtoaster::data_t &data) {

}

void Application::on_end_processing(dbtoaster::data_t &data, bool print_result) {
    if (print_result) {
        print_query_result(data);
    }
}

void Application::print_running_time(Stopwatch &timer) {
    timer.stop();
    std::cout << "Running Time: " << timer.elapsedTimeInMilliSeconds() << " ms" << std::endl;
}

void Application::print_memory_usage() {
    FILE *file = fopen("/proc/self/status", "r");
    if (file != NULL) {
        char line[128];

        while (fgets(line, 128, file) != nullptr) {
            if (!strncmp(line, "VmPeak:", 7)) {
                unsigned long mem = strtoul(line + 7, nullptr, 0);
                std::cout << "Memory Usage: " << mem << " kB" << std::endl;
                return;
            }
        }
    }

    std::cout << "Memory Usage: N/A" << std::endl;
    return;
}

void Application::print_query_result(dbtoaster::data_t &data) {
    data.serialize(std::cout, 0);
    std::cout << std::endl;
}

#endif
