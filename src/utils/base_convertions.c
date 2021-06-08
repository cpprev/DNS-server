#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils/base_convertions.h"

int power(int a, int b)
{
    int res = 1;
    for (; b > 0; --b)
        res *= a;
    return res;
}

int binary_to_decimal_unsigned(string *in)
{
    int res = 0;
    for (int i = in->size - 1; i >= 0; --i)
    {
        char c = in->arr[i];
        res += (c - '0') * power(2, in->size - i - 1);
    }
    return res;
}

int binary_to_decimal(string *in)
{
    int res = binary_to_decimal_unsigned(in);
    if (res > 127 && res < 256)
        res -= 256;
    return res;
}

string *hexa_to_binary(string *hex)
{
    char *binary[16] = { "0000", "0001", "0010", "0011",
                         "0100", "0101", "0110", "0111",
                         "1000", "1001", "1010", "1011",
                         "1100", "1101", "1110", "1111" };
    const char digits[16] = "0123456789abcdef";
    string *res = string_init();
    for (size_t i = 0; i < hex->size; ++i)
    {
        char c = tolower(hex->arr[i]);
        char *v = strchr(digits, c);
        if (v)
            string_add_str(res, binary[v - digits]);
            //strcat(res, binary[v - digits]);
    }
    return res;
}
