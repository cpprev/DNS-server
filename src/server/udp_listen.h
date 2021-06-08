#ifndef UDP_LISTEN_H
#define UDP_LISTEN_H

#include "config/server_config.h"

#include "parser/options.h"

void *server_UDP_listen(void *args);

void udp_recvfrom(server_config *cfg, options *options, int udp_socket);

#endif
