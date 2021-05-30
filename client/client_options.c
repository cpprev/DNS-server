#include <stdlib.h>
#include <string.h>

#include "utils/string.h"

#include "messages/request/request.h"
#include "messages/response/response.h"

#include "client_options.h"
#include "build_req.h"

client_options *client_options_init(PROTOCOL proto)
{
    client_options *opt = malloc(sizeof(client_options));

    opt->ip = string_init();
    string_add_str(opt->ip, "127.0.0.1");
    opt->port = 53;

    request *req = build_request();
    opt->message = message_to_bits(proto, req->msg);
    request_free(req);

    return opt;
}

void client_options_free(client_options *opt)
{
    if (opt == NULL)
        return;
    if (opt->ip != NULL)
        string_free(opt->ip);
    if (opt->message != NULL)
        string_free(opt->message);
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
        else if (i + 1 < argc && strcmp(cur, "--qr") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--opcode") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--aa") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--tc") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--rd") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--ra") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--qdcount") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--ancount") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--nscount") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--arcount") == 0)
        {
            // TODO
        }
        else if (i + 1 < argc && strcmp(cur, "--question") == 0)
        {
            // TODO
        }
    }
    return opt;
}
