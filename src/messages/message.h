#ifndef MESSAGE_H
#define MESSAGE_H

#include <stdint.h>

#include "messages/question_array.h"
#include "messages/rcode.h"
#include "messages/qr.h"
#include "messages/opcode.h"

#include "config/record_array.h"

#define UDP_HEADER_SIZE (96 / 8)
#define TCP_HEADER_SIZE (96 / 8 + 2)

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
    record_array *authority;
    record_array *additional;
} message;

message *message_init();

message *message_copy(message *m);

void message_free(message *m);

#endif
