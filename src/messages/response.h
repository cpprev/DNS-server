#ifndef RESPONSE_H
#define RESPONSE_H

#include "messages/message.h"

typedef struct
{
    message *msg;
} response;

response *response_init();

void response_free(response *r);

#endif
