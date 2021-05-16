#ifndef MESSAGE_H
#define MESSAGE_H

#include "messages/question.h"

typedef struct
{
    string *id;
    question **questions;
} message;

message *message_init();

void message_free(message *m);

#endif
