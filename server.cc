#include "server.h"

namespace io = boost::asio;
namespace ip = boost::asio::ip;

//
// Binds the server to the given port on all IPv4 interfaces
//
Server::Server(int port): binding(ip::tcp::v4(), port) {
}

//
// Synchronous blocking function that accepts incoming
// connections and echoes the request back to the sender.
//
void Server::serve() {
    // create an io_service and use it to accept new
    // io events on the server-bound port/interface
    io::io_service     io_service;
    ip::tcp::acceptor  acceptor(io_service, binding);

    std::string        header       = "echo #%d\n=========\n";

    int count = 0;

    // try to go on forever, but catch exceptions when they occur
    try {
        for (;;) {
            // instantiate a new socket representation
            ip::tcp::socket  socket(io_service);

            // accept the connection from said socket
            acceptor.accept(socket);

            // wrap the connection
            Connection conn(socket);

            char* c = &conn.data[0];

            // formulate our echo
            std::stringstream ss;
            ss << boost::format(header) % count << c << std::endl;

            // write the echo and zero our data
            conn.write(ss.str());
            count += 1;
        }
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    };
}
