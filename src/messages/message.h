#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

#include "messages/question_array.h"
#include "messages/rcode.h"
#include "messages/qr.h"
#include "messages/opcode.h"

#include "config/record_array.h"

typedef struct
{
    uint16_t id;
    QR qr;
    OPCODE opcode;
    bool aa;
    bool tc;
    bool rd;
    bool ra;
    RCODE rcode;
    uint16_t qdcount;
    uint16_t ancount;
    uint16_t nscount;
    uint16_t arcount;
    // Question section
    question_array *questions;
    void *raw_questions;
    size_t raw_questions_size;
    // Answer section
    record_array *answers;
    // TODO auth + additional sections (unless not asked in subject)
} message;

message *message_init();

message *message_copy(message *m);

void message_free(message *m);

#endif
