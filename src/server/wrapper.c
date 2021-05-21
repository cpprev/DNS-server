#define _GNU_SOURCE

#include <sys/types.h>
#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "server/wrapper.h"
#include "server/protocol.h"

#include "utils/error.h"

server_wrapper server_wrapper_init(server_config *server_cfg, options *options)
{
    server_wrapper w;
    w.cfg = server_cfg;
    w.opt = options;
    return w;
}

request_wrapper request_wrapper_init(int socket,server_wrapper s_wrapper)
{
    request_wrapper r;
    r.socket = socket;
    r.s_wrapper = s_wrapper;
    return r;
}

int get_addrinfo_wrapper(server_config *cfg, PROTOCOL proto)
{
    struct addrinfo *res = NULL, *rp = NULL;

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    if (proto == UDP)
        hints.ai_socktype = SOCK_DGRAM;
    else if (proto == TCP)
        hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    char snum[10];
    sprintf(snum, "%d", cfg->port);
    int s = getaddrinfo(cfg->ip->arr, snum, &hints, &res);
    exit_if_true(s != 0, "getaddrinfo error");

    int sockfd = -1;
    for (rp = res; rp != NULL; rp = rp->ai_next)
    {
        sockfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        int optval = 1;
        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
        if (sockfd == -1)
            continue;
        if (bind(sockfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;
        close(sockfd);
    }
    freeaddrinfo(res);
    exit_if_true(rp == NULL, "No sockets found");

    return sockfd;
}
