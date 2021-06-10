#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "connect.h"
#include "client_options.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    g_options = parse_client_options(argc, argv, TCP);

    // Launch stress test
    //basic_stress_test(UDP);
    //thread_pool_stress_test(UDP);
    //udp_send_request();
    tcp_send_request();

    // Free memory
    client_options_free(g_options);
}
