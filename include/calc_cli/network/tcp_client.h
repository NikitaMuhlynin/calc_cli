#pragma once

#include <string>

namespace calc_cli {

class TcpClient {
public:
    TcpClient(std::string host, std::string port);
    ~TcpClient() = default;

    TcpClient(const TcpClient&) = default;
    TcpClient& operator=(const TcpClient&) = default;

    TcpClient(TcpClient&&) noexcept = default;
    TcpClient& operator=(TcpClient&&) noexcept = default;

    std::string send(const std::string& request);

private:
    std::string host_;
    std::string port_;
};

}