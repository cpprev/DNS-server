#ifndef MESSAGE_H
#define MESSAGE_H

#include "messages/question_array.h"
#include "messages/rcode.h"
#include "messages/qr.h"
#include "messages/opcode.h"

#include "config/record_array.h"

typedef struct
{
    int id;
    QR qr;
    OPCODE opcode;
    bool aa;
    bool tc;
    bool rd;
    bool ra;
    RCODE rcode;
    int qdcount;
    int ancount;
    int nscount;
    int arcount;
    // Question section
    question_array *questions;
    // Answer section
    record_array *answers;
    // TODO auth + additional sections (unless not asked in subject)
} message;

message *message_init();

message *message_copy(message *m);

void message_free(message *m);

#endif
