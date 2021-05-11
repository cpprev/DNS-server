#include <stdio.h>
#include <stdlib.h>

#include "config/zone_array.h"

#define Z_ARR_CAP_ADD 16

zone_array *zone_array_init (void)
{
    zone_array *z_arr = malloc(sizeof(zone_array));
    z_arr->capacity = Z_ARR_CAP_ADD;
    z_arr->size = 0;
    z_arr->arr = malloc((z_arr->capacity + 1) * sizeof(zone *));
    z_arr->arr[z_arr->size] = NULL;
    return z_arr;
}

void zone_array_free (zone_array *z_arr)
{
    if (z_arr == NULL)
        return;
    if (z_arr->arr != NULL)
    {
        for (int i = 0; z_arr->arr[i]; ++i)
            free(z_arr->arr[i]);
        free(z_arr->arr);
    }
    free(z_arr);
}

void zone_array_resize (zone_array *z_arr)
{
    z_arr->capacity += Z_ARR_CAP_ADD;
    z_arr->arr = realloc(z_arr->arr, (z_arr->capacity + 1) * sizeof(zone *));
}

void zone_array_add_zone (zone_array *z_arr, zone *z)
{
    if (z_arr->size + 1 >= z_arr->capacity)
        zone_array_resize(z_arr);
    z_arr->arr[z_arr->size] = z;
    z_arr->size++;
    z_arr->arr[z_arr->size] = NULL;
}
