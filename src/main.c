#define _GNU_SOURCE

#include <stdio.h>
#include <signal.h>
#include <threads.h>

#include "utils/error.h"
#include "utils/string.h"
#include "utils/utils.h"
#include "utils/printer.h"

#include "parser/input.h"
#include "parser/options.h"

#include "config/server_config.h"

#include "server/udp_listen.h"
#include "server/tcp_listen.h"
#include "server/wrapper.h"

int main(int argc, char *argv[])
{
    string *error = string_init();
    options *options = parse_options(argc, argv, error);
    exit_if_true(options == NULL, error->arr);

    server_config *server_cfg = parse_server_config(options->file->arr, error);
    exit_if_true(server_cfg == NULL, error->arr);

    string_free(error);
    if (options->check)
    {
        options_free(options);
        server_config_free(server_cfg);
        printf("Config valid ;)\n");
        return 0;
    }

    if (options->verbose)
        print_server_config(server_cfg);

    // Catch sigints
    struct sigaction si =
    {
        .sa_handler = sigint_handler,
        .sa_flags = SA_NODEFER
    };
    sigaction(SIGINT, &(si), 0);

    // TCP Listen on separate thread
    server_wrapper w = { .opt = options, .cfg = server_cfg };
    thrd_t thread_id;
    thrd_create(&thread_id, server_TCP_listen, (void *)&w);

    // UDP Listen
    server_UDP_listen(server_cfg, options);

    return 0;
}
