#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config/record.h"
#include "config/record_values.h"

#include "utils/file.h"
#include "utils/utils.h"
#include "utils/string.h"

record *record_init()
{
    record *r = malloc(sizeof(record));
    r->type = RECORD_NONE;
    r->class = IN;
    r->domain = NULL;
    r->ttl = -1;
    r->value = NULL;
    r->string_value = NULL;
    return r;
}

record *record_copy(record *r)
{
    record *new_record = record_init();
    new_record->type = r->type;
    new_record->ttl = r->ttl;
    string_copy(&new_record->domain, r->domain);
    new_record->value = malloc((r->value_size + 1) * sizeof(uint8_t));
    uint8_t *new_bits = new_record->value;
    uint8_t *old_bits = r->value;
    for (size_t i = 0; i < r->value_size; ++i)
    {
        new_bits[i] = old_bits[i];
    }
    new_record->value_size = r->value_size;
    string_copy(&new_record->string_value, r->string_value);
    return new_record;
}

void record_free(record *r)
{
    if (r == NULL)
        return;
    if (r->domain != NULL)
        string_free(r->domain);
    if (r->string_value != NULL)
        string_free(r->string_value);
    if (r->value != NULL)
        free(r->value);
    free(r);
}

void get_soa_values(string *s, string **mname, string **rname, string **serial, string **refresh, string **retry, string **expire, string **minimum)
{
    int cur = 1;
    for (size_t i = 0; s->arr[i]; ++i)
    {
        if (s->arr[i] == ' ')
            ++cur;
        else
        {
            if (cur == 1) string_add_char(*mname, s->arr[i]);
            else if (cur == 2) string_add_char(*rname, s->arr[i]);
            else if (cur == 3) string_add_char(*serial, s->arr[i]);
            else if (cur == 4) string_add_char(*refresh, s->arr[i]);
            else if (cur == 5) string_add_char(*retry, s->arr[i]);
            else if (cur == 6) string_add_char(*expire, s->arr[i]);
            else if (cur == 7) string_add_char(*minimum, s->arr[i]);
        }
    }
}

void process_record(record *r, string **tampon, int count_semicolon, string *error)
{
    if (string_is_empty(*tampon))
        return;
    if (count_semicolon == 0)
    {
        string_copy(&r->domain, *tampon);
    }
    else if (count_semicolon == 1)
    {
        if (strcmp((*tampon)->arr, "A") == 0) r->type = A;
        else if (strcmp((*tampon)->arr, "AAAA") == 0) r->type = AAAA;
        else if (strcmp((*tampon)->arr, "CNAME") == 0) r->type = CNAME;
        else if (strcmp((*tampon)->arr, "TXT") == 0) r->type = TXT;
        else if (strcmp((*tampon)->arr, "SOA") == 0) r->type = SOA;
        else if (strcmp((*tampon)->arr, "NS") == 0) r->type = NS;
        else string_add_str(error, "Unknown record type. ");
    }
    else if (count_semicolon == 2)
    {
        r->ttl = atoi((*tampon)->arr);
    }
    else if (count_semicolon == 3)
    {
        if (r->type == AAAA)
            ipv6_extand(tampon);
        size_t b = 0;
        r->value = record_val_to_bits(r->type, *tampon, &b);
        r->value_size = b;

        string_copy(&r->string_value, *tampon);
    }
}

record *parse_record(string *zone_name, string *in, string *error)
{
    if (string_is_empty(in))
        return NULL;
    record *r = record_init();
    string *tampon = string_init();
    int count_semicolon = 0;
    for (size_t i = 0; i < in->size; ++i)
    {
        char c = in->arr[i];
        if ((c == ';' && i > 0 && in->arr[i - 1] != '\\') || i == in->size - 1)
        {
            if (i == in->size - 1)
                string_add_char(tampon, c);
            process_record(r, &tampon, count_semicolon, error);
            count_semicolon += 1;
            string_flush(tampon);
        }
        else
        {
            string_add_char(tampon, c);
        }
    }
    string_free(tampon);
    if (string_is_empty(r->domain) || r->ttl == -1 || r->type == RECORD_NONE)
    {
        record_free(r);
        string *custom_error = string_init();
        string_add_str(custom_error, "Invalid record found in zone \'");
        string_add_str(custom_error, zone_name->arr);
        string_add_str(custom_error, "\'. Records have to be in such format : example.com;A;86400;192.168.2.2");
        string_add_str(error, custom_error->arr);
        string_free(custom_error);
        return NULL;
    }
    return r;
}
