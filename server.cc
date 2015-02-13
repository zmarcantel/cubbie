#include "server.h"

namespace io = boost::asio;
namespace ip = boost::asio::ip;

//
// Binds the server to the given port on all IPv4 interfaces
//
Server::Server(int port, int threads):
pool(threads), acceptor(pool.next()), conn(new Connection(pool.next())) {
    // bind to all ipv4 interfaces on the given port
    ip::tcp::endpoint    binding(ip::tcp::v4(), port);

    // allow multiple acceptors and bind to the endpoint
    acceptor.open(binding.protocol());
    acceptor.set_option(ip::tcp::acceptor::reuse_address(true));
    acceptor.bind(binding);

    // listen and kickoff the main loop
    acceptor.listen();
    acceptor.async_accept(conn->socket(),
        boost::bind(&Server::handler, this, io::placeholders::error));
}

//
// Start the server.
// Spins up the thread/io_service pool and begins serving requests
//
void Server::start() {
    std::cout << "starting iopool" << std::endl;
    pool.run();
}

//
// Stops the server.
//
void Server::stop() {
    std::cout << "stopping iopool" << std::endl;
    pool.stop();
}

//
// Handler for connections.
// Starts a connection which will handle the connection flow.
// Resets the connection after handling and schedules another accept.
//
// Note: conn->start() __must__ be synchronous within the scope of this function
// TODO: check for above case being false
//
void Server::handler(const sys::error_code& e) {
    if (!e) {
        conn->start();
        conn.reset(new Connection(pool.next()));
        acceptor.async_accept(conn->socket(),
            boost::bind(&Server::handler, this,
                io::placeholders::error));
    }
}
