#ifndef WRAPPER_H
#define WRAPPER_H

#include "config/server_config.h"

#include "parser/options.h"

typedef struct
{
    server_config *cfg;
    options *opt;
} wrapper;

wrapper wrapper_init(server_config *server_cfg, options *options);

#endif
