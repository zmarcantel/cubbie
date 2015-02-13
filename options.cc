#include "options.h"

void printAdditionError(po::error e) {
    std::cout << "command addition error: " << e.what() << std::endl;
}

//
// Construct the options holder from argc, argv and the
// top level description/category we desire
//
Options::Options(int c, const char** v, std::string d): argc(c), argv(v), desc(d) {
}

//
// Determines if the given command/argument was supplied
// in the arguments to the running program
//
bool Options::has_command(std::string cmd) {
    return varmap.count(cmd);
}

//
// Returns the options description for the help menu
// or in case of needing direct access
//
po::options_description& Options::description() {
    return desc;
}

//
// Adds a flag and description
//
void Options::add(const char* d, const char* h) {
    try {
        desc.add_options()(d, h);
        update();
    } catch(po::error e) {
        // TODO: handle, not just print
        printAdditionError(e);
    }
}

//
// Adds an argument with the given value_semantic
//
void Options::add(const char * d, const po::value_semantic * v) {
    try {
        desc.add_options()(d, v);
        update();
    } catch(po::error e) {
        // TODO: handle, not just print
        printAdditionError(e);
    }
}

//
// Adds an argument with the given value_semantic and description
//
void Options::add(const char * d, const po::value_semantic * v, const char * h) {
    try {
        desc.add_options()(d, v, h);
        update();
    } catch(po::error e) {
        // TODO: handle, not just print
        printAdditionError(e);
    }
}

int Options::get_int(std::string key) {
    return varmap[key].as<int>();
}

//
// Update the options after and addition or some manual
// operation on the result of a .description() call
//
void Options::update() {
    try {
        po::store(po::parse_command_line(argc, argv, desc), varmap);
        po::notify(varmap);
    } catch (std::exception &e) {
        std::cout << "update error: " << e.what() << std::endl;
    }
}
