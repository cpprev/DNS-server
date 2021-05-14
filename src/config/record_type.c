#include <stdio.h>

#include "config/record_type.h"

// Cf. https://en.wikipedia.org/wiki/List_of_DNS_record_types
RECORD_TYPE int_to_record_type(int qtype)
{
    switch (qtype)
    {
        case 1:
            return A;
        case 2:
            return NS;
        case 5:
            return CNAME;
        case 6:
            return SOA;
        case 16:
            return TXT;
        case 28:
            return AAAA;
        default:
            return RECORD_NONE;
    }
}

void print_record_type(RECORD_TYPE type)
{
    switch (type)
    {
        case A:
            printf("A");
            break;
        case NS:
            printf("NS");
            break;
        case CNAME:
            printf("CNAME");
            break;
        case SOA:
            printf("SOA");
            break;
        case TXT:
            printf("TXT");
            break;
        case AAAA:
            printf("AAAA");
            break;
        default:
            printf("Unknown type");;
            break;
    }
}
