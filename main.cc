#include "main.h"

int     bindPort;
int     threadCount;

const int    DEFAULT_PORT     = 1572;
const int    DEFAULT_THREADS  = 4;

int main(int argc, const char** argv) {
    init();

    // create our options holder
    Options opts(argc, argv, "Basic Options");

    // register the options
    opts.add("help", "Display this help dialog");
    opts.add("port",
        po::value<int>(&bindPort)->default_value(DEFAULT_PORT),
        "Select which port to bind on");
    opts.add("threads",
        po::value<int>(&threadCount)->default_value(DEFAULT_THREADS),
        "Number of threads to use");

    // check if the program was given --help flag
    if (opts.has_command("help")) {
        print_build_info();
        std::cout << opts.description() << "\n";
        return 0;
    }

    try {
        // create a new server and let it bind to the port
        Server s(bindPort, threadCount);


        // create a signal set for the child, and a copy of
        // the set we have to restore later
        sigset_t   sig_child;
        sigset_t   sig_reset;

        // fill the entire signal set of the child
        sigfillset(&sig_child);

        // block all signal (child set to all, so block all)
        pthread_sigmask(SIG_BLOCK, &sig_child, &sig_reset);

        // start the server in a thread
        boost::thread t(boost::bind(&Server::start, &s));

        // listen to signals again
        pthread_sigmask(SIG_SETMASK, &sig_reset, 0);

        // wait for an 'approved' exit signal
        int code = wait();

        // stop the server 
        s.stop();
        t.join();
    } catch (std::exception& e) {
        std::cerr << "exception: " << e.what() << "\n";
    }

    return 0;
}

int wait() {
    // create another signal set
    sigset_t   is_done;
    sigemptyset(&is_done);
    sigaddset(&is_done, SIGINT);
    sigaddset(&is_done, SIGQUIT);
    sigaddset(&is_done, SIGTERM);
    pthread_sigmask(SIG_BLOCK, &is_done, 0);
    int sig = 0;
    
    // wait for a kill signal
    sigwait(&is_done, &sig);
    return sig;
}


//
// Initializes data and structs the program needs
//
void init() {
    proginfo.name = "Cubbie";
    proginfo.description = "evented key/value storage system";
    proginfo.major = 0;
    proginfo.minor = 0;
    proginfo.patch = 1;
}

//
// Prints the build info.
// Intended for use in the help command.
//
void print_build_info() {
    printf("%s: %s\nVersion %d.%d.%d\n\n",
        proginfo.name.c_str(), proginfo.description.c_str(),
        proginfo.major, proginfo.minor, proginfo.patch);
}
