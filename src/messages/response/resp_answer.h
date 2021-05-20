#ifndef RESP_ANSWER_H
#define RESP_ANSWER_H

#include "utils/string.h"

#include "messages/response/response.h"

void response_answer_to_bits(response *resp, string *s);

void write_answer_A_record(record *r, string *s);

void write_answer_A_record(record *r, string *s);

void write_answer_AAAA_record(record *r, string *s);

void write_answer_SOA_record(string *s, string *mname, string *rname, string *serial, string *refresh,
                             string *retry, string *expire, string *minimum);

void write_answer_TXT_record(record *r, string *s);

void write_answer_CNAME_record(record *r, string *s);

void write_answer_NS_record(record *r, string *s);

#endif
