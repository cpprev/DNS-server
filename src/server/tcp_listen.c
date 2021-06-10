#define _GNU_SOURCE

#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <errno.h>

#include "server/tcp_listen.h"
#include "server/wrapper.h"

#include "utils/error.h"
#include "utils/printer.h"
#include "utils/bits.h"
#include "utils/base_convertions.h"

#include "parser/parse_request.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

void *server_TCP_listen(void *args)
{
    server_wrapper *w = (server_wrapper *) args;
    server_config *cfg = w->cfg;
    options *options = w->opt;

    int tcp_socket = get_addrinfo_wrapper(cfg, TCP);
    set_socket_non_blocking(tcp_socket);
    exit_if_true(listen(tcp_socket, SOMAXCONN) != 0, "Listen error");

    puts("[TCP] Waiting for incoming connections...");

    int running = 1, event_count, i;
    struct epoll_event event, events[TCP_MAX_EVENTS];
    int epoll_fd = epoll_create(TCP_MAX_EVENTS);
    exit_if_true(epoll_fd == -1, "Failed to create epoll FD.");

    event.events = EPOLLIN;
    event.data.fd = tcp_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, tcp_socket, &event) < 0)
    {
        close(epoll_fd);
        exit_if_true(true, "Failed to add FD to epoll.");
    }

    while (running)
    {
        event_count = epoll_wait(epoll_fd, events, TCP_MAX_EVENTS, 1000);
        exit_if_true(event_count < 0, "tcp epoll_wait error");
        if (event_count == 0) continue;

        for (i = 0; i < event_count; ++i)
        {
            if (events[i].data.fd == tcp_socket)
            {
                tcp_accept(epoll_fd, tcp_socket);
            }
            else
            {
                tcp_recv(cfg, options, epoll_fd, events[i].data.fd);
            }
        }
    }

    exit_if_true(close(epoll_fd), "Failed to close epoll FD");
    return 0;
}

void tcp_accept(int epoll_fd, int tcp_socket)
{
    int cliFd;
    struct sockaddr_in cliaddr;
    socklen_t socklen = sizeof(struct sockaddr_in);
    struct epoll_event ev;

    cliFd = accept(tcp_socket, (struct sockaddr*)&cliaddr, &socklen);
    exit_if_true(cliFd < 0, "accept error");

    set_socket_non_blocking(cliFd);

    ev.events = EPOLLIN;
    ev.data.fd = cliFd;

    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cliFd, &ev);
}

void tcp_recv(server_config *cfg, options *options, int epoll_fd, int connFd)
{
    char read_buffer[TCP_READ_SIZE + 1];
    int bytes_read = 0;
    bits *in = NULL;
    while ((bytes_read = recv(connFd, read_buffer, TCP_READ_SIZE, 0)) > 0)
        bits_add_bits(&in, (uint8_t *) read_buffer, bytes_read);
    if (in->size > 0)
    {
        // Parse DNS request
        request *req = parse_request(TCP, in->arr);
        bits_free(in);
        response *resp = build_response(cfg, req);
        void *bits = NULL;
        size_t b = 0;
        message_to_bits(TCP, resp->msg, &bits, &b);

        // Send response
        send(connFd, bits, b, 0);

        close(connFd);

        if (options->verbose)
        {
            print_request(TCP, req);
            print_response(resp);
        }

        // Free memory
        request_free(req);
        response_free(resp);

        struct epoll_event ev;
        ev.data.fd = connFd;
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, connFd, &ev);
        close(connFd);
    }
}
