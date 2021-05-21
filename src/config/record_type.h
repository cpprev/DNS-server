#ifndef RECORD_TYPE_H
#define RECORD_TYPE_H

#include <stdbool.h>

// Cf. https://en.wikipedia.org/wiki/List_of_DNS_record_types
typedef enum
{
    RECORD_NONE = -1,
    A = 1,
    NS = 2,
    CNAME = 5,
    SOA = 6,
    TXT = 16,
    AAAA = 28
} RECORD_TYPE;

const char *record_type_to_string(RECORD_TYPE type);

bool is_supported_record_type(RECORD_TYPE type);

#endif
