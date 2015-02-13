#include "connection.h"

namespace io = boost::asio;
namespace ip = boost::asio::ip;
namespace sys = boost::system;

//
// Create a connection on the given socket.
// This function also reads data off the socket and into
// the instance's data array.
//
Connection::Connection(ip::tcp::socket & s): socket(s) {
    // TODO: read_some will not read all data by design,
    //       it actually just reads SOME. For an echo server
    //       this is fine, but should be wrapped in a loop
    //       until EOF, sys::errc::success, or explicit error
    size_t length = socket.read_some(io::buffer(data), error);

    // check for a non EOF error
    if (error != sys::errc::success && error != io::error::eof) {
        // write the error to the connection
        io::write(socket, io::buffer(""), io::transfer_all(), error);

        // and throw it
        throw boost::system::system_error(error);
    }

    return;
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
    io::write(socket, io::buffer(msg), io::transfer_all(), error);
}

