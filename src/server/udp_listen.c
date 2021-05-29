#define _GNU_SOURCE

#include <threads.h>
#include <stdio.h>
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

#define UDP_MTU 512
#define UDP_READ_SIZE 4096
#define UDP_MAX_EVENTS 10000

void server_UDP_listen(server_config *cfg, options *options)
{
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
}

void udp_recvfrom(server_config *cfg, options *options, int udp_socket)
{
    char read_buffer[UDP_READ_SIZE + 1];
    struct sockaddr_in client;
    socklen_t c = sizeof(struct sockaddr_in);

    int bytes_read = recvfrom(udp_socket, read_buffer, 2048, 0, &client, &c);
    if (bytes_read > 0)
    {
        read_buffer[bytes_read] = '\0';
        string *req_bits = string_init();
        for (int k = 0; k < bytes_read; ++k)
        {
            string *cur_binary = decimal_to_binary(read_buffer[k]);
            string_pad_zeroes(&cur_binary, 8);
            string_add_str(req_bits, cur_binary->arr);
            string_free(cur_binary);
        }

        // Parse DNS request
        request *req = parse_request(UDP, req_bits);
        response *resp = build_response(cfg, req);
        string *resp_bits = message_to_bits(UDP, resp->msg);

        // Response too big -> switch to TCP (by setting TC bit)
        if (resp_bits->size >= UDP_MTU)
        {
            string_free(resp_bits);
            resp->msg->tc = true;

            string *s = string_init();
            message_headers_to_bits(resp->msg, s);
            resp_bits = binary_bits_to_ascii_string(s);
            sendto(udp_socket, resp_bits->arr, resp_bits->size, 0, &client, c);
        }
        else
        {
            // Regular UDP response sending
            sendto(udp_socket, resp_bits->arr, resp_bits->size, 0, &client, c);
        }

        if (options->verbose)
        {
            print_request(UDP, req);
            print_response(resp);
        }

        // Free memory
        string_free(req_bits);
        string_free(resp_bits);
        request_free(req);
        response_free(resp);
    }
}
