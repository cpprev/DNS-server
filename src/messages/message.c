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
    m->raw_questions = NULL;
    m->raw_questions_size = 0;
    m->answers = NULL;
    m->authority = NULL;
    m->additional = NULL;
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
    new_m->authority = record_array_copy(m->authority);
    new_m->additional = record_array_copy(m->additional);

    new_m->raw_questions = malloc((m->raw_questions_size + 1) * sizeof(uint8_t));
    uint8_t *new_questions = new_m->raw_questions;
    uint8_t *old_questions = m->raw_questions;
    for (size_t i = 0; i < m->raw_questions_size; ++i)
        new_questions[i] = old_questions[i];
    new_m->raw_questions_size = m->raw_questions_size;

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
    if (m->authority != NULL)
        record_array_free(m->authority);
    if (m->additional != NULL)
        record_array_free(m->additional);
    if (m->raw_questions != NULL)
        free(m->raw_questions);
    free(m);
}
