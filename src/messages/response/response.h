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

void message_to_bits(PROTOCOL proto, message *msg, void **bits, size_t *b);

void domain_name_to_bits(string *qname, void *raw, size_t *b);

#endif
