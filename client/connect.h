#ifndef CONNECT_H
#define CONNECT_H

#include "config/server_config.h"

int tcp_send_request();

void *udp_send_request();

extern int g_nb_req;
extern string *g_message;
extern string *g_ip;
extern int g_port;

#endif
