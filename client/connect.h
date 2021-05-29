#ifndef CONNECT_H
#define CONNECT_H

#include "config/server_config.h"

void tcp_send_request(string *ip, int port);

void udp_send_request(string *ip, int port);

#endif
