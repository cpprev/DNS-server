#include <stdlib.h>
#include <string.h>
#include <fnmatch.h>

#include "utils/string.h"

#include "config/server_config.h"
#include "config/zone_array.h"

server_config *server_config_init_empty()
{
    server_config *serv_cfg = malloc(sizeof(server_config));
    serv_cfg->ip = NULL;
    serv_cfg->zones = NULL;
    return serv_cfg;
}

server_config *server_config_init(string *ip, int port, zone_array *z_arr)
{
    server_config *serv_cfg = malloc(sizeof(server_config));
    serv_cfg->ip = ip;
    serv_cfg->port = port;
    serv_cfg->zones = z_arr;
    return serv_cfg;
}

string *get_config_value(string *line)
{
    string *s = string_init();
    bool hit_equals = false, hit_whitespace = false;
    for (size_t i = 0; i < line->size; ++i)
    {
        char c = line->arr[i];
        if (c == '=')
            hit_equals = true;
        else if (hit_equals)
        {
            if ((c == ' ' || c == '\t') && !hit_whitespace)
                hit_whitespace = true;
            else
                string_add_char(s, c);
        }
    }
    return s;
}

void server_config_set_attribute(server_config **server_cfg, string *line, INPUT_STATE st, size_t zone_ind)
{
    if (*server_cfg == NULL)
        *server_cfg = server_config_init_empty();

    if (st == SERVER)
    {
        if (fnmatch("*ip*=*", line->arr, 0) == 0)
        {
            string *ip = get_config_value(line);
            (*server_cfg)->ip = string_copy(ip);
            string_free(ip);
        }
        else if (fnmatch("*port*=*", line->arr, 0) == 0)
        {
            string *port_str = get_config_value(line);
            int port = atoi(port_str->arr);
            (*server_cfg)->port = port;
            string_free(port_str);
        }
    }
    else if (st == ZONE)
    {
        if ((*server_cfg)->zones == NULL)
            (*server_cfg)->zones = zone_array_init();
        if (zone_ind >= (*server_cfg)->zones->size)
        {
            zone *z = zone_init();
            zone_array_add_zone((*server_cfg)->zones, z);
        }
        if (fnmatch("*name*=*", line->arr, 0) == 0)
        {
            string *name = get_config_value(line);
            (*server_cfg)->zones->arr[zone_ind]->name = string_copy(name);
            string_free(name);
        }
        else if (fnmatch("*path*=*", line->arr, 0) == 0)
        {
            string *path = get_config_value(line);
            (*server_cfg)->zones->arr[zone_ind]->path = string_copy(path);
            string_free(path);
        }
    }
}

void server_config_free(server_config *server_cfg)
{
    string_free(server_cfg->ip);
    zone_array_free(server_cfg->zones);
    free(server_cfg);
}
