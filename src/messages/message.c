#include <stdlib.h>

#include "messages/message.h"

message *message_init()
{
    message *m = malloc(sizeof(message));
    m->id = -1;
    m->qr = QR_NONE;
    m->opcode = OPCODE_QUERY;
    m->aa = false;
    m->tc = false;
    m->rd = false;
    m->ra = false;
    m->rcode = NO_ERR;
    m->qdcount = 0;
    m->ancount = 0;
    m->nscount = 0;
    m->arcount = 0;
    m->questions = NULL;
    m->answers = NULL;
    return m;
}

message *message_copy(message *m)
{
    message *new_m = message_init();
    new_m->id = m->id;
    new_m->qr = m->qr;
    new_m->opcode = m->opcode;
    new_m->aa = m->aa;
    new_m->tc = m->tc;
    new_m->rd = m->rd;
    new_m->ra = m->ra;
    new_m->rcode = m->rcode;
    new_m->qdcount = m->qdcount;
    new_m->ancount = m->ancount;
    new_m->nscount = m->nscount;
    new_m->arcount = m->arcount;
    new_m->questions = question_array_copy(m->questions);
    new_m->answers = record_array_copy(m->answers);
    return new_m;
}

void message_free(message *m)
{
    if (m == NULL)
        return;
    if (m->questions != NULL)
        question_array_free(m->questions);
    if (m->answers != NULL)
        record_array_free(m->answers);
    free(m);
}
