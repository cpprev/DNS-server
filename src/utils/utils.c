#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/utils.h"

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nExited\n");
        exit(0);
    }
}

void ipv6_extand(string **ip)
{
    if ((*ip)->size == 0)
        return;
    string *extanded = string_init();
    string_add_char(extanded, (*ip)->arr[0]);
    int count_before = 0, count_after = 0, ind = 0;
    for (size_t i = 1; i < (*ip)->size && !((*ip)->arr[i - 1] == ':' && (*ip)->arr[i] == ':'); ++i)
    {
        if ((*ip)->arr[i] == ':')
            ++count_before;
        string_add_char(extanded, (*ip)->arr[i]);
    }
    for (int i = (*ip)->size - 2; i >= 0 && !((*ip)->arr[i + 1] == ':' && (*ip)->arr[i] == ':'); --i)
    {
        if ((*ip)->arr[i] == ':')
            ++count_after;
        ind = i + 1;
    }
    if (count_before != 7)
    {
        for (int i = 0; i < 8 - (count_before + count_after); ++i)
            string_add_str(extanded, "0:");
        for (size_t i = ind; i < (*ip)->size; ++i)
            string_add_char(extanded, (*ip)->arr[i]);
    }
    string_free(*ip);
    *ip = extanded;
}
