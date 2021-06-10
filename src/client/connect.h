#ifndef CONNECT_H
#define CONNECT_H

#include "config/server_config.h"

#include "client/client_options.h"

int tcp_send_request();

void *udp_send_request();

extern int g_nb_req;
extern client_options *g_options;

#endif
