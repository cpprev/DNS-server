#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "utils/string.h"
#include "utils/base_convertions.h"

#include "messages/request/request.h"
#include "messages/response/response.h"
#include "messages/response/resp_headers.h"
#include "messages/response/resp_question.h"

#include "parser/parse_request.h"

#include "server/udp_listen.h"

#include "client_options.h"
#include "build_req.h"

client_options *client_options_init(PROTOCOL proto)
{
    (void) proto;
    client_options *opt = malloc(sizeof(client_options));

    opt->ip = string_init();
    string_add_str(opt->ip, "127.0.0.1");
    opt->port = 53;

    opt->message = NULL;
    opt->message_size = 0;

    return opt;
}

void client_options_free(client_options *opt)
{
    if (opt == NULL)
        return;
    if (opt->ip != NULL)
        string_free(opt->ip);
    if (opt->message != NULL)
        free(opt->message);
    free(opt);
}

client_options *parse_client_options(int argc, char *argv[], PROTOCOL proto)
{
    client_options *opt = client_options_init(proto);
    for (int i = 1; i < argc; ++i)
    {
        char *cur = argv[i];
        if (i + 1 < argc && (strcmp(cur, "-s") == 0 || strcmp(cur, "--server") == 0))
        {
            string_flush(opt->ip);
            string_add_str(opt->ip, argv[i + 1]);
        }
        else if (i + 1 < argc && (strcmp(cur, "-p") == 0 || strcmp(cur, "--port") == 0))
        {
            opt->port = atoi(argv[i + 1]);
        }
        else if (strcmp(cur, "-a") == 0 || strcmp(cur, "--alter-headers") == 0)
        {
            //alter_headers = true;
        }
    }
    request *req = build_request();
    void *msg = NULL;
    size_t msg_size = 0;
    message_to_bits(proto, req->msg, &msg, &msg_size);
    opt->message = msg;
    opt->message_size = msg_size;
    request_free(req);
    return opt;
}
