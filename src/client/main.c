#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "client/connect.h"
#include "client/client_options.h"

int main(int argc, char *argv[])
{
    srand(time(NULL));

    g_options = parse_client_options(argc, argv, UDP);

    // Launch stress test
    //basic_stress_test(UDP);
    //thread_pool_stress_test(UDP);
    udp_send_request();
    //tcp_send_request();

    // Free memory
    client_options_free(g_options);
}
