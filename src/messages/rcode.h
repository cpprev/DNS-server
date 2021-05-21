#ifndef RCODE_H
#define RCODE_H

typedef enum
{
    NO_ERR = 0,
    FORMAT_ERR = 1,
    SERV_FAIL = 2,
    NXDOMAIN = 3,
    NOT_IMPL = 4,
    REFUSED = 5,
} RCODE;

const char *rcode_to_string(RCODE rcode);

#endif
