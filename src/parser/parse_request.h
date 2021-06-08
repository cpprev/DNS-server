#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include "messages/request/request.h"

#include "utils/string.h"

#include "config/record_type.h"

#include "server/protocol.h"

request *parse_request(PROTOCOL proto, void *raw);

string *binary_bits_to_ascii_string(string *qname_bits);

string *parse_whole_qname(void *raw, size_t *b);

void parse_request_headers(PROTOCOL proto, message *m, void *raw, size_t *b);

void parse_request_question(message *m, void *raw, size_t *b);

string *get_next_field(size_t *until, size_t step, size_t *i, string *bits);

#endif
