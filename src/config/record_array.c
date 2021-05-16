#include <stdio.h>
#include <stdlib.h>

#include "config/record_array.h"

#define R_ARR_CAP_ADD 8

record_array *record_array_init (void)
{
    record_array *r_arr = malloc(sizeof(record_array));
    r_arr->capacity = R_ARR_CAP_ADD;
    r_arr->size = 0;
    r_arr->arr = malloc((r_arr->capacity + 1) * sizeof(record *));
    r_arr->arr[r_arr->size] = NULL;
    return r_arr;
}

void record_array_free (record_array *r_arr)
{
    if (r_arr == NULL)
        return;
    if (r_arr->arr != NULL)
    {
        for (size_t i = 0; i < r_arr->size; ++i)
            record_free(r_arr->arr[i]);
        free(r_arr->arr);
    }
    free(r_arr);
}

void record_array_resize (record_array *r_arr)
{
    r_arr->capacity += R_ARR_CAP_ADD;
    r_arr->arr = realloc(r_arr->arr, (r_arr->capacity + 1) * sizeof(record *));
}

void record_array_add_record (record_array *r_arr, record *r)
{
    if (r_arr->size + 1 >= r_arr->capacity)
        record_array_resize(r_arr);
    r_arr->arr[r_arr->size] = r;
    r_arr->size++;
    r_arr->arr[r_arr->size] = NULL;
}

void record_array_add_copied_record (record_array *r_arr, record *r)
{
    if (r_arr->size + 1 >= r_arr->capacity)
        record_array_resize(r_arr);
    r_arr->arr[r_arr->size] = record_copy(r);
    r_arr->size++;
    r_arr->arr[r_arr->size] = NULL;
}
