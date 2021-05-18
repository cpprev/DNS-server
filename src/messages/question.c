#include <stdlib.h>

#include "messages/question.h"

question *question_init()
{
    question *q = malloc(sizeof(question));
    q->qname = NULL;
    return q;
}

question *question_copy(question *q)
{
    question *new_q = question_init();
    string_copy(&new_q->qname, q->qname);
    new_q->qtype = q->qtype;
    return new_q;
}

void question_free(question *q)
{
    if (q == NULL)
        return;
    if (q->qname != NULL)
        string_free(q->qname);
    free(q);
}
