#include <arpa/inet.h>

#include "messages/response/resp_question.h"

#include "utils/base_convertions.h"

void message_question_to_bits(message *msg, void *raw, size_t *b)
{
    if (msg->questions == NULL)
        return;
    if (msg->questions->size > 0)
    {
        uint8_t *bits = raw;
        uint8_t *raw_questions = msg->raw_questions;
        for (size_t i = 0; i < msg->raw_questions_size; ++i)
            bits[(*b)++] = raw_questions[i];
    }
}
