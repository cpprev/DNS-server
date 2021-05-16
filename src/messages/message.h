#ifndef MESSAGE_H
#define MESSAGE_H

#include "messages/question.h"
#include "messages/answer.h"

typedef enum
{
    QR_NONE = -1,
    REQUEST = 0,
    RESPONSE = 1
} QR;

typedef enum
{
    OPCODE_NONE = -1,
    OPCODE_QUERY = 0,
    OPCODE_IQUERY = 1,
    OPCODE_STATUS = 2
} OPCODE;

typedef enum
{
    RCODE_NONE = -1,
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
    question **questions;
    answer *answer;
} message;

message *message_init();

void message_free(message *m);

#endif
