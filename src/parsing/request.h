#ifndef REQUEST_H
#define REQUEST_H

#include "utils/string.h"

#include "config/record_type.h"

typedef struct
{
    string *id;
    string *qname;
    RECORD_TYPE qtype;
} request;

request *request_init();

void request_free(request *r);

request *parse_request(int *bits, size_t sz);

#endif
