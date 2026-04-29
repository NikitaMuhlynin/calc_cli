#include "calc_cli/printer.h"

#include <iostream>

namespace calc_cli {

void Printer::print_help(const char* programName) {
    std::cout
        << "Usage:\n"
        << "  " << programName << " '<json>'\n"
        << "  " << programName << " '{\"help\": true}'\n\n"
        << "Description:\n"
        << "  calc_cli performs safe integer arithmetic using JSON input.\n\n"
        << "Supported operations:\n"
        << "  add, subtract, multiply, divide, power, factorial\n\n"
        << "JSON format:\n"
        << "  Binary operations:\n"
        << "    {\"left\": <int>, \"right\": <int>, \"operation\": \"add\"}\n"
        << "  Factorial:\n"
        << "    {\"left\": <int>, \"operation\": \"factorial\"}\n"
        << "  Help:\n"
        << "    {\"help\": true}\n\n"
        << "Examples:\n"
        << "  " << programName << " '{\"left\": 10, \"right\": 20, \"operation\": \"add\"}'\n"
        << "  " << programName << " '{\"left\": 6, \"operation\": \"factorial\"}'\n"
        << "  " << programName << " '{\"help\": true}'\n";
}

void Printer::print_result(long long result) {
    std::cout << "Result: " << result << '\n';
}

void Printer::print_error(const std::string& message) {
    std::cerr << message << '\n';
}

}