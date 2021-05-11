#include "utils/json.h"
#include "utils/file.h"
#include "utils/string.h"

#include "config/zone.h"

string *copy_to_next_brace(string *in, size_t *startIndex, char brace)
{
    string *result = string_init();
    string_add_char(result, brace);

    *startIndex += 1;

    char openingRef = brace, closingRef = brace == '{' ? '}' : ']';
    int countOpening = 1, countClosing = 0;

    for (; *startIndex < in->size - 1; (*startIndex)++)
    {
        char c = in->arr[*startIndex];
        if (c == openingRef)
            ++countOpening;
        else if (c == closingRef)
            ++countClosing;
        string_add_char(result, c);
        if (countClosing == countOpening)
            break;
    }
    return result;
}

server_config *parse_server_config(const char* path)
{
    string *s = read_json_file(path);
    server_config *server_cfg = NULL;

    string *before_semicolon = string_init();
    string *after_semicolon = string_init();
    bool hit_semicolon = false;
    for (size_t i = 1; i < s->size - 1; ++i)
    {
        char c = s->arr[i];
        if (c == '[')
        {
            string_free(after_semicolon);
            after_semicolon = copy_to_next_brace(s, &i, '[');
            string_print(after_semicolon);
            if (i == s->size - 2)
                i--;
            continue;
        }
        else if (c == ',' || i == s->size - 2)
        {
            hit_semicolon = false;
            string_print(before_semicolon);
            server_config_set_attribute(&server_cfg, before_semicolon, after_semicolon);

            string_flush(before_semicolon);
            string_flush(after_semicolon);
        }
        else if (c == ':')
        {
            hit_semicolon = true;
        }

        if (c != ':' && c != ',' && c != '"')
        {
            if (!hit_semicolon)
                string_add_char(before_semicolon, c);
            else
                string_add_char(after_semicolon, c);
        }
    }

    puts(server_cfg->ip->arr);
    printf("%d\n", server_cfg->port);
    if (server_cfg->zones != NULL)
    {
        for (size_t z = 0; z < server_cfg->zones->size; ++z)
        {
            puts(server_cfg->zones->arr[z]->tmp);
        }
    }
    string_free(before_semicolon);
    string_free(after_semicolon);
    string_free(s);

    if (server_cfg->ip == NULL || server_cfg->port == -1 || server_cfg->zones == NULL)
    {
        server_config_free(server_cfg);
        server_cfg = NULL;
    }
    return server_cfg;
}

zone_array *parse_zones(string *s)
{
    zone_array *z_arr = zone_array_init();
    string *array_elem = string_init();
    for (size_t i = 1; i < s->size - 1 ; ++i)
    {
        char c = s->arr[i];
        if (c == ',' || i == s->size - 2)
        {
            printf("%s\n", array_elem->arr);
            zone *z = parse_zone(array_elem->arr);
            if (z == NULL)
            {
                zone_array_free(z_arr);
                z_arr = NULL;
                break;
            }
            zone_array_add_zone(z_arr, z);
            string_flush(array_elem);
        }

        if (c != ',' && c != '"')
        {
            string_add_char(array_elem, c);
        }
    }
    string_free(array_elem);
    return z_arr;
}
