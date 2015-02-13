#include "connection.h"

namespace io = boost::asio;
namespace ip = boost::asio::ip;
namespace sys = boost::system;

//
// Create a connection on the given socket.
// This function also reads data off the socket and into
// the instance's data array.
//
Connection::Connection(io::io_service& s): sock(s) {
}

ip::tcp::socket& Connection::socket() {
    return sock;
}

void Connection::start() {
    // TODO: read_some will not read all data by design,
    //       it actually just reads SOME. For an echo server
    //       this is fine, but should be wrapped in a loop
    //       until EOF, sys::errc::success, or explicit error
    sock.async_read_some(io::buffer(data),
        boost::bind(&Connection::read,
            shared_from_this(),
            io::placeholders::error,
            io::placeholders::bytes_transferred));
}

//
// Read from the connection.
//
// Notes:
//   1. As the above todo states, this method assumes a single
//      1k buffer will hold the message and that a single call
//      to read_some will read the entire message. Should be
//      expanded to check boundries as well as wait for EOF
//      or other handleable error code
//   2. Because of (1), this function writes back the echo
//      rather that delegating to a handler as is the plan
//
int count = 0;
std::string   header = "echo #%d\n=========\n";
void Connection::read(const sys::error_code& e, std::size_t bytes_transferred) {

    char* c = &data[0];
    std::stringstream ss;
    ss << boost::format(header) % count << c;

    write(ss.str());
    count += 1;
}

//
// Set the error for the connection.
// Does not write, but the call to .write() will write the error
//
void Connection::set_error(sys::error_code err) {
    error = err;
}

//
// Write a string to the connection's socket.
// The instance's error value is written as well. If there was
// no error set using the .set_error() function, it's ignored.
//
void Connection::write(std::string msg) {
    io::write(sock, io::buffer(msg), io::transfer_all(), error);

    sys::error_code e;
    sock.shutdown(io::ip::tcp::socket::shutdown_both, e);
}

