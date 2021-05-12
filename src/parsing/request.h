#ifndef REQUEST_H
#define REQUEST_H

#include "utils/string.h"

typedef struct
{
    string *id;
} request;

void free_request(request *r);

request *parse_request(int *bits, size_t sz);

#endif
