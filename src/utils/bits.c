#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "utils/bits.h"

bits *bits_init (void)
{
    bits *b = malloc(sizeof(bits));
    b->size = 0;
    b->arr = NULL;
    return b;
}

void bits_free (bits *b)
{
    if (b == NULL)
        return;
    if (b->arr != NULL)
        free(b->arr);
    free(b);
}

void bits_resize (bits *b, size_t add_size)
{
    b->arr = realloc(b->arr, (b->size + add_size + 1) * sizeof(uint8_t));
}

void bits_add_bits (bits **b1, uint8_t *b2, size_t size_b2)
{
    if (*b1 == NULL)
        *b1 = bits_init();
    bits_resize(*b1, size_b2);

    for (size_t i = 0; i < size_b2; ++i)
        (*b1)->arr[(*b1)->size++] = b2[i];
}
