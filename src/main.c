#include <stdio.h>
#include <stdlib.h>

#include "utils/string.h"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    string s = string_init();
    string_add_char(s, 'c');

    string_add_str(s, "abc");

    string_add_str(s, "abczejiguizguhzjeuiehizuhizeuhriuezhriuezhruizehrizuehrzieurh");

    for (int i = 0; s.arr[i]; ++i)
    {
        printf("%c", s.arr[i]);
    }
    printf("\n");

    // Parse input file
    return 0;
}
