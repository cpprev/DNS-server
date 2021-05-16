#ifndef RESPONSE_H
#define RESPONSE_H

#include "config/server_config.h"

#include "messages/request.h"
#include "messages/message.h"

typedef struct
{
    message *msg;
} response;

response *response_init();

void response_free(response *r);

response *build_response(server_config *cfg, request *req);

int *response_to_bits(response *resp);

#endif
