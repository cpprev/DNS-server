#include "messages/qr.h"

const char *qr_to_string(QR qr)
{
    switch (qr)
    {
        case REQUEST:
            return "REQUEST";
        case RESPONSE:
            return "RESPONSE";
        default:
            return "Unknown qr";;
    }
}
