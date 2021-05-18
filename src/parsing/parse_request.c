#include <string.h>
#include <stdlib.h>

#include "parsing/parse_request.h"

#include "messages/question.h"
#include "messages/message.h"
#include "messages/request.h"
#include "messages/question_array.h"

#include "utils/string.h"
#include "utils/base_convertions.h"

string *get_next_field(size_t *until, size_t step, size_t *i, string *bits)
{
    string *res = string_init();
    *until += step;
    for (; *i < *until && *i < bits->size; (*i)++)
        string_add_char(res, bits->arr[*i]);
    return res;
}

string *binary_bits_to_ascii_string(string *qname_bits)
{
    string *res = string_init();
    string *tampon = string_init();
    for (size_t i = 0; i < qname_bits->size; ++i)
    {
        if ((i > 0 && i % 8 == 0) || i == qname_bits->size - 1)
        {
            if (i == qname_bits->size - 1)
                string_add_char(tampon, qname_bits->arr[i]);
            string_add_char(res, (char)binary_to_decimal(tampon));
            string_flush(tampon);
        }
        string_add_char(tampon, qname_bits->arr[i]);
    }
    string_free(tampon);
    return res;
}

string *parse_whole_qname(size_t *i, size_t *until, string *req_bits)
{
    string *tampon = string_init(), *qname = string_init();
    int tmp_len = 0;
    size_t i_init = *i;
    for (; *i < req_bits->size; ++(*i))
    {
        if (*i > i_init && *i % 8 == 0)
        {
            int dec = binary_to_decimal(tampon);
            // 63 is max length for a label (label : test.com -> test is a label)
            if (dec > 0 && dec < 63)
            {
                (*until) += tampon->size;
                tmp_len = dec;
                string *qnamebits = get_next_field(until, tmp_len * 8, i, req_bits);
                string *tmp_name = binary_bits_to_ascii_string(qnamebits);
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
        string_add_char(tampon, req_bits->arr[*i]);
    }
    string_free(tampon);
    return qname;
}

// Cf https://datatracker.ietf.org/doc/html/rfc1035#section-4
request *parse_request(string *req_bits)
{
    message *m = message_init();
    m->questions = question_array_init();

    size_t i = 0, until = 0;

    // 1. Header section
    // 1.1. ID: 16 req_bits
    string *id = get_next_field(&until, 16, &i, req_bits);
    m->id = binary_to_decimal(id);
    // 1.2. QR (1 bit)
    string *qr = get_next_field(&until, 1, &i, req_bits);
    m->qr = binary_to_decimal(qr) == 0 ? REQUEST : RESPONSE;
    // 1.3. OPCode (4 req_bits)
    string *opcode = get_next_field(&until, 4, &i, req_bits);
    m->opcode = binary_to_decimal(opcode);
    // 1.4. AA (1 bit) -> required in responses (ignore here)
    i += 1; until += 1;
    // 1.5. TC (1 bit)
    string *tc = get_next_field(&until, 1, &i, req_bits);
    m->tc = binary_to_decimal(tc) == 1;
    // 1.6. RD (1 bit) -> optional
    string *rd = get_next_field(&until, 1, &i, req_bits);
    m->rd = binary_to_decimal(rd) == 1;
    // 1.7. RA (1 bit)
    string *ra = get_next_field(&until, 1, &i, req_bits);
    m->ra = binary_to_decimal(ra) == 1;
    // 1.8. / 1.9. Z (3 req_bits) AND RCODE (4 req_bits) -> ignore in request
    i += 3 + 4; until += 3 + 4;
    // 1.10. QDCOUNT (16 req_bits)
    string *qdcount = get_next_field(&until, 16, &i, req_bits);
    m->qdcount = binary_to_decimal(qdcount);
    // 1.11. ANCOUNT (16 req_bits)
    string *ancount = get_next_field(&until, 16, &i, req_bits);
    m->ancount = binary_to_decimal(ancount);
    // 1.12. NSCOUNT (16 req_bits)
    string *nscount = get_next_field(&until, 16, &i, req_bits);
    m->nscount = binary_to_decimal(nscount);
    // 1.13. ARCOUNT (16 req_bits)
    string *arcount = get_next_field(&until, 16, &i, req_bits);
    m->arcount = binary_to_decimal(arcount);

    // 2. Question section
    // 2.1. QNAME (domain name)
    for (int j = 0; j < m->qdcount; ++j)
    {
        question *q = question_init();
        string *qname = parse_whole_qname(&i, &until, req_bits);
        string_copy(&q->qname, qname);
        // 2.2. QTYPE (16 req_bits) AAAA = 28; A = 1; etc -> Cf https://en.wikipedia.org/wiki/List_of_DNS_record_types
        string *qtype = get_next_field(&until, 16, &i, req_bits);
        int qtypeInt = binary_to_decimal(qtype);
        q->qtype = (RECORD_TYPE) qtypeInt;
        // 2.3. QCLASS (16 req_bits) -> IN class = 1 (ignore other classes)
        string *qclass = get_next_field(&until, 16, &i, req_bits);

        printf("%ld\n", m->questions->size);
        question_array_add_question(m->questions, q);
        //m->questions->arr[j] = q;
        //m->questions->arr[j + 1] = NULL;

        // TODO print delete later
        printf("qname = %s\n", m->questions->arr[0]->qname->arr);
        printf("qtype = %s\n", qtype->arr);
        printf("qclassZ = %s\n", qclass->arr);

        string_free(qname);
        string_free(qtype);
        string_free(qclass);
    }

    // TODO print delete later
    printf("ID = %d\n", m->id);
    printf("QR = %s\n", qr->arr);
    printf("Opcode = %d\n", m->opcode);
    printf("TC = %s\n", tc->arr);
    printf("RD = %s\n", rd->arr);
    printf("RA = %s\n", ra->arr);
    printf("qdcount = %s\n", qdcount->arr);
    printf("ancount = %s\n", ancount->arr);
    printf("nscount = %s\n", nscount->arr);
    printf("arcount = %s\n", arcount->arr);

    // Free memory
    string_free(id);
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
