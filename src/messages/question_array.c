#include <stdlib.h>

#include "messages/question_array.h"

#define Q_ARR_CAP_ADD 8

question_array *question_array_init (void)
{
    question_array *q_arr = malloc(sizeof(question_array));
    q_arr->capacity = Q_ARR_CAP_ADD;
    q_arr->size = 0;
    q_arr->arr = malloc((q_arr->capacity + 1) * sizeof(question *));
    q_arr->arr[q_arr->size] = NULL;
    return q_arr;
}

question_array *question_array_copy (question_array *q_arr)
{
    if (q_arr == NULL)
        return NULL;
    question_array *new_arr = question_array_init();
    for (int i = 0; q_arr->arr[i]; ++i)
        question_array_add_copied_question(new_arr, q_arr->arr[i]);
    new_arr->capacity = q_arr->capacity;
    new_arr->size = q_arr->size;
    return new_arr;
}

void question_array_free (question_array *q_arr)
{
    if (q_arr == NULL)
        return;
    if (q_arr->arr != NULL)
    {
        for (size_t i = 0; i < q_arr->size; ++i)
            question_free(q_arr->arr[i]);
        free(q_arr->arr);
    }
    free(q_arr);
}

void question_array_resize (question_array *q_arr)
{
    q_arr->capacity += Q_ARR_CAP_ADD;
    q_arr->arr = realloc(q_arr->arr, (q_arr->capacity + 1) * sizeof(question *));
}

void question_array_add_question (question_array *q_arr, question *q)
{
    if (q_arr->size + 1 >= q_arr->capacity)
        question_array_resize(q_arr);
    q_arr->arr[q_arr->size] = q;
    q_arr->size++;
    q_arr->arr[q_arr->size] = NULL;
}

void question_array_add_copied_question (question_array *q_arr, question *q)
{
    if (q_arr->size + 1 >= q_arr->capacity)
        question_array_resize(q_arr);
    q_arr->arr[q_arr->size] = question_copy(q);
    q_arr->size++;
    q_arr->arr[q_arr->size] = NULL;
}