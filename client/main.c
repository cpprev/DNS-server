#include <stdio.h>
#include <stdlib.h>

#include "utils/string.h"

#include "connect.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    string *ip = string_init();
    string_add_str(ip, "127.0.0.1");
    int port = 53;

    tcp_send_request(ip, port);

    // Free memory
    string_free(ip);
}
