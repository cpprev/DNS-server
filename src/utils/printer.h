#ifndef PRINTER_H
#define PRINTER_H

#include "messages/request/request.h"
#include "messages/response/response.h"

#include "config/server_config.h"

void print_server_config(server_config* cfg);

void print_message(message *msg);

void print_request(PROTOCOL proto, request* req);

void print_response(response* resp);

void string_print(string *s);

void print_record_type(RECORD_TYPE type);

#endif
