#ifndef MESSAGE_H
#define MESSAGE_H

#include "utils/string.h"

#include "config/record_type.h"

typedef struct
{
    string *id;
    string *qname;
    RECORD_TYPE qtype;
} message;

message *message_init();

void message_free(message *m);

#endif
