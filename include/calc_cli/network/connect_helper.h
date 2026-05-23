#include <iostream>
#include <cstdint>
#include <string>

namespace calc_cli {

class ConnectHelper {
public:
    inline static std::string getEnvOrDefault(
        const char* name,
        const std::string& defaultValue
    ) {
        const char* value = std::getenv(name);

        if (value == nullptr) {
            return defaultValue;
        }

        return value;
    }

    inline static std::string collectRequest(
        int argc,
        char* argv[],
        int startIndex
    ) {
        std::string request;

        for (int index = startIndex; index < argc; ++index) {
            if (index > startIndex) {
                request += ' ';
            }

            request += argv[index];
        }

        return request;
    }

    inline static std::uint16_t parsePort(const std::string& value) {
        const int port = std::stoi(value);

        if (port <= 0 || port > 65535) {
            throw std::invalid_argument("Error: port must be in range 1..65535");
        }

        return static_cast<std::uint16_t>(port);
    }

    inline static void printUsage() {
        std::cerr
            << "Usage:\n"
            << "  calc_cli --server [--port 5555]\n"
            << "  calc_cli --client [--host 127.0.0.1] [--port 5555] '<json>'\n"
            << "  calc_cli '<json>'\n";
    }

};

}