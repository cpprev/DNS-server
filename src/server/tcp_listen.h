#ifndef TCP_LISTEN_H
#define TCP_LISTEN_H

#include "config/server_config.h"

#include "parser/options.h"

#define TCP_READ_SIZE 10
#define TCP_MAX_EVENTS 10000

void *server_TCP_listen(void *args);

void tcp_accept(int epoll_fd, int tcp_socket);

void tcp_recv(server_config *cfg, options *options, int epoll_fd, int connFd);

#endif
