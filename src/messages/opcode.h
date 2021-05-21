#ifndef OPCODE_H
#define OPCODE_H

typedef enum
{
    OPCODE_QUERY = 0,
    OPCODE_IQUERY = 1,
    OPCODE_STATUS = 2
} OPCODE;

const char *opcode_to_string(OPCODE opcode);

#endif
