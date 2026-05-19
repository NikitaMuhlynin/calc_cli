#include "calc_cli/application/printer.h"

namespace calc_cli {

void Printer::printResult(std::ostream& out, long long result) const {
    out << "Result: " << result << '\n';
}

void Printer::printHelp(std::ostream& out) const {
    out
        << "Usage:\n"
        << "  " <<  " '<json>'\n"
        << "  " <<  " '{\"help\": true}'\n\n"
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
        << "  " <<  " '{\"left\": 10, \"right\": 20, \"operation\": \"add\"}'\n"
        << "  " <<  " '{\"left\": 6, \"operation\": \"factorial\"}'\n"
        << "  " <<  " '{\"help\": true}'\n";
}

void Printer::printError(std::ostream& out, const std::string& message) const {
    out << message << '\n';
}

}


