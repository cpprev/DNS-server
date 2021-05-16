#include <stdio.h>

#include "config/record_type.h"

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
