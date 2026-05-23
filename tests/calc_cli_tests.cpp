#include "calc_cli/application/runner.h"

#include <gtest/gtest.h>

#include <boost/asio.hpp>

#include <chrono>
#include <cstdint>
#include <istream>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <utility>

namespace {

class TestTcpClient {
public:
    TestTcpClient(std::string host, std::string port) 
        : host_{std::move(host)},
          port_{std::move(port)} {}
    
    std::string send(const std::string& request) const {
        boost::asio::io_context ioContext;

        boost::asio::ip::tcp::resolver resolver(ioContext);
        const auto endpoints = resolver.resolve(host_, port_);

        boost::asio::ip::tcp::socket socket(ioContext);
        boost::asio::connect(socket, endpoints);

        const std::string requestWithDelimiter = request + '\n';

        boost::asio::write(
            socket,
            boost::asio::buffer(requestWithDelimiter)
        );

        boost::asio::streambuf responseBuffer;

        boost::asio::read_until(
            socket,
            responseBuffer,
            '\n'
        );

        std::istream responseStream(&responseBuffer);

        std::string response;
        std::getline(responseStream, response);

        return response;
    }

private:
    std::string host_;
    std::string port_;

};

void waitUntilServerReady(
    const std::string& host,
    const std::string& port,
    std::chrono::milliseconds timeout
) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;

    while (std::chrono::steady_clock::now() < deadline) {
        boost::asio::io_context ioContext;
        boost::asio::ip::tcp::resolver resolver(ioContext);

        boost::system::error_code resolveError;
        const auto endpoints = resolver.resolve(host, port, resolveError);

        if (resolveError) {
            std::this_thread::yield();
            continue;
        }

        boost::asio::ip::tcp::socket socket(ioContext);

        boost::system::error_code connectError;
        boost::asio::connect(socket, endpoints, connectError);

        if (!connectError) {
            return;
        }

        std::this_thread::yield();
    }

    throw std::runtime_error("Server did not become ready before timeout");
}

class CalcCliNetworkTest : public ::testing::Test {
protected:
    void SetUp() override {
        runner_ = std::make_shared<calc_cli::Runner>(
            connectionString_,
            port_
        );

        serverThread_ = std::thread([runner = runner_]() {
            runner->run();
        });

        waitUntilServerReady(
            host_,
            std::to_string(port_),
            std::chrono::milliseconds(3000)
        );
    }

    void TearDown() override {
        if (runner_ != nullptr) {
            runner_->stop();
        }

        if (serverThread_.joinable()) {
            serverThread_.join();
        }
    }

protected:
    const std::string host_ = "127.0.0.1";
    const std::uint16_t port_ = 5556;

    const std::string connectionString_ =
        "host=localhost "
        "port=5432 "
        "dbname=calcli_cache "
        "user=calcli_user "
        "password=calcli";

    std::shared_ptr<calc_cli::Runner> runner_;
    std::thread serverThread_;

};

TEST_F(CalcCliNetworkTest, HelpRequested) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"help": true})"
    );
    
    EXPECT_EQ(response, "Usage:");
}

TEST_F(CalcCliNetworkTest, NormalAdd) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":1,"right":2,"operation":"add"})"
    );

    EXPECT_EQ(response, "Result: 3");
}

TEST_F(CalcCliNetworkTest, NormalSub) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":3,"right":2,"operation":"subtract"})"
    );

    EXPECT_EQ(response, "Result: 1");
}

TEST_F(CalcCliNetworkTest, NormalMul) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":3,"right":2,"operation":"multiply"})"
    );

    EXPECT_EQ(response, "Result: 6");
}

TEST_F(CalcCliNetworkTest, NormalDiv) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":8,"right":2,"operation":"divide"})"
    );

    EXPECT_EQ(response, "Result: 4");
}

TEST_F(CalcCliNetworkTest, NormalPow) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":8,"right":2,"operation":"power"})"
    );

    EXPECT_EQ(response, "Result: 64");
}

TEST_F(CalcCliNetworkTest, NormalFact) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":6,"operation":"factorial"})"
    );

    EXPECT_EQ(response, "Result: 720");
}

TEST_F(CalcCliNetworkTest, InvalidJSON) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"help"})"
    );

    EXPECT_NE(response.find("Error: invalid JSON input"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, InvalidOperation) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":16,"right":2,"operation":"squareroot"})"
    );

    EXPECT_NE(response.find("Error: unknown operation"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, LeftIsMissing) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"right":16,"operation":"add"})"
    );

    EXPECT_NE(response.find("Error: field 'left' is required"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, RightIsMissing) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":16,"operation":"add"})"
    );

    EXPECT_NE(response.find("Error: field 'right' is required"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, OperationIsMissing) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":16,"right":2})"
    );

    EXPECT_NE(response.find("Error: field 'operation' is required"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, DivisionByZero) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":16,"right":0,"operation":"divide"})"
    );

    EXPECT_NE(response.find("Error: division by zero"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, Overflow) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":21,"operation":"factorial"})"
    );

    EXPECT_NE(response.find("Error: multiplication overflow"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, NegativePow) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":2,"right":-2,"operation":"power"})"
    );

    EXPECT_NE(response.find("Error: negative exponent"), std::string::npos);
}

TEST_F(CalcCliNetworkTest, NegativeFactorial) {
    const TestTcpClient client(host_, std::to_string(port_));

    const std::string response = client.send(
        R"({"left":-5,"operation":"factorial"})"
    );

    EXPECT_NE(response.find("Error: negative factorial argument"), std::string::npos);
}


}