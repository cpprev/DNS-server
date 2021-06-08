#ifndef TCP_LISTEN_H
#define TCP_LISTEN_H

#include "config/server_config.h"

#include "parser/options.h"

void *server_TCP_listen(void *args);

void tcp_accept(int epoll_fd, int tcp_socket);

void tcp_recv(server_config *cfg, options *options, int epoll_fd, int connFd);

#endif
