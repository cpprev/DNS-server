#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include "messages/request/request.h"

#include "utils/string.h"

#include "config/record_type.h"

#include "server/protocol.h"

request *parse_request(PROTOCOL proto, string *req_bits);

string *binary_bits_to_ascii_string(string *qname_bits);

string *parse_whole_qname(size_t *i, size_t *until, string *req_bits);

void parse_request_headers(PROTOCOL proto, message *m, string *req_bits, size_t *i, size_t *until);

void parse_request_question(message *m, string *req_bits, size_t *i, size_t *until);

string *get_next_field(size_t *until, size_t step, size_t *i, string *bits);

#endif
