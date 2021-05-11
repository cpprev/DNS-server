#ifndef ZONE_H
#define ZONE_H

#include "config/record_array.h"

#include "utils/string.h"

typedef struct
{
    string *name;
    string *path;
    record_array *records;
} zone;

zone *zone_init();

void zone_free(zone *z);

void parse_zone(zone *zone);

#endif
