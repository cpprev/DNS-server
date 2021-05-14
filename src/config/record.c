#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "config/record.h"

#include "utils/file.h"
#include "utils/string.h"

record *record_init()
{
    record *r = malloc(sizeof(record));
    r->type = RECORD_NONE;
    r->domain = NULL;
    r->ttl = -1;
    r->value = NULL;
    return r;
}

void record_free(record *r)
{
    if (r == NULL)
        return;
    if (r->domain != NULL)
        string_free(r->domain);
    if (r->value != NULL)
        string_free(r->value);
    free(r);
}

void process_record(record *r, string *tampon, int count_semicolon)
{
    if (string_is_empty(tampon))
        return;
    if (count_semicolon == 0)
    {
        string_copy(&r->domain, tampon);
    }
    else if (count_semicolon == 1)
    {
        if (strcmp(tampon->arr, "A") == 0) r->type = A;
        else if (strcmp(tampon->arr, "AAAA") == 0) r->type = AAAA;
        else if (strcmp(tampon->arr, "CNAME") == 0) r->type = CNAME;
        else if (strcmp(tampon->arr, "TXT") == 0) r->type = TXT;
        else if (strcmp(tampon->arr, "SOA") == 0) r->type = SOA;
        else if (strcmp(tampon->arr, "NS") == 0) r->type = NS;
    }
    else if (count_semicolon == 2)
    {
        r->ttl = atoi(tampon->arr);
    }
    else if (count_semicolon == 3)
    {
        string_copy(&r->value, tampon);
    }
}

record *parse_record(string *in)
{
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
            process_record(r, tampon, count_semicolon);
            count_semicolon += 1;
            string_flush(tampon);
        }
        else
        {
            string_add_char(tampon, c);
        }
    }
    string_free(tampon);
    if (string_is_empty(r->domain) || string_is_empty(r->value) || r->ttl == -1 || r->type == RECORD_NONE)
    {
        record_free(r);
        return NULL;
    }
    return r;
}
