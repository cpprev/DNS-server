#include <stdlib.h>

#include "messages/message.h"

message *message_init()
{
    message *m = malloc(sizeof(message));
    m->id = NULL;
    m->questions = NULL;
    return m;
}

void message_free(message *m)
{
    if (m == NULL)
        return;
    if (m->id != NULL)
        string_free(m->id);
    if (m->questions != NULL)
    {
        for (int i = 0; m->questions[i] != NULL; ++i)
            question_free(m->questions[i]);
        free(m->questions);
    }
    free(m);
}
