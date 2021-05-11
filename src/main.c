#include <stdio.h>
#include <stdlib.h>

#include "utils/error.h"
#include "utils/string.h"
#include "utils/file.h"
#include "utils/json.h"

#include "config/server_config.h"
#include "config/zone_array.h"
#include "config/zone.h"

int main(int argc, char *argv[])
{
    exit_if_true(argc != 2, "[Usage] ./dns [Input file]");
    char *input_path = argv[1];
    exit_if_true(!is_file(input_path), "[Runtime error] Input file cannot be found.");

    // TODO Check if JSON valid before parsing (to make it clearer)

    server_config *server_cfg = parse_server_config(input_path);
    exit_if_true(server_cfg == NULL, "[Runtime error] Input file is not in valid JSON format.");

    // Free memory
    server_config_free(server_cfg);

    return 0;
}
