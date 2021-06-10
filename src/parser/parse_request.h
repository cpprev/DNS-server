#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include "messages/request/request.h"

#include "utils/string.h"

#include "config/record_type.h"

#include "server/protocol.h"

request *parse_request(PROTOCOL proto, void *raw, size_t size);

string *binary_bits_to_ascii_string(string *qname_bits);

string *parse_whole_qname(void *raw, size_t *b, uint8_t *raw_questions, size_t *raw_questions_b);

void parse_request_headers(PROTOCOL proto, message *m, void *raw, size_t *b);

void parse_request_question(message *m, void *raw, size_t *b, size_t size);

record_array *parse_request_records(int count, void *raw, size_t *b);

#endif
