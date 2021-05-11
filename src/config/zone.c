#include <stdlib.h>
#include <string.h>

#include "config/zone.h"

#include "utils/file.h"
#include "utils/string.h"

zone *zone_init()
{
    zone *z = malloc(sizeof(zone));

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

void parse_zone(zone *z)
{
    if (!is_file(z->path->arr))
        return;
    string *zone_contents = read_file(z->path->arr);
    if (string_is_empty(zone_contents))
    {
        string_free(zone_contents);
        return;
    }
    // TODO Parse zone file
    string_free(zone_contents);
    string_add_str(z->name, "TEST");
}
