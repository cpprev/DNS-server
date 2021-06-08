#define _GNU_SOURCE

#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netdb.h>

#include "server/udp_listen.h"
#include "server/wrapper.h"

#include "utils/error.h"
#include "utils/printer.h"
#include "utils/base_convertions.h"

#include "messages/response/resp_headers.h"

#include "parser/parse_request.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

void *server_UDP_listen(void *args)
{
    server_wrapper *w = (server_wrapper *) args;
    server_config *cfg = w->cfg;
    options *options = w->opt;

    int udp_socket = get_addrinfo_wrapper(cfg, UDP);
    set_socket_non_blocking(udp_socket);

    puts("[UDP] Waiting for incoming connections...");

    int running = 1, event_count, i;
    struct epoll_event event, events[UDP_MAX_EVENTS];
    int epoll_fd = epoll_create(UDP_MAX_EVENTS);
    exit_if_true(epoll_fd == -1, "Failed to create epoll FD.");

    event.events = EPOLLIN;
    event.data.fd = udp_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, udp_socket, &event) < 0)
    {
        close(epoll_fd);
        exit_if_true(true, "Failed to add FD to epoll.");
    }

    while (running)
    {
        event_count = epoll_wait(epoll_fd, events, UDP_MAX_EVENTS, 1000);
        exit_if_true(event_count < 0, "udp epoll_wait error");
        if (event_count == 0) continue;

        for (i = 0; i < event_count; ++i)
        {
            if (events[i].data.fd == udp_socket)
            {
                udp_recvfrom(cfg, options, udp_socket);
            }
        }
    }

    exit_if_true(close(epoll_fd), "Failed to close epoll FD");
    return 0;
}

void udp_recvfrom(server_config *cfg, options *options, int udp_socket)
{
    char read_buffer[UDP_READ_SIZE + 1];
    struct sockaddr_in client;
    socklen_t c = sizeof(struct sockaddr_in);

    int bytes_read = recvfrom(udp_socket, read_buffer, UDP_READ_SIZE, 0, &client, &c);
    if (bytes_read > 0)
    {
        read_buffer[bytes_read] = '\0';

        // Parse DNS request
        request *req = parse_request(UDP, (void *) read_buffer);
        response *resp = build_response(cfg, req);
        void *bits = NULL;
        size_t b = 0;
        message_to_bits(UDP, resp->msg, &bits, &b);

        // Response too big -> switch to TCP (by setting TC bit)
        if (b >= UDP_MTU)
        {
            free(bits);
            resp->msg->tc = true;

            bits = malloc(32);
            b = 0;
            message_headers_to_bits(resp->msg, bits, &b);
            sendto(udp_socket, bits, b, 0, &client, c);
        }
        else
        {
            // Regular UDP response sending
            //sendto(udp_socket, resp_bits->arr, resp_bits->size, 0, &client, c);
            sendto(udp_socket, bits, b, 0, &client, c);
        }

        if (options->verbose)
        {
            print_request(UDP, req);
            print_response(resp);
        }

        // Free memory
        request_free(req);
        response_free(resp);
        free(bits);
    }
}
