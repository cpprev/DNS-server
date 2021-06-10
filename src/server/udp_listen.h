#ifndef UDP_LISTEN_H
#define UDP_LISTEN_H

#include "config/server_config.h"

#include "parser/options.h"

#define UDP_MTU 512
#define UDP_MESSAGE_SIZE (512 * 8)

#define UDP_MAX_EVENTS 10000

void *server_UDP_listen(void *args);

void udp_recvfrom(server_config *cfg, options *options, int udp_socket);

#endif
