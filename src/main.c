#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "utils/error.h"
#include "utils/string.h"
#include "utils/file.h"

#include "parsing/input.h"

#include "config/server_config.h"
#include "config/zone.h"
#include "config/record_type.h"

#include "server/listen.h"

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nExited\n");
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    exit_if_true(argc != 2, "[Usage] ./dns [Input file]");
    char *input_path = argv[1];
    exit_if_true(!is_file(input_path), "[Runtime error] Input file cannot be found.");

    server_config *server_cfg = parse_server_config(input_path);
    exit_if_true(server_cfg == NULL, "[Runtime error] Input file is not in valid format.");

    // TODO Delete
    string_print(server_cfg->ip);
    printf("%d\n", server_cfg->port);
    if (server_cfg->zones != NULL)
    {
        for (int i = 0; server_cfg->zones->arr[i]; ++i)
        {
            for(int j = 0; server_cfg->zones->arr[i]->records->arr[j]; ++j)
            {
                record *r = server_cfg->zones->arr[i]->records->arr[j];
                printf("%s\t", r->domain->arr);
                print_record_type(r->type);
                printf("\t%d\t%s\n", r->ttl, r->value->arr);
            }
            puts("-------------------------------------");
        }
    }

    // Catch sigints
    struct sigaction si =
    {
        .sa_handler = sigint_handler,
        .sa_flags = SA_NODEFER
    };
    sigaction(SIGINT, &(si), 0);

    // Launch server
    server_listen(server_cfg);

    // Free memory
    server_config_free(server_cfg);

    return 0;
}
