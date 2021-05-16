#include <stdlib.h>

#include "messages/answer.h"

answer *answer_init()
{
    answer *a = malloc(sizeof(answer));
    a->records = NULL;
    return a;
}

void answer_free(answer *ans)
{
    if (ans == NULL)
        return;
    if (ans->records != NULL)
        record_array_free(ans->records);
    free(ans);
}
