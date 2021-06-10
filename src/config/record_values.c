#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "messages/response/response.h"

#include "utils/base_convertions.h"

#include "config/record_values.h"
#include "config/record.h"

void *record_val_to_bits(RECORD_TYPE type, string *val, size_t *b)
{
    switch(type)
    {
        case A:
            return A_init(val, b);
        case AAAA:
            return AAAA_init(val, b);
        case TXT:
            return TXT_init(val, b);
        case CNAME:
            return CNAME_init(val, b);
        case NS:
            return NS_init(val, b);
        case SOA:
            return SOA_init(val, b);
        default:
            return NULL;
    }
}

void *A_init(string *val, size_t *b)
{
    uint8_t *bits = malloc((4) * sizeof(uint8_t));
    string *tampon = string_init();
    for (size_t i = 0; val->arr[i]; ++i)
    {
        char c = val->arr[i];
        if (c == '.' || i == val->size - 1)
        {
            if (i == val->size - 1)
                string_add_char(tampon, c);
            bits[(*b)++] = atoi(tampon->arr);
            string_flush(tampon);
        }
        else
            string_add_char(tampon, c);
    }
    string_free(tampon);
    return bits;
}

void *AAAA_init(string *val, size_t *b)
{
    void *res = malloc((16) * sizeof(uint16_t));
    uint16_t *bits = res;
    int cur_b = 0;
    string *tampon = string_init();
    for (size_t i = 0; val->arr[i]; ++i)
    {
        char c = val->arr[i];
        if (c == ':' || i == val->size - 1)
        {
            if (i == val->size - 1)
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
    return res;
}

void *SOA_init(string *val, size_t *b)
{
    void *res = malloc((val->size * 2 + 1) * sizeof(uint8_t));

    string *mname = string_init(), *rname = string_init(), *serial = string_init(), *refresh = string_init(),
            *retry = string_init(), *expire = string_init(), *minimum = string_init();
    get_soa_values(val, &mname, &rname, &serial, &refresh, &retry, &expire, &minimum);

    domain_name_to_bits(mname, res, b);

    domain_name_to_bits(rname, res, b);

    uint32_t *bits = (uint32_t *) ((uint8_t *) res + *b);
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

    string_free(mname);
    string_free(rname);
    string_free(serial);
    string_free(refresh);
    string_free(retry);
    string_free(expire);
    string_free(minimum);

    return res;
}

void *TXT_init(string *val, size_t *b)
{
    void *res = malloc((val->size + 1) * sizeof(uint8_t));
    uint8_t *bits = res;
    bits[(*b)++] = val->size;
    for (size_t i = 0; val->arr[i]; ++i)
    {
        bits[(*b)++] = val->arr[i];
    }
    return res;
}

void *CNAME_init(string *val, size_t *b)
{
    void *res = malloc((val->size * 2 + 1) * sizeof(uint8_t));
    domain_name_to_bits(val, res, b);
    return res;
}

void *NS_init(string *val, size_t *b)
{
    void *res = malloc((val->size * 2 + 1) * sizeof(uint8_t));
    domain_name_to_bits(val, res, b);
    return res;
}
