#include <stdlib.h>
#include <string.h>

#include "config/zone.h"

#include "utils/file.h"
#include "utils/string.h"

zone *zone_init()
{
    zone *z = malloc(sizeof(zone));
    // TODO fill zone object
    z->name = NULL;
    z->path = NULL;

    return z;
}

void zone_free(zone *z)
{
    if (z == NULL)
        return;
    if (z->name != NULL)
        string_free(z->name);
    if (z->path != NULL)
        string_free(z->path);
    free(z);
}

/*zone *parse_zone(const char *path)
{
    if (!is_file(path))
        return NULL;
    string *zone_contents = read_file(path);
    if (string_is_empty(zone_contents))
    {
        string_free(zone_contents);
        return NULL;
    }
    zone *z = zone_init();
    // TODO Parse zone file
    string_free(zone_contents);
    return z;
}*/
