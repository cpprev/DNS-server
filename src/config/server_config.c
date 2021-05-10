#include <stdlib.h>

#include "server_config.h"

server_config *server_config_init(string *ip, int port, zone_array *z_arr)
{
    server_config *serv_cfg = malloc(sizeof(server_config));
    serv_cfg->ip = ip;
    serv_cfg->port = port;
    serv_cfg->zones = z_arr;
    return serv_cfg;
}
