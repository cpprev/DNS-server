#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/string.h"

#define ADD_CAP 16

string *string_init (void)
{
    string *s = malloc(sizeof(string));
    s->capacity = ADD_CAP;
    s->size = 0;
    s->arr = malloc((s->capacity + 1) * sizeof(char));
    strcpy(s->arr, "\0");
    return s;
}

void string_free (string *s)
{
    if (s->arr != NULL)
        free(s->arr);
    free(s);
}

void string_resize (string *s)
{
    s->capacity += ADD_CAP;
    s->arr = realloc(s->arr, (s->capacity + 1) * sizeof(char));
}

void string_add_char (string *s, char c)
{
    if (s->size >= s->capacity)
    {
        string_resize(s);
    }
    s->arr[s->size] = c;
    s->size++;
    s->arr[s->size] = '\0';
}

void string_add_str (string *s, char *s2)
{
    for (size_t i = 0; s2[i]; ++i)
    {
        string_add_char(s, s2[i]);
    }
}
