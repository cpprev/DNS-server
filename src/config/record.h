#ifndef RECORD_H
#define RECORD_H

#include "utils/string.h"

typedef enum
{
    A,
    AAAA,
    SOA,
    TXT
} RECORD_TYPE;

typedef struct
{
    RECORD_TYPE type;
} record;

record *record_init();

void record_free(record *r);

#endif
