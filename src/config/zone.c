#include <stdlib.h>
#include <string.h>

#include "config/zone.h"

#include "utils/file.h"
#include "utils/string.h"

zone *zone_init()
{
    zone *z = malloc(sizeof(zone));
    // TODO fill zone object
    z->tmp = "test";
    return z;
}

zone *parse_zone(const char *path)
{
    if (!is_file(path))
        return NULL;
    string *zone_contents = read_zone_file(path);
    if (string_is_empty(zone_contents))
    {
        string_free(zone_contents);
        return NULL;
    }
    zone *z = zone_init();
    // TODO Parse zone file
    string_free(zone_contents);
    return z;
}
