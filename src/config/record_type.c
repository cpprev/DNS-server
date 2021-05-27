#include <stdio.h>

#include "config/record_type.h"

const char *record_type_to_string(RECORD_TYPE type)
{
    switch (type)
    {
        case A:
            return "A";
        case NS:
            return "NS";
        case CNAME:
            return "CNAME";
        case SOA:
            return "SOA";
        case TXT:
            return "TXT";
        case AAAA:
            return "AAAA";
        case ANY:
            return "ANY";
        default:
            return "Unknown type";;
    }
}

bool is_supported_record_type(RECORD_TYPE type)
{
    return type == A || type == AAAA || type == NS || type == TXT || type == SOA || type == CNAME || type == ANY;
}
