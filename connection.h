#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/enable_shared_from_this.hpp>

namespace io = boost::asio;
namespace ip = boost::asio::ip;
namespace sys = boost::system;

class Connection:
public boost::enable_shared_from_this<Connection>,
private boost::noncopyable {
    private:
        sys::error_code    error;
        ip::tcp::socket    sock;

        // read into a static 1K buffer until improved upon
        boost::array<char, 1024> data;


    public:
        Connection(io::io_service&);
        void start();
        void read(const sys::error_code&, std::size_t);
        void set_error(sys::error_code);
        void write(std::string);
        ip::tcp::socket& socket();
};

typedef boost::shared_ptr<Connection> conn_ptr;
