#ifndef RESP_ANSWER_H
#define RESP_ANSWER_H

#include "utils/string.h"

#include "messages/response/response.h"

void message_answer_to_bits(message *msg, void *raw, size_t *b);

#endif
