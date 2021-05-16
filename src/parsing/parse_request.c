#include <string.h>
#include <stdlib.h>

#include "parsing/parse_request.h"

#include "messages/question.h"
#include "messages/message.h"
#include "messages/request.h"
#include "messages/response.h"

#include "utils/string.h"
#include "utils/base_convertions.h"

string *get_next_field(size_t *until, size_t step, size_t *i, int *bits, size_t bits_size)
{
    string *res = string_init();
    *until += step;
    for (; *i < *until && *i < bits_size; (*i)++)
        string_add_char(res, bits[*i] + '0');
    return res;
}

string *parse_qname(string *qnamebits)
{
    string *res = string_init();
    string *tampon = string_init();
    for (size_t i = 0; i < qnamebits->size; ++i)
    {
        if ((i > 0 && i % 8 == 0) || i == qnamebits->size - 1)
        {
            if (i == qnamebits->size - 1)
                string_add_char(tampon, qnamebits->arr[i]);
            string_print(tampon);
            string_add_char(res, (char)binary_to_decimal(tampon));
            string_flush(tampon);
        }
        string_add_char(tampon, qnamebits->arr[i]);
    }
    string_free(tampon);
    return res;
}

string *parse_whole_qname(size_t *i, size_t *until, size_t sz, int *bits)
{
    string *tampon = string_init(), *qname = string_init();
    int tmp_len = 0;
    size_t i_init = *i;
    for (; *i < sz; ++(*i))
    {
        if (*i > i_init && *i % 8 == 0)
        {
            int dec = binary_to_decimal(tampon);
            if (dec > 0 && dec < 65)
            {
                (*until) += tampon->size;
                tmp_len = dec;
                string *qnamebits = get_next_field(until, tmp_len * 8, i, bits, sz);
                string *tmp_name = parse_qname(qnamebits);
                if (!string_is_empty(qname))
                    string_add_char(qname, '.');
                string_add_str(qname, tmp_name->arr);
                string_free(qnamebits);
                string_free(tmp_name);
            }
            else
            {
                (*until) += tampon->size;
                break;
            }
            string_flush(tampon);
        }
        string_add_char(tampon, bits[*i] + '0');
    }
    string_free(tampon);
    return qname;
}

// Cf https://datatracker.ietf.org/doc/html/rfc1035#section-4
request *parse_request(int *bits, size_t sz)
{
    message *m = message_init();

    size_t i = 0, until = 0;

    // 1. Header section
    // 1.1. ID: 16 bits
    m->id = get_next_field(&until, 16, &i, bits, sz);
    // 1.2. QR (1 bit)
    string *qr = get_next_field(&until, 1, &i, bits, sz);
    // 1.3. OPCode (4 bits)
    string *opcode = get_next_field(&until, 4, &i, bits, sz);
    // 1.4. AA (1 bit) -> required in responses (ignore here)
    i += 1; until += 1;
    // 1.5. TC (1 bit)
    string *tc = get_next_field(&until, 1, &i, bits, sz);
    // 1.6. RD (1 bit) -> optional
    string *rd = get_next_field(&until, 1, &i, bits, sz);
    // 1.7. RA (1 bit)
    string *ra = get_next_field(&until, 1, &i, bits, sz);
    // 1.8. / 1.9. Z (3 bits) AND RCODE (4 bits) -> ignore in request
    i += 3 + 4; until += 3 + 4;
    // 1.10. QDCOUNT (16 bits)
    string *qdcount = get_next_field(&until, 16, &i, bits, sz);
    int qdcountInt = binary_to_decimal(qdcount);
    printf("TST: %d\n", qdcountInt);
    m->questions = malloc((qdcountInt + 1) * sizeof(question *));
    // 1.11. ANCOUNT (16 bits)
    string *ancount = get_next_field(&until, 16, &i, bits, sz);
    // 1.12. NSCOUNT (16 bits)
    string *nscount = get_next_field(&until, 16, &i, bits, sz);
    // 1.13. ARCOUNT (16 bits)
    string *arcount = get_next_field(&until, 16, &i, bits, sz);

    // 2. Question section
    // TODO Handle multiple questions ?
    // 2.1. QNAME (domain name)
    for (int j = 0; j < qdcountInt; ++j)
    {
        question *q = question_init();
        string *qname = parse_whole_qname(&i, &until, sz, bits);
        string_copy(&q->qname, qname);
        // 2.2. QTYPE (16 bits) AAAA = 28; A = 1; etc -> Cf https://en.wikipedia.org/wiki/List_of_DNS_record_types
        string *qtype = get_next_field(&until, 16, &i, bits, sz);
        int qtypeInt = binary_to_decimal(qtype);
        q->qtype = int_to_record_type(qtypeInt);
        // 2.3. QCLASS (16 bits) -> IN class = 1 (ignore other classes)
        string *qclass = get_next_field(&until, 16, &i, bits, sz);

        m->questions[j] = q;
        m->questions[j + 1] = NULL;

        printf("qname = %s\n", m->questions[0]->qname->arr);
        printf("qtype = %s\n", qtype->arr);
        printf("qclass = %s\n", qclass->arr);

        string_free(qname);
        string_free(qtype);
        string_free(qclass);
    }

    // TODO print delete later
    printf("ID = %s\n", m->id->arr);
    printf("QR = %s\n", qr->arr);
    printf("Opcode = %s\n", opcode->arr);
    printf("TC = %s\n", tc->arr);
    printf("RD = %s\n", rd->arr);
    printf("RA = %s\n", ra->arr);
    printf("qdcount = %s\n", qdcount->arr);
    printf("ancount = %s\n", ancount->arr);
    printf("nscount = %s\n", nscount->arr);
    printf("arcount = %s\n", arcount->arr);

    // Free memory
    string_free(arcount);
    string_free(nscount);
    string_free(qdcount);
    string_free(ancount);
    string_free(ra);
    string_free(rd);
    string_free(tc);
    string_free(opcode);
    string_free(qr);

    request *req = request_init();
    req->msg = m;
    return req;
}
