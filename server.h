#include <iostream>
#include <boost/asio.hpp>

#include "connection.h"
#include "thread_pool.h"

namespace ip = boost::asio::ip;

class Server {
    private:
        iopool             pool;
        ip::tcp::acceptor  acceptor;
        conn_ptr           conn;

    public:
        Server(int, int);
        void start();
        void stop();
        void handler(const sys::error_code&);
};
