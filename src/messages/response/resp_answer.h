#ifndef RESP_ANSWER_H
#define RESP_ANSWER_H

#include "utils/string.h"

#include "messages/response/response.h"

void message_answer_to_bits(message *msg, void *raw, size_t *b);

void write_answer_A_record(record *r, void *raw, size_t *b);

void write_answer_AAAA_record(record *r, void *raw, size_t *b);

void write_answer_SOA_record(string *mname, string *rname, string *serial, string *refresh,
                             string *retry, string *expire, string *minimum, void *raw, size_t *b);

void write_answer_TXT_record(record *r, void *raw, size_t *b);

void write_answer_CNAME_record(record *r, void *raw, size_t *b);

void write_answer_NS_record(record *r, void *raw, size_t *b);

#endif
