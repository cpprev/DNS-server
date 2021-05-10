#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils/string.h"

#define ADD_CAP 16

string string_init (void)
{
    string s;
    s.capacity = ADD_CAP;
    s.size = 0;
    s.arr = malloc((s.capacity + 1) * sizeof(char));
    strcpy(s.arr, "\0");
    return s;
}

string string_resize (string s)
{
    s.capacity += ADD_CAP;
    s.arr = realloc(s.arr, (s.capacity + 1) * sizeof(char));
    return s;
}

string string_add_char (string s, char c)
{
    if (s.size >= s.capacity)
    {
        s = string_resize(s);
    }
    s.arr[s.size] = c;
    s.size++;
    s.arr[s.size] = '\0';
    return s;
}

string string_add_str (string s, char *s2)
{
    for (size_t i = 0; s2[i]; ++i)
    {
        s = string_add_char(s, s2[i]);
    }
    return s;
}
