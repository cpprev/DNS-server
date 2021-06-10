#ifndef RECORD_H
#define RECORD_H

#include <stdint.h>

#include "utils/string.h"

#include "config/record_type.h"
#include "config/class_type.h"

typedef struct
{
    // 1st value
    void *domain;
    size_t domain_size;
    string *string_domain;
    // 2nd value
    RECORD_TYPE type;
    // 3rd value
    int32_t ttl;
    // 4th value
    void *value;
    size_t value_size;
    string *string_value;
    // Class (IN class only, for now TODO)
    CLASS_TYPE class;
} record;

record *record_init();

record *record_copy(record *r);

void record_free(record *r);

record *parse_record(string *zone_name, string *in, string *error);

void get_soa_values(string *s, string **mname, string **rname, string **serial, string **refresh, string **retry, string **expire, string **minimum);

#endif
