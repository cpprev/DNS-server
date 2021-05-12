#include <string.h>
#include <stdlib.h>

#include "parsing/request.h"

#include "utils/string.h"

string *get_next_field(size_t *until, size_t step, size_t *i, int *bits, size_t bits_size)
{
    string *res = string_init();
    *until += step;
    for (; *i < *until && *i < bits_size; (*i)++)
        string_add_char(res, bits[*i] + '0');
    return res;
}

// cf https://datatracker.ietf.org/doc/html/rfc1035#section-4
request *parse_request(int *bits, size_t sz)
{
    size_t i = 0, until = 0;

    // 1. Header section
    // 1.1. ID: 16 bits
    string *id = get_next_field(&until, 16, &i, bits, sz);
    printf("ID = %s\n", id->arr);
    // 1.2. QR (1 bit)
    string *qr = get_next_field(&until, 1, &i, bits, sz);
    printf("QR = %s\n", qr->arr);
    // 1.3. OPCode (4 bits)
    string *opcode = get_next_field(&until, 4, &i, bits, sz);
    printf("Opcode = %s\n", opcode->arr);
    // 1.4. AA (1 bit) -> required in responses (ignore here)
    i += 1; until += 1;
    // 1.5. TC (1 bit)
    string *tc = get_next_field(&until, 1, &i, bits, sz);
    printf("TC = %s\n", tc->arr);
    // 1.6. RD (1 bit) -> optional
    string *rd = get_next_field(&until, 1, &i, bits, sz);
    printf("RD = %s\n", rd->arr);
    // 1.7. RA (1 bit)
    string *ra = get_next_field(&until, 1, &i, bits, sz);
    printf("RA = %s\n", ra->arr);
    // 1.8. / 1.9. Z (3 bits) AND RCODE (4 bits) -> ignore in request
    i += 3 + 4; until += 3 + 4;
    // 1.10. QDCOUNT (16 bits)
    string *qdcount = get_next_field(&until, 16, &i, bits, sz);
    printf("qdcount = %s\n", qdcount->arr);
    // 1.11. ANCOUNT (16 bits)
    string *ancount = get_next_field(&until, 16, &i, bits, sz);
    printf("ancount = %s\n", ancount->arr);
    // 1.12. NSCOUNT (16 bits)
    string *nscount = get_next_field(&until, 16, &i, bits, sz);
    printf("nscount = %s\n", nscount->arr);
    // 1.13. ARCOUNT (16 bits)
    string *arcount = get_next_field(&until, 16, &i, bits, sz);
    printf("arcount = %s\n", arcount->arr);

    // 2. Question section
    // 2.0. 8 bits representing the length of QNAME field (in number of bytes)
    string *qnameLen = get_next_field(&until, 8, &i, bits, sz);
    printf("qnameLen = %s\n", qnameLen->arr);
    // 2.1. QNAME (domain name)
    // TODO replace with number of octets (convert qnameLen to decimal)
    string *qname = get_next_field(&until, 8, &i, bits, sz);
    printf("qname = %s\n", qname->arr);
    // skip 8 bits
    i += 8; until += 8;
    // 2.2. QTYPE (16 bits) AAAA = 28; A = 1; etc -> Cf https://en.wikipedia.org/wiki/List_of_DNS_record_types
    string *qtype = get_next_field(&until, 16, &i, bits, sz);
    printf("qtype = %s\n", qtype->arr);
    // 2.3. QCLASS (16 bits) -> IN class = 1 (ignore other classes)
    string *qclass = get_next_field(&until, 16, &i, bits, sz);
    printf("qclass = %s\n", qclass->arr);

    // Free memory
    string_free(qtype);
    string_free(qname);
    string_free(qnameLen);
    string_free(arcount);
    string_free(nscount);
    string_free(qdcount);
    string_free(ancount);
    string_free(ra);
    string_free(rd);
    string_free(tc);
    string_free(opcode);
    string_free(id);
    string_free(qr);

    return NULL;
}
