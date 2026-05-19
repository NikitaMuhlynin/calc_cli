#include "calc_cli/application/runner.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {   
    try {
        const std::string connectionString =
            "host=localhost "
            "port=5432 "
            "dbname=calcli_cache "
            "user=calcli_user "
            "password=calcli";
        calc_cli::Runner runner(
            std::cout,
            std::cerr,
            connectionString);
        runner.run();
        return 0;

    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
        
    } catch (...) {
        std::cerr << "Unknown error\n";
        return 1;
    }
}