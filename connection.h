#include <boost/asio.hpp>

namespace io = boost::asio;
namespace ip = boost::asio::ip;
namespace sys = boost::system;

class Connection {
    private:
        sys::error_code    error;
        ip::tcp::socket&   socket;

    public:
        // read into a static 1K buffer until improved upon
        boost::array<char, 1024> data;

        Connection(ip::tcp::socket&);
        void set_error(sys::error_code);
        void write(std::string);
};

