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
        default:
            return "Unknown type";;
    }
}
