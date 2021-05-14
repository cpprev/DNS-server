#ifndef RECORD_H
#define RECORD_H

#include "utils/string.h"

#include "config/record_type.h"

typedef struct
{
    // 1st value
    string *domain;
    // 2nd value
    RECORD_TYPE type;
    // 3rd value
    int ttl;
    // 4th value
    string *value;
} record;

record *record_init();

void record_free(record *r);

record *parse_record(string *in);

#endif
