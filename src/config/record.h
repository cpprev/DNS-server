#ifndef RECORD_H
#define RECORD_H

#include "utils/string.h"

#include "config/record_type.h"

typedef enum
{
    IN = 1
} CLASS;

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
    // Class (IN class only, for now TODO)
    CLASS class;
} record;

record *record_init();

record *record_copy(record *r);

void record_free(record *r);

record *parse_record(string *in);

void get_soa_values(string *s, string **mname, string **rname, string **serial, string **refresh, string **retry, string **expire, string **minimum);

#endif
