#ifndef ZONE_H
#define ZONE_H

#include "utils/string.h"

typedef struct
{
    string *name;
    string *path;
} zone;

zone *zone_init();

void zone_free(zone *z);

zone *parse_zone(const char *path);

#endif
