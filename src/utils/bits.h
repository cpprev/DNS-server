#ifndef BITS_H
#define BITS_H

#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    uint8_t *arr;
    size_t size;
} bits;

bits *bits_init (void);

void bits_free (bits *b);

void bits_resize (bits *b, size_t add_size);

void bits_add_bits (bits **b1, uint8_t *b2, size_t size_b2);

#endif
