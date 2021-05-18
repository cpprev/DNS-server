#ifndef PARSE_REQUEST_H
#define PARSE_REQUEST_H

#include "messages/request.h"

#include "utils/string.h"

#include "config/record_type.h"

string *binary_bits_to_ascii_string(string *qname_bits);

request *parse_request(string *req_bits);

#endif
