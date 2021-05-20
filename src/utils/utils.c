#include "utils/utils.h"

int count_number_dots(string *in)
{
    int res = 0;
    for (size_t i = 0; in->arr[i]; ++i)
    {
        if (in->arr[i] == '.')
            ++res;
    }
    return res;
}
