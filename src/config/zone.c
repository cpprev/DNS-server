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
    z->records = NULL;
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
    if (z->records != NULL)
        record_array_free(z->records);
    free(z);
}
