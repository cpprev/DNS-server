#ifndef RECORD_H
#define RECORD_H

#include "utils/string.h"

typedef enum
{
    RECORD_NONE,
    A,
    NS,
    AAAA,
    SOA,
    CNAME,
    TXT
} RECORD_TYPE;

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

void print_record_type(record *r);

#endif
