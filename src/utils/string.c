#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/string.h"

#define STR_CAP_ADD 16

string *string_init (void)
{
    string *s = malloc(sizeof(string));
    s->capacity = STR_CAP_ADD;
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

void string_flush (string *s)
{
    free(s->arr);
    s->size = 0;
    s->capacity = STR_CAP_ADD;
    s->arr = malloc((s->capacity + 1) * sizeof(char));
    strcpy(s->arr, "\0");
}

void string_resize (string *s)
{
    s->capacity += STR_CAP_ADD;
    s->arr = realloc(s->arr, (s->capacity + 1) * sizeof(char));
}

void string_add_char (string *s, char c)
{
    if (s->size + 1 >= s->capacity)
        string_resize(s);
    s->arr[s->size] = c;
    s->size++;
    s->arr[s->size] = '\0';
}

void string_add_str (string *s, char *s2)
{
    for (size_t i = 0; s2[i]; ++i)
        string_add_char(s, s2[i]);
}

bool string_is_empty(string *s)
{
    return s == NULL || s->size == 0;
}

void string_print(string *s)
{
    for (size_t i = 0; i < s->size; ++i)
        printf("%c", s->arr[i]);
    printf("\n");
}

void string_copy(string **dst, string *src)
{
    if (*dst == NULL)
        *dst = string_init();
    string_flush(*dst);
    string_add_str(*dst, src->arr);
}

bool string_equals(string *s1, string *s2)
{
    return strcmp(s1->arr, s2->arr) == 0;
}

void string_pad_zeroes(string **s, int n)
{
    size_t needed = n - (*s)->size;
    string *temp = string_init();
    for (size_t i = 0; i < needed; ++i)
        string_add_char(temp, '0');
    string_add_str(temp, (*s)->arr);
    string_free(*s);
    *s = temp;
}
