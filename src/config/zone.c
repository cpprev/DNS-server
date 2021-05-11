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

    z->records = record_array_init();
    string *tampon = string_init();
    for (size_t i = 0; i < zone_contents->size; ++i)
    {
        char c = zone_contents->arr[i];
        if (c == '\n' || i == zone_contents->size - 1)
        {
            if (c != '\n' && i == zone_contents->size - 1)
                string_add_char(tampon, c);

            record *r = parse_record(tampon);
            if (r != NULL)
                record_array_add_record(z->records, r);
            string_flush(tampon);
        }
        else
            string_add_char(tampon, c);
    }
    string_free(tampon);
    string_free(zone_contents);
}
