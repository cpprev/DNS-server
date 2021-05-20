#ifndef RESP_HEADERS_H
#define RESP_HEADERS_H

#include "utils/string.h"

#include "messages/response/response.h"

void response_headers_to_bits(response *resp, string *s);

#endif
