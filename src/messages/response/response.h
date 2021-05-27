#ifndef RESPONSE_H
#define RESPONSE_H

#include "config/server_config.h"

#include "messages/request/request.h"
#include "messages/message.h"

#include "server/protocol.h"

typedef struct
{
    message *msg;
} response;

response *response_init();

void response_free(response *r);

response *build_response(server_config *cfg, request *req);

string *response_to_bits(PROTOCOL proto, response *resp);

void write_domain_name_in_response(string *s, string *cur);

#endif
