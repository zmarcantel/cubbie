#include <iostream>
#include <boost/asio.hpp>
#include <boost/format.hpp>

#include "connection.h"

namespace ip = boost::asio::ip;

class Server {
    private:
        ip::tcp::endpoint  binding;

    public:
        Server(int);
        void serve();
};
