#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include "messages/request.h"

#include "utils/string.h"

#include "config/record_type.h"

request *parse_request(int *bits, size_t sz);

#endif
