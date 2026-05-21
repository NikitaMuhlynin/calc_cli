#include "calc_cli/network/TcpClient.h"

#include <boost/asio.hpp>

#include <istream>
#include <string>
#include <utility>

namespace calc_cli {

TcpClient::TcpClient(std::string host, std::string port)
    : host_{std::move(host)},
      port_{std::move(port)} {}

std::string TcpClient::send(const std::string& request) {
    boost::asio::io_context ioContext;

    boost::asio::ip::tcp::resolver resolver(ioContext);

    boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve(host_, port_);

    boost::asio::ip::tcp::socket socket(ioContext);

    boost::asio::connect(socket, endpoints);

    std::string requestWithDelimiter = request + '\n';

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

}