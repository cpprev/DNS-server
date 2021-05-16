#include <stdlib.h>

#include "messages/question.h"

question *question_init()
{
    question *q = malloc(sizeof(question));
    q->qname = NULL;
    return q;
}

void question_free(question *q)
{
    if (q == NULL)
        return;
    if (q->qname != NULL)
        string_free(q->qname);
    free(q);
}
