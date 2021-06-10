#ifndef OPCODE_H
#define OPCODE_H

#include <stdbool.h>

typedef enum
{
    OPCODE_QUERY = 0,
    OPCODE_IQUERY = 1,
    OPCODE_STATUS = 2
} OPCODE;

const char *opcode_to_string(OPCODE opcode);

bool is_opcode_valid(OPCODE opcode);

#endif
