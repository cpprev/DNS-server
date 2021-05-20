#ifndef REQUEST_H
#define REQUEST_H

#include "messages/message.h"

typedef struct
{
    message *msg;
} request;

request *request_init();

void request_free(request *r);

#endif
