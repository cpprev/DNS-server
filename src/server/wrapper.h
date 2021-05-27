#ifndef WRAPPER_H
#define WRAPPER_H

#include <netdb.h>

#include "config/server_config.h"

#include "parser/options.h"

#include "server/protocol.h"

typedef struct
{
    server_config *cfg;
    options *opt;
} server_wrapper;

typedef struct
{
    int socket;
    server_wrapper s_wrapper;
} request_wrapper;

int get_addrinfo_wrapper(server_config *cfg, PROTOCOL proto);

void set_socket_non_blocking(int socket);

#endif
