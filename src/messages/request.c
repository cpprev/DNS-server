#include <stdlib.h>

#include "messages/request.h"

request *request_init()
{
    request *r = malloc(sizeof(request));
    r->msg = NULL;
    return r;
}

void request_free(request *r)
{
    if (r == NULL)
        return;
    if (r->msg != NULL)
        message_free(r->msg);
    free(r);
}
