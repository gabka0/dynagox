#include "application.hpp"

int main(int argc, char** argv) {
    std::string opt_input = "";
    bool opt_print_result = true;
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--input-path") == 0 || strcmp(argv[i], "-i") == 0) {
            opt_input = argv[i + 1];
        }
        opt_print_result = opt_print_result && (strcmp(argv[i], "--no-output") != 0);
    }

    Application app;
    app.run(opt_input, opt_print_result);

    return 0;
}