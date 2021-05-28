#include <stdio.h>
#include <stdlib.h>

#include "utils/string.h"
#include "utils/error.h"

#include "config/server_config.h"

#include "parser/input.h"

#include "connect.h"

int main(int argc, char *argv[])
{
    exit_if_true(argc != 2, "[Usage] ./client [input-file]");
    string *error = string_init();
    server_config *server_cfg = parse_server_config(argv[1], error);

    tcp_send_request(server_cfg);

    // Free memory
    server_config_free(server_cfg);
    string_free(error);
}
