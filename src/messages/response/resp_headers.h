#ifndef RESP_HEADERS_H
#define RESP_HEADERS_H

#include "utils/string.h"

#include "messages/response/response.h"

void message_headers_to_bits(message *msg, string *s);

#endif
