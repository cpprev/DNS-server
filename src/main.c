#include <stdio.h>
#include <stdlib.h>

#include "utils/string.h"
#include "config/server_config.h"
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


    server_config *cfg = server_config_init(s, 10, z_arr);
    for (size_t i = 0; i < cfg->ip->size; ++i)
    {
        printf("%c", cfg->ip->arr[i]);
    }
    printf("\n");
    for (size_t i = 0; i < cfg->zones->size; ++i)
    {
        puts(cfg->zones->arr[i]->tmp);
    }


    // Parse input file
    return 0;
}
