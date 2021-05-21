#include <string.h>

#include "parser/parse_zone.h"

#include "utils/file.h"

void parse_zone(zone *z, string *error)
{
    if (!is_file(z->path->arr))
    {
        string *custom_error = string_init();
        string_add_str(custom_error, "Zone file at this path could not be found : ");
        string_add_str(custom_error, z->path->arr);
        string_add_str(error, custom_error->arr);
        string_free(custom_error);
        return;
    }
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

            record *r = parse_record(z->name, tampon, error);
            if (r != NULL)
                record_array_add_record(z->records, r);
            else if (!string_is_empty(error))
                break;
            string_flush(tampon);
        }
        else
            string_add_char(tampon, c);
    }
    string_free(tampon);
    string_free(zone_contents);
}
