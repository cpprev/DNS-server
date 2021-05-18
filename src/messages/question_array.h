#ifndef QUESTION_ARRAY_H
#define QUESTION_ARRAY_H

#include "messages/question.h"

typedef struct
{
    size_t size;
    size_t capacity;
    question **arr;
} question_array;

question_array *question_array_init (void);

void question_array_free (question_array *q_arr);

void question_array_resize (question_array *q_arr);

void question_array_add_question (question_array *q_arr, question *q);

#endif
