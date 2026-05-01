#include "calc_cli/runner.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {   
    try {
        calc_cli::Runner runner(std::cout, std::cerr);
        return runner.run(argc, argv);
    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
}