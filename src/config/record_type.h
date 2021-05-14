#ifndef RECORD_TYPE_H
#define RECORD_TYPE_H

typedef enum
{
    RECORD_NONE,
    A,
    NS,
    AAAA,
    SOA,
    CNAME,
    TXT
} RECORD_TYPE;

RECORD_TYPE int_to_record_type(int qtype);

void print_record_type(RECORD_TYPE type);

#endif
