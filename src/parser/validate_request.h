#ifndef VALIDATE_REQUEST_H
#define VALIDATE_REQUEST_H

#include "utils/string.h"

#include "messages/request/request.h"

#include "server/protocol.h"

RCODE validate_request(string *req_bits, PROTOCOL proto, void *raw);

RCODE validate_request_headers(string *req_bits, PROTOCOL proto, size_t *i, size_t *until, int *qdcount, void *raw);

RCODE validate_request_questions(int qdcount, void *raw, size_t *b);

#endif
