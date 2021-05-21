#include "messages/rcode.h"

const char *rcode_to_string(RCODE rcode)
{
    switch (rcode)
    {
        case NO_ERR:
            return "NO_ERR";
        case FORMAT_ERR:
            return "FORMAT_ERR";
        case SERV_FAIL:
            return "SERV_FAIL";
        case NXDOMAIN:
            return "NXDOMAIN";
        case NOT_IMPL:
            return "NOT_IMPL";
        case REFUSED:
            return "REFUSED";
        default:
            return "Unknown rcode";;
    }
}
