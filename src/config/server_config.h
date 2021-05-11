#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include "utils/string.h"
#include "config/zone_array.h"

typedef struct
{
    string *ip;
    int port;
    zone_array *zones;
} server_config;

server_config *server_config_init(string *ip, int port, zone_array *z_arr);

void server_config_set_attribute(server_config **server_cfg, string *before_semicolon, string *after_semicolon);

void server_config_free(server_config *server_cfg);

#endif
