#define _XOPEN_SOURCE 600

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "server/listen.h"

#include "utils/error.h"

#include "parsing/request.h"

void server_listen(server_config *cfg)
{
    struct addrinfo *res = NULL, *rp = NULL;

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;
    //hints.ai_socktype = SOCK_STREAM;
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

    puts("Waiting for incoming connections...");

    struct sockaddr_in client;
    int c = sizeof(struct sockaddr_in);
    char client_message[2000];

    while (true)
    {
        int sz = recvfrom(sockfd, client_message, 2000, 0, (struct sockaddr *)&client, (socklen_t*)&c);
        client_message[sz] = '\0';
        int bs[1000];
        int c = 0;
        for (int i = 0; i < sz; ++i)
        {
            for (int j = 7; j >= 0; --j)
            {
                int byte = (client_message[i] >> j) & 1;
                printf("%u", byte);
                bs[c++] = byte;
                bs[c] = '\0';
            }
        }
        puts("");

        // Parse DNS request
        parse_request(bs, c);

        sendto(sockfd, client_message, strlen(client_message), 0, (struct sockaddr *)&client, (socklen_t)c);
    }
}
