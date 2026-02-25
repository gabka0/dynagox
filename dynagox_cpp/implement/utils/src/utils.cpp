#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#include <cstring>
#endif

void Utils::print_execution_time(
    std::ostream &output,
    std::chrono::time_point<std::chrono::system_clock> start,
    std::chrono::time_point<std::chrono::system_clock> end) {
  auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
  fmt::println(output, "Running Time: {} ms", time / 1000);
}

void Utils::print_memory_usage(std::ostream &output) {
#ifdef _WIN32
  PROCESS_MEMORY_COUNTERS_EX pmc;
  if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc))) {
    // Convert bytes to KB
    SIZE_T mem_kb = pmc.PeakWorkingSetSize / 1024;
    fmt::println(output, "Memory Usage: {} kB", mem_kb);
    return;
  }
#else
  FILE *file = fopen("/proc/self/status", "r");
  if (file != nullptr) {
    char line[128];

    while (fgets(line, 128, file) != nullptr) {
      if (!strncmp(line, "VmPeak:", 7)) {
        unsigned long mem = strtoul(line + 7, nullptr, 0);
        fmt::println(output, "Memory Usage: {} kB", mem);
        fclose(file);
        return;
      }
    }
    fclose(file);
  }
#endif

  fmt::println(output, "Memory Usage: N/A");
}

double Utils::get_b(double epsilon, double n) {
  return pow((1 + epsilon), 1.0 / n);
}
