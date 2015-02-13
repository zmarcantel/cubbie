#include "main.h"

int          bindPort;

int main(int argc, const char** argv) {
    init();

    // create our options holder
    Options opts(argc, argv, "Basic Options");

    // register the options
    opts.add("help", "Display this help dialog");
    opts.add("port",
        po::value<int>(&bindPort)->default_value(1572),
        "Select which port to bind on");

    // check if the program was given --help flag
    if (opts.has_command("help")) {
        print_build_info();
        std::cout << opts.description() << "\n";
        return 0;
    }

    // get the port given (or default)
    int port = opts.get_int("port");

    // create a new server and let it bind to the port
    Server s(port);

    // blocking serve
    s.serve();

    return 0;
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
