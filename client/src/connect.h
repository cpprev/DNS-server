#ifndef CONNECT_H
#define CONNECT_H

#include "config/server_config.h"

void tcp_send_request(server_config *cfg);

void udp_send_request(server_config *cfg);

#endif
