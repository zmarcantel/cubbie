#include <stdio.h>
#include <iostream>

#include "options.h"
#include "server.h"

typedef struct {
    std::string   name;
    std::string   description;
    int           major, minor, patch;
} program_info;

program_info proginfo;

void init();
void print_build_info();
