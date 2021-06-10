#ifndef CLIENT_OPTIONS_H
#define CLIENT_OPTIONS_H

#include "server/protocol.h"

typedef struct
{
    string *ip;
    int port;
    void *message;
    size_t message_size;
} client_options;

client_options *client_options_init(PROTOCOL proto);

void client_options_free(client_options *opt);

client_options *parse_client_options(int argc, char *argv[], PROTOCOL proto);

#endif
