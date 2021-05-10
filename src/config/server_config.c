#include <stdlib.h>

#include "utils/string.h"

#include "config/server_config.h"
#include "config/zone_array.h"

server_config *server_config_init(string *ip, int port, zone_array *z_arr)
{
    server_config *serv_cfg = malloc(sizeof(server_config));
    serv_cfg->ip = ip;
    serv_cfg->port = port;
    serv_cfg->zones = z_arr;
    return serv_cfg;
}

void server_config_free(server_config *server_cfg)
{
    string_free(server_cfg->ip);
    zone_array_free(server_cfg->zones);
    free(server_cfg);
}
