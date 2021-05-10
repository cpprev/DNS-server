#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include "utils/string.h"
#include "config/zone_array.h"

typedef struct
{
    string ip;
    int port;
    zone_array zones;
} server_config;

#endif
