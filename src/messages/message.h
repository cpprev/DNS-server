#ifndef MESSAGE_H
#define MESSAGE_H

#include "messages/question_array.h"

#include "config/record_array.h"

typedef enum
{
    QR_NONE,
    REQUEST = 0,
    RESPONSE = 1
} QR;

typedef enum
{
    OPCODE_QUERY = 0,
    OPCODE_IQUERY = 1,
    OPCODE_STATUS = 2
} OPCODE;

typedef enum
{
    NO_ERR = 0,
    FORMAT_ERR = 1,
    SERV_FAIL = 2,
    NAME_ERR = 3,
    NOT_IMPL = 4,
    REFUSED = 5
} RCODE;

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
