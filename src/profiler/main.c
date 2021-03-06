#include <stdlib.h>
#include <string.h>

#include "utils/string.h"

#include "config/server_config.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

#include "parser/parse_request.h"
#include "parser/input.h"

#include "server/udp_listen.h"

#include "client/client_options.h"
#include "client/build_req.h"

#define OCCURENCES 128000

void code_to_profile(server_config *cfg, void *read_buffer, size_t size)
{
    for (int i = 0; i < OCCURENCES; ++i)
    {
        request *req = parse_request(UDP, (void *) read_buffer, size);
        response *resp = build_response(cfg, req);
        void *bits = NULL;
        size_t b = 0;
        message_to_bits(UDP, resp->msg, &bits, &b);

        // Free memory
        request_free(req);
        response_free(resp);
        free(bits);
    }
}

int main(int argc, char *argv[])
{
    // Input parsing (copied from src/main.c)
    string *error = string_init();
    options *options = parse_options(argc, argv, error);
    server_config *cfg = parse_server_config(options->file->arr, error);
    string_free(error);

    // Build dummy req (copied from client/client_options.c)
    request *dummy_req = build_request();
    void *read_buffer = NULL;
    size_t msg_size = 0;
    message_to_bits(UDP, dummy_req->msg, &read_buffer, &msg_size);

    // UDP Listen code that we want to profile (copied from src/server/udp_listen.c)
    code_to_profile(cfg, read_buffer, msg_size);

    free(read_buffer);
    request_free(dummy_req);
    options_free(options);
    server_config_free(cfg);

    return 0;
}
