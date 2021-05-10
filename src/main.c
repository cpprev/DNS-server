#include <stdio.h>
#include <stdlib.h>

#include "utils/string.h"
#include "config/zone_array.h"
#include "config/zone.h"

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    string *s = string_init();
    string_add_char(s, 'c');

    string_add_str(s, "abc");

    string_add_str(s, "abczejiguizguhzjeuiehizuhizeuhriuezhriuezhruizehrizuehrzieurh");

    for (size_t i = 0; i < s->size; ++i)
    {
        printf("%c", s->arr[i]);
    }
    printf("\n");



    zone_array *z_arr = zone_array_init();

    zone *z1 = malloc(20);
    z1->tmp = "z1 string";
    zone *z2 = malloc(20);
    z2->tmp = "z2 string";
    zone *z3 = malloc(20);
    z3->tmp = "z3 string";

    zone_array_add_zone(z_arr, z1);
    zone_array_add_zone(z_arr, z2);
    zone_array_add_zone(z_arr, z3);

    for (size_t i = 0; i < z_arr->size; ++i)
    {
        puts(z_arr->zones[i]->tmp);
    }

    // Parse input file
    return 0;
}
