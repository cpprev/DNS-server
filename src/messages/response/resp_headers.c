#include <arpa/inet.h>

#include "messages/response/resp_headers.h"

#include "utils/base_convertions.h"

void message_headers_to_bits(message *msg, void *raw, size_t *b)
{
    uint16_t *bits = raw;
    // Id (16 bits)
    bits[(*b)++] = htons(msg->id);

    // Flags : QR, OPCode, AA, TC, RD, RA, Z, RCODE -> (16 bits)
    uint16_t flags = 0x0;
    flags |= msg->qr << 15 | msg->opcode << 11
            | msg->aa << 10 | msg->tc << 9
            | msg->rd << 8 | msg->ra << 7
            | 0 << 4 | msg->rcode;
    bits[(*b)++] = ntohs(flags);

    // QDCOUNT (16 bits)
    bits[(*b)++] = htons(msg->qdcount);
    // ANCOUNT (16 bits)
    bits[(*b)++] = htons(msg->ancount);
    // NSCOUNT (16 bits)
    bits[(*b)++] = htons(msg->nscount);
    // ARCOUNT (16 bits)
    bits[(*b)++] = htons(msg->arcount);

    (*b) *= 2;
}
