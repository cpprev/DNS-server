#define _GNU_SOURCE

#include <threads.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
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

#define UDP_THREAD_CAP 48
#define UDP_MTU 512

void server_UDP_listen(server_config *cfg, options *options)
{
    int udp_socket = get_addrinfo_wrapper(cfg, UDP);
    //set_socket_non_blocking(udp_socket);

    puts("[UDP] Waiting for incoming connections...");

    thrd_t tid[UDP_THREAD_CAP];
    int i = 0;

    while (true)
    {
        request_wrapper wrapper = request_wrapper_init(udp_socket, server_wrapper_init(cfg, options));
        thrd_create(&tid[i++], udp_receive_request, (void*)&wrapper);

        if (i >= UDP_THREAD_CAP - 1)
        {
            i = 0;
            while (i < UDP_THREAD_CAP - 1)
                thrd_join(tid[i++],NULL);
        }
    }
}

int udp_receive_request(void *args)
{
    request_wrapper *w = (request_wrapper *)args;
    int udp_socket = w->socket;
    server_config *cfg = w->s_wrapper.cfg;
    options *options = w->s_wrapper.opt;
    char client_message[2048];
    struct sockaddr_in client;
    socklen_t c = sizeof(struct sockaddr_in);

    int sz = recvfrom(udp_socket, client_message, 2048, 0, &client, &c);
    client_message[sz] = '\0';
    string *req_bits = string_init();
    for (int i = 0; i < sz; ++i)
    {
        string *cur_binary = decimal_to_binary(client_message[i]);
        string_pad_zeroes(&cur_binary, 8);
        string_add_str(req_bits, cur_binary->arr);
        string_free(cur_binary);
    }

    // Parse DNS request
    request *req = parse_request(UDP, req_bits);
    response *resp = build_response(cfg, req);
    string *resp_bits = response_to_bits(UDP, resp);

    // Response too big -> switch to TCP (by setting TC bit)
    if (resp_bits->size >= UDP_MTU)
    {
        string_free(resp_bits);
        resp->msg->tc = true;

        string *s = string_init();
        response_headers_to_bits(resp, s);
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

    return 0;
}
