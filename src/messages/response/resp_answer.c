#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "messages/response/resp_answer.h"

#include "utils/base_convertions.h"

int get_answer_value_length(record *r, int soa_size)
{
    switch (r->type)
    {
        case A:
            return 4;
        case AAAA:
            return 16;
        case TXT:
            // "+ 1" : for the single length octet required before the TXT string
            return r->value->size + 1;
        case CNAME:
            // Length of string : <len_octet1>label1<len_octet2>label2,...,<null_octet>
            return r->value->size + 1;
        case SOA:
            // Length of : MNAME + RNAME + SERIAL + REFRESH + RETRY + EXPIRE + MINIMUM
            return soa_size + 4 + 4 + 4 + 4 + 4;
        case NS:
            // Length of string : <len_octet1>label1<len_octet2>label2,...,<null_octet>
            return r->value->size + 1;
        default:
            return -1;
    }
}

void message_answer_to_bits(message *msg, void *raw, size_t *b)
{
    if (msg->answers == NULL)
        return;
    uint8_t *bits = raw;
    for (size_t k = 0; msg->answers->arr[k]; ++k)
    {
        record *r = msg->answers->arr[k];
        // NAME
        domain_name_to_bits(r->domain, bits, b);

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
        // SOA vals
        string *mname = string_init(), *rname = string_init(), *serial = string_init(), *refresh = string_init(),
                *retry = string_init(), *expire = string_init(), *minimum = string_init();
        get_soa_values(r->value, &mname, &rname, &serial, &refresh, &retry, &expire, &minimum);
        int rdlenInt = get_answer_value_length(r, (mname->size + 1) + (rname->size + 1));
        cur[4] = htons(rdlenInt);
        *b += 2;

        if (r->type == A)
            write_answer_A_record(r, raw, b);
        else if (r->type == AAAA)
            write_answer_AAAA_record(r, raw, b);
        else if (r->type == CNAME)
            write_answer_CNAME_record(r, raw, b);
        else if (r->type == TXT)
            write_answer_TXT_record(r, raw, b);
        else if (r->type == SOA)
            write_answer_SOA_record(mname, rname, serial, refresh, retry, expire, minimum, raw, b);
        else if (r->type == NS)
            write_answer_NS_record(r, raw, b);

        // SOA vals free
        string_free(mname);
        string_free(rname);
        string_free(serial);
        string_free(refresh);
        string_free(retry);
        string_free(expire);
        string_free(minimum);
    }
}

void write_answer_A_record(record *r, void *raw, size_t *b)
{
    string *tampon = string_init();
    uint8_t *bits = raw;
    for (size_t i = 0; r->value->arr[i]; ++i)
    {
        char c = r->value->arr[i];
        if (c == '.' || i == r->value->size - 1)
        {
            if (i == r->value->size - 1)
                string_add_char(tampon, c);
            bits[(*b)++] = atoi(tampon->arr);
            string_flush(tampon);
        }
        else
            string_add_char(tampon, c);
    }
    string_free(tampon);
}

void write_answer_AAAA_record(record *r, void *raw, size_t *b)
{
    string *tampon = string_init();
    uint16_t *bits = (uint16_t *) ((uint8_t *)raw + *b);
    int cur_b = 0;
    for (size_t i = 0; r->value->arr[i]; ++i)
    {
        char c = r->value->arr[i];
        if (c == ':' || i == r->value->size - 1)
        {
            if (i == r->value->size - 1)
                string_add_char(tampon, c);
            string *rdata_temp = hexa_to_binary(tampon);
            bits[cur_b++] = htons(binary_to_decimal(rdata_temp));
            *b += 2;
            string_flush(tampon);
        }
        else
            string_add_char(tampon, c);
    }
    string_free(tampon);
}

void write_answer_SOA_record(string *mname, string *rname, string *serial, string *refresh,
                             string *retry, string *expire, string *minimum, void *raw, size_t *b)
{
    domain_name_to_bits(mname, raw, b);

    domain_name_to_bits(rname, raw, b);

    uint32_t *bits = (uint32_t *) ((uint8_t *) raw + *b);
    bits[0] = htonl(atoi(serial->arr));
    *b += 4;
    bits[1] = htonl(atoi(refresh->arr));
    *b += 4;
    bits[2] = htonl(atoi(retry->arr));
    *b += 4;
    bits[3] = htonl(atoi(expire->arr));
    *b += 4;
    bits[4] = htonl(atoi(minimum->arr));
    *b += 4;
}

void write_answer_TXT_record(record *r, void *raw, size_t *b)
{
    uint8_t *bits = raw;
    bits[(*b)++] = r->value->size;
    for (size_t i = 0; r->value->arr[i]; ++i)
    {
        bits[(*b)++] = r->value->arr[i];
    }
}

void write_answer_CNAME_record(record *r, void *raw, size_t *b)
{
    domain_name_to_bits(r->value, raw, b);
}

void write_answer_NS_record(record *r, void *raw, size_t *b)
{
    domain_name_to_bits(r->value, raw, b);
}
