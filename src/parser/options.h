#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdbool.h>

#include "utils/string.h"

typedef struct
{
    bool check;
    bool verbose;
    string *file;
} options;

options *options_init();

void options_free(options *o);

options *parse_options(int argc, char *argv[], string *error);

#endif
