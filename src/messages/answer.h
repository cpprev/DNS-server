#ifndef ANSWER_H
#define ANSWER_H

#include "config/record_array.h"

typedef struct
{
    record_array *records;
} answer;

answer *answer_init();

void answer_free(answer *ans);

#endif
