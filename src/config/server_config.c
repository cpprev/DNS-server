#include <stdlib.h>
#include <string.h>

#include "utils/string.h"
#include "utils/json.h"

#include "config/server_config.h"
#include "config/zone_array.h"

server_config *server_config_init_empty()
{
    server_config *serv_cfg = malloc(sizeof(server_config));
    serv_cfg->ip = NULL;
    serv_cfg->zones = NULL;
    return serv_cfg;
}

server_config *server_config_init(string *ip, int port, zone_array *z_arr)
{
    server_config *serv_cfg = malloc(sizeof(server_config));
    serv_cfg->ip = ip;
    serv_cfg->port = port;
    serv_cfg->zones = z_arr;
    return serv_cfg;
}

void server_config_set_attribute(server_config **server_cfg, string *before_semicolon, string *after_semicolon)
{
    if (*server_cfg == NULL)
        *server_cfg = server_config_init_empty();
    if (strcmp(before_semicolon->arr, "ip") == 0)
    {
        (*server_cfg)->ip = string_copy(after_semicolon);
    }
    else if (strcmp(before_semicolon->arr, "port") == 0)
    {
        (*server_cfg)->port = atoi(after_semicolon->arr);
    }
    else if (strcmp(before_semicolon->arr, "zones") == 0)
    {
        (*server_cfg)->zones = parse_zones(after_semicolon);
    }
}

void server_config_free(server_config *server_cfg)
{
    string_free(server_cfg->ip);
    zone_array_free(server_cfg->zones);
    free(server_cfg);
}
