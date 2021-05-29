#include <stdio.h>
#include <stdlib.h>

#include "utils/string.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

#include "connect.h"
#include "build_req.h"

int main(int argc, char *argv[])
{
    (void)argc; (void)argv;

    string *ip = string_init();
    string_add_str(ip, "127.0.0.1");
    int port = 53;

    request *req = build_request();
    string *message = message_to_bits(UDP, req->msg);
    printf("%s\n", message->arr);

    udp_send_request(message, ip, port);
    //tcp_send_request(message, ip, port);

    // Free memory
    string_free(ip);
    request_free(req);
    string_free(message);
}
