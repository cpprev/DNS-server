#include <stdlib.h>

#include "messages/message.h"

message *message_init()
{
    message *m = malloc(sizeof(message));
    m->id = -1;
    m->qr = QR_NONE;
    m->opcode = OPCODE_NONE;
    m->aa = false;
    m->tc = false;
    m->rd = false;
    m->ra = false;
    m->rcode = RCODE_NONE;
    m->qdcount = 0;
    m->ancount = 0;
    m->nscount = 0;
    m->arcount = 0;
    m->questions = NULL;
    m->answer = NULL;
    return m;
}

void message_free(message *m)
{
    if (m == NULL)
        return;
    if (m->questions != NULL)
    {
        for (int i = 0; m->questions[i] != NULL; ++i)
            question_free(m->questions[i]);
        free(m->questions);
    }
    if (m->answer != NULL)
        answer_free(m->answer);
    free(m);
}
