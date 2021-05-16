#include <stdio.h>

#include "utils/base_convertions.h"

int power(int a, int b)
{
    int res = 1;
    for (; b > 0; --b)
        res *= a;
    return res;
}

int binary_to_decimal(string *in)
{
    int res = 0;
    for (int i = in->size - 1; i >= 0; --i)
    {
        char c = in->arr[i];
        res += (c - '0') * power(2, in->size - i - 1);
    }
    return res;
}

string *decimal_to_binary(int in)
{
    string *res = string_init();
    char buf[32];
    int i;
    for (i = 0; in > 0; ++i)
    {
        buf[i] = (in % 2) + '0';
        in /= 2;
    }
    for (int j = i - 1; j >= 0; --j)
        string_add_char(res, buf[j]);
    return res;
}
