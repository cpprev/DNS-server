#ifndef JSON_H
#define JSON_H

#include <stdbool.h>

#include "config/server_config.h"
#include "config/zone_array.h"

server_config *parse_server_config(const char* path);

zone_array *parse_zones(string *s);

#endif
