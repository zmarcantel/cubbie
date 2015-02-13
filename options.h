#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Options {
    private:
        int argc;
        const char** argv;
        po::variables_map varmap;
        po::options_description desc;
        void update();

    public:
        Options(int, const char**, std::string);
        po::options_description& description();

        bool has_command(std::string cmd);
        void add(const char* d, const char* h);
        void add(const char * d, const po::value_semantic * v);
        void add(const char * d, const po::value_semantic * v, const char * h);
        int get_int(std::string key);
};

