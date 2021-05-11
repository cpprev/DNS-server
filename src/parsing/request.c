#include <string.h>
#include <stdlib.h>

#include "parsing/request.h"

#include "utils/string.h"

// cf https://datatracker.ietf.org/doc/html/rfc1035#section-4
request *parse_request(int *bits, size_t sz)
{
    size_t i;
    // 1. Header section
    // 1.1. ID: 16 bits
    string *id = string_init();
    size_t until = 16;
    for (i = 0; i < until && i < sz; ++i)
        string_add_char(id, bits[i] + '0');
    printf("ID = ");
    string_print(id);

    // 1.2. QR (1 bit)
    string *qr = string_init();
    until += 1;
    for (; i < until && i < sz; ++i)
        string_add_char(qr, bits[i] + '0');
    printf("QR = ");
    string_print(qr);

    // 1.3. OPCode (4 bits)
    string *opcode = string_init();
    until += 4;
    for (; i < until && i < sz; ++i)
        string_add_char(opcode, bits[i] + '0');
    printf("Opcode = ");
    string_print(opcode);

    // 1.4. AA (1 bit) -> required in responses (ignore here)
    i += 1;
    until += 1;

    // 1.5. TC (1 bit)
    string *tc = string_init();
    until += 1;
    for (; i < until && i < sz; ++i)
        string_add_char(tc, bits[i] + '0');
    printf("TC = ");
    string_print(tc);

    // 1.6. RD (1 bit) -> optional
    string *rd = string_init();
    until += 1;
    for (; i < until && i < sz; ++i)
        string_add_char(rd, bits[i] + '0');
    printf("RD = ");
    string_print(rd);

    // 1.7. RA (1 bit)
    string *ra = string_init();
    until += 1;
    for (; i < until && i < sz; ++i)
        string_add_char(ra, bits[i] + '0');
    printf("RA = ");
    string_print(ra);

    // 1.8. Z (3 bits) -> ignore
    i += 3;
    until += 3;

    // 1.9. RCODE (4 bits) -> ignore (needed in responses)
    i += 4;
    until += 4;

    // 1.10. QDCOUNT (16 bits)
    string *qdcount = string_init();
    until += 16;
    for (; i < until && i < sz; ++i)
        string_add_char(qdcount, bits[i] + '0');
    printf("qdcount = ");
    string_print(qdcount);

    // 1.11. ANCOUNT (16 bits)
    string *ancount = string_init();
    until += 16;
    for (; i < until && i < sz; ++i)
        string_add_char(ancount, bits[i] + '0');
    printf("ancount = ");
    string_print(ancount);

    // 1.12. NSCOUNT (16 bits)
    string *nscount = string_init();
    until += 16;
    for (; i < until && i < sz; ++i)
        string_add_char(nscount, bits[i] + '0');
    printf("nscount = ");
    string_print(nscount);

    // 1.13. ARCOUNT (16 bits)
    string *arcount = string_init();
    until += 16;
    for (; i < until && i < sz; ++i)
        string_add_char(arcount, bits[i] + '0');
    printf("arcount = ");
    string_print(arcount);

    printf("%ld\n",i);

    for (; i < sz; ++i)
        printf("%d", bits[i]);
    puts("\n");
    // 2. Question section

    // 2.0. 00000001 byte to skip (from what I obsered)
    i += 8;
    until += 8;

    // 2.1. QNAME (domain name)

    // 2.2. QTYPE (16 bits)

    // 2.3. QCLASS (16 bits) -> We handle IN class so ignore ?

    // Free memory
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
