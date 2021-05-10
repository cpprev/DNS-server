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

#endif
