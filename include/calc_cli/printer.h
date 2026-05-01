#include <ostream>
#include <string>

namespace calc_cli {

class Printer {
public:
    Printer() = default;
    ~Printer() = default;

    Printer(const Printer&) = default;
    Printer& operator=(const Printer&) = default;

    Printer(Printer&&) = default;
    Printer& operator=(Printer&&) = default;

    void printResult(std::ostream& out, long long result) const;
    void printHelp(std::ostream& out, const std::string& programName) const;
    void printError(std::ostream& out, const std::string& message) const;
};

}