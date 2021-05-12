#ifndef REQUEST_H
#define REQUEST_H

#include "utils/string.h"

typedef struct
{
    string *id;
} request;

request *request_init();

void request_free(request *r);

request *parse_request(int *bits, size_t sz);

#endif
