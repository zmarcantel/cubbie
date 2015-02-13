#include <iostream>
#include <boost/thread.hpp>

#include "options.h"
#include "server.h"

typedef struct {
    std::string   name;
    std::string   description;
    int           major, minor, patch;
} program_info;

program_info proginfo;

void init();
int  wait();
void print_build_info();
