#include <stdlib.h>

#include "messages/message.h"

message *message_init()
{
    message *m = malloc(sizeof(message));
    m->id = NULL;
    m->qname = NULL;
    m->qtype = RECORD_NONE;
    return m;
}

void message_free(message *m)
{
    if (m == NULL)
        return;
    if (m->id != NULL)
        string_free(m->id);
    if (m->qname != NULL)
        string_free(m->qname);
    free(m);
}

