#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "messages/response/resp_answer.h"

#include "utils/base_convertions.h"

void message_answer_to_bits(message *msg, void *raw, size_t *b)
{
    if (msg->answers == NULL)
        return;
    uint8_t *bits = raw;
    for (size_t k = 0; msg->answers->arr[k]; ++k)
    {
        record *r = msg->answers->arr[k];
        // NAME
        uint8_t *domain = r->domain;
        for (size_t i = 0; i < r->domain_size; ++i)
            bits[(*b)++] = domain[i];
        //domain_name_to_bits(r->domain, bits, b);

        // Null byte after name
        bits[(*b)++] = 0;

        // TYPE & CLASS & TTL
        uint16_t *cur = (uint16_t *)((uint8_t *) raw + *b);
        cur[0] = htons(r->type);
        *b += 2;
        cur[1] = htons(r->class);
        *b += 2;
        uint32_t *ttl = (uint32_t *)((uint8_t *)raw + *b);
        ttl[0] = htonl(r->ttl);
        *b += 4;

        cur[4] = htons(r->value_size);
        *b += 2;

        uint8_t *val = r->value;
        for (size_t i = 0; i < r->value_size; ++i)
            bits[(*b)++] = val[i];
    }
}
