#ifndef RESP_QUESTION_H
#define RESP_QUESTION_H

#include "utils/string.h"

#include "messages/response/response.h"

void message_question_to_bits(message *msg, void *raw, size_t *b);

#endif
