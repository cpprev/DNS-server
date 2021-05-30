#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/string.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

#include "build_req.h"
#include "stress_test.h"
#include "connect.h"

int main()
{
    request *req = build_request();
    g_message = message_to_bits(UDP, req->msg);

    g_ip = string_init();
    string_add_str(g_ip, "127.0.0.1");

    // Launch stress test
    //basic_stress_test(UDP);
    //thread_pool_stress_test(UDP);
    udp_send_request();

    // Free memory
    request_free(req);
    string_free(g_ip);
    string_free(g_message);
}
