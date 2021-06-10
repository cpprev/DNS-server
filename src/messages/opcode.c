#include "messages/opcode.h"

const char *opcode_to_string(OPCODE opcode)
{
    switch (opcode)
    {
        case OPCODE_QUERY:
            return "OPCODE_QUERY";
        case OPCODE_IQUERY:
            return "OPCODE_IQUERY";
        case OPCODE_STATUS:
            return "OPCODE_STATUS";
        default:
            return "Unknown opcode";;
    }
}

bool is_opcode_valid(OPCODE opcode)
{
    return opcode == OPCODE_QUERY || opcode == OPCODE_IQUERY || opcode == OPCODE_STATUS;
}
