#include "calc_cli/application/runner.h"
#include "calc_cli/network/TcpClient.h"

#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <string>

namespace {

std::string collectRequest(int argc, char** argv, int startIndex) {
    std::string request;

    for (int index = startIndex; index < argc; ++index) {
        if (index > startIndex) {
            request += ' ';
        }

        request += argv[index];
    }

    return request;
}

}

int main(int argc, char** argv) {   
    try {
        const std::string connectionString =
            "host=localhost "
            "port=5432 "
            "dbname=calcli_cache "
            "user=calcli_user "
            "password=calcli";

        if (argc >= 2 && std::string(argv[1]) == "--server") {
            std::uint16_t port = 5555;

            for (int index = 2; index < argc; ++index) {
                std::string argument = argv[index];

                if (argument == "--port" && index + 1 < argc) {
                    port = static_cast<std::uint16_t>(
                        std::stoi(argv[index + 1])
                    );
                    ++index;
                }
            }

            calc_cli::Runner runner(
                connectionString,
                port
            );

            runner.run();

            return 0;
        }

        if (argc >= 2 && std::string(argv[1]) == "--client") {
            std::string host = "127.0.0.1";
            std::string port = "5555";
            int requestStartIndex = -1;

            for (int index = 2; index < argc; ++index) {
                std::string argument = argv[index];

                if (argument == "--host" && index + 1 < argc) {
                    host = argv[index + 1];
                    ++index;
                    continue;
                }

                if (argument == "--port" && index + 1 < argc) {
                    port = argv[index + 1];
                    ++index;
                    continue;
                }

                requestStartIndex = index;
                break;
            }

            if (requestStartIndex == -1) {
                throw std::invalid_argument("Error: JSON request is required");
            }

            std::string request = collectRequest(argc, argv, requestStartIndex);

            calc_cli::TcpClient client(host, port);

            std::string response = client.send(request);

            std::cout << response << '\n';

            return 0;
        }

        if (argc >= 2) {
            std::string request = collectRequest(argc, argv, 1);

            calc_cli::TcpClient client("127.0.0.1", "5555");

            std::string response = client.send(request);

            std::cout << response << '\n';

            return 0;
        }

        std::cerr << "Usage:\n";
        std::cerr << "  calc_cli --server --port 5555\n";
        std::cerr << "  calc_cli --client --host 127.0.0.1 --port 5555 '{\"left\":2,\"right\":3,\"operation\":\"add\"}'\n";
        std::cerr << "  calc_cli '{\"left\":2,\"right\":3,\"operation\":\"add\"}'\n";

        return 1;

    } catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
        return 1;
        
    } catch (...) {
        std::cerr << "Unknown error\n";
        return 1;
    }
}