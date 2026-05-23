#include "calc_cli/application/printer.h"

namespace calc_cli {

std::string Printer::printResult(long long result) const {
    return "Result: " + std::to_string(result);
}

std::string Printer::printHelp() const {
    return
         "Usage:\n"
           " '<json>'\n"
           " '{\"help\": true}'\n\n"
         "Description:\n"
         "  calc_cli performs safe integer arithmetic using JSON input.\n\n"
         "Supported operations:\n"
         "  add, subtract, multiply, divide, power, factorial\n\n"
         "JSON format:\n"
         "  Binary operations:\n"
         "    {\"left\": <int>, \"right\": <int>, \"operation\": \"add\"}\n"
         "  Factorial:\n"
         "    {\"left\": <int>, \"operation\": \"factorial\"}\n"
         "  Help:\n"
         "    {\"help\": true}\n\n"
         "Examples:\n"
           " '{\"left\": 10, \"right\": 20, \"operation\": \"add\"}'\n"
           " '{\"left\": 6, \"operation\": \"factorial\"}'\n"
           " '{\"help\": true}'\n";
}

std::string Printer::printError(const std::string& message) const {
    return "Error: " + message;
}

}


