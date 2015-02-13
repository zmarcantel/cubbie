#include "thread_pool.h"

//
// Create a pool of io_services to handle connections on
//
iopool::iopool(std::size_t pool_size) {
    if (pool_size == 0) {
        throw std::runtime_error("must start at least one thread");
    }

    // create io_services and work handles
    // push them into the list
	for (std::size_t i = 0; i < pool_size; ++i) {
		io_service_ptr    io_service(new io::io_service);
		work_ptr          w(new io::io_service::work(*io_service));
		services.push_back(io_service);
		work.push_back(w);
	}
}

//
// Start a thread for every io_Service
//
void iopool::run() {
	std::vector<boost::shared_ptr<boost::thread> > threads;

    // start new threads dedicated to the io_service
	for (std::size_t i = 0; i < services.size(); ++i) {
	    boost::shared_ptr<boost::thread> thread(new boost::thread(
	        boost::bind(&io::io_service::run, services[i])));
	    threads.push_back(thread);
	}

    // block waiting for everything to exit
    // TODO: do not wait for all threads, handle indiv'ly and restart
	for (std::size_t i = 0; i < threads.size(); ++i) {
        threads[i]->join();
    }
}

//
// Stop the io_service threads
//
void iopool::stop() {
    for (std::size_t i = 0; i < services.size(); ++i) {
        services[i]->stop();
    }
}

//
// Fetches the next io_service thread.
// This is where all scheduling will happen.
// Current scheduler is round robin
//
io::io_service& iopool::next() {
    // get and increment
    io::io_service& io_service = *services[next_io++];

    // check for rollover
    if (next_io == services.size()) {
        next_io = 0;
    }
    return io_service;
}
