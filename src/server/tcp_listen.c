#define _GNU_SOURCE

#include <threads.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>

#include "server/tcp_listen.h"
#include "server/wrapper.h"

#include "utils/error.h"
#include "utils/printer.h"
#include "utils/base_convertions.h"

#include "parser/parse_request.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

#define TCP_THREAD_CAP 12

int server_TCP_listen(void *args)
{
    server_wrapper *w = (server_wrapper *) args;
    server_config *cfg = w->cfg;
    options *options = w->opt;

    int tcp_socket = get_addrinfo_wrapper(cfg, TCP);

    puts("[TCP] Waiting for incoming connections...");

    exit_if_true(listen(tcp_socket, SOMAXCONN) != 0, "Listen error");

    thrd_t tid[TCP_THREAD_CAP];
    int i = 0;
    while (true)
    {
        request_wrapper wrapper = request_wrapper_init(tcp_socket, server_wrapper_init(cfg, options));
        thrd_create(&tid[i++], tcp_receive_request, (void*)&wrapper);

        if (i >= TCP_THREAD_CAP - 1)
        {
            i = 0;
            while (i < TCP_THREAD_CAP - 1)
                thrd_join(tid[i++],NULL);
        }
    }
    return 0;
}

int tcp_receive_request(void *args)
{
    request_wrapper *w = (request_wrapper *)args;
    int tcp_socket = w->socket;
    server_config *cfg = w->s_wrapper.cfg;
    options *options = w->s_wrapper.opt;
    char client_message[4096];
    struct sockaddr_in client;
    socklen_t c = sizeof(struct sockaddr_in);

    int connfd = accept(tcp_socket, &client, &c);

    int sz = recv(connfd, client_message, 4096, 0);
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
    request *req = parse_request(TCP, req_bits);
    response *resp = build_response(cfg, req);
    string *resp_bits = response_to_bits(TCP, resp);

    // Send response
    send(connfd, resp_bits->arr, resp_bits->size, 0);

    close(connfd);

    if (options->verbose)
    {
        print_request(TCP, req);
        print_response(resp);
    }

    // Free memory
    string_free(req_bits);
    string_free(resp_bits);
    request_free(req);
    response_free(resp);

    return 0;
}
