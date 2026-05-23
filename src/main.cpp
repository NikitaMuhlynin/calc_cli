#include "calc_cli/application/runner.h"
#include "calc_cli/network/tcp_client.h"
#include "calc_cli/network/connect_helper.h"

#include <iostream>
#include <stdexcept>
#include <cstdint>
#include <string>

int main(int argc, char* argv[]) {
    try {
        const std::string defaultConnectionString =
            "host=localhost "
            "port=5432 "
            "dbname=calcli_cache "
            "user=calcli_user "
            "password=calcli";

        const std::string connectionString = 
            calc_cli::ConnectHelper::getEnvOrDefault(
            "CALC_CLI_CONNECTION_STRING",
            defaultConnectionString
        );

        const std::string defaultHost = 
            calc_cli::ConnectHelper::getEnvOrDefault(
            "CALC_CLI_HOST",
            "127.0.0.1"
        );

        const std::string defaultPort = 
            calc_cli::ConnectHelper::getEnvOrDefault(
            "CALC_CLI_PORT",
            "5555"
        );

        if (argc >= 2 && std::string(argv[1]) == "--server") {
            std::uint16_t port = calc_cli::ConnectHelper::parsePort(defaultPort);

            for (int index = 2; index < argc; ++index) {
                const std::string argument = argv[index];

                if (argument == "--port") {
                    if (index + 1 >= argc) {
                        throw std::invalid_argument("Error: --port value is missing");
                    }

                    port = calc_cli::ConnectHelper::parsePort(argv[index + 1]);
                    ++index;
                    continue;
                }

                throw std::invalid_argument("Error: unknown server argument: " + argument);
            }

            auto runner = std::make_shared<calc_cli::Runner>(
                connectionString,
                port
            );

            runner->run();

            return 0;
        }

        if (argc >= 2 && std::string(argv[1]) == "--client") {
            std::string host = defaultHost;
            std::string port = defaultPort;
            int requestStartIndex = -1;

            for (int index = 2; index < argc; ++index) {
                const std::string argument = argv[index];

                if (argument == "--host") {
                    if (index + 1 >= argc) {
                        throw std::invalid_argument("Error: --host value is missing");
                    }

                    host = argv[index + 1];
                    ++index;
                    continue;
                }

                if (argument == "--port") {
                    if (index + 1 >= argc) {
                        throw std::invalid_argument("Error: --port value is missing");
                    }

                    port = argv[index + 1];
                    ++index;
                    continue;
                }

                requestStartIndex = index;
                break;
            }

            if (requestStartIndex == -1) {
                throw std::invalid_argument("Error: JSON request is missing");
            }

            const std::string request = calc_cli::ConnectHelper::collectRequest(
                argc,
                argv,
                requestStartIndex
            );

            calc_cli::TcpClient client(host, port);

            const std::string response = client.send(request);

            std::cout << response << '\n';

            return 0;
        }

        if (argc >= 2) {
            const std::string request = calc_cli::ConnectHelper::collectRequest(argc, argv, 1);

            calc_cli::TcpClient client(defaultHost, defaultPort);

            const std::string response = client.send(request);

            std::cout << response << '\n';

            return 0;
        }

        calc_cli::ConnectHelper::printUsage();
        return 1;
    } catch (const std::exception& exception) {
        std::cerr << exception.what() << '\n';
        return 1;
    } catch (...) {
        std::cerr << "Unknown error\n";
        return 1;
    }
}