#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>

namespace io = boost::asio;

class iopool {
    private:
		typedef boost::shared_ptr<io::io_service>        io_service_ptr;
		typedef boost::shared_ptr<io::io_service::work>  work_ptr;

		std::size_t                  next_io;
		std::vector<work_ptr>        work;
		std::vector<io_service_ptr>  services;

    public:
		iopool(std::size_t pool_size);
		void run();
		void stop();
		io::io_service& next();
};
