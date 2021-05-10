#ifndef ZONE_ARRAY_H
#define ZONE_ARRAY_H

#include <stdio.h>

#include "config/zone.h"

typedef struct
{
    size_t size;
    zone *zones;
} zone_array;

#endif
