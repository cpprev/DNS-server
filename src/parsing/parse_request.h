#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include "messages/request.h"

#include "utils/string.h"

#include "config/record_type.h"

string *parse_qname(string *qnamebits);

request *parse_request(string *bits);

#endif
