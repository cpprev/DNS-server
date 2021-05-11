#ifndef ZONE_H
#define ZONE_H

typedef struct
{
    char *tmp;
} zone;

zone *zone_init();

zone *parse_zone(const char *path);

#endif
