#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "parser/parse_request.h"
#include "parser/validate_request.h"

#include "messages/question.h"
#include "messages/message.h"
#include "messages/request/request.h"
#include "messages/question_array.h"

#include "utils/string.h"
#include "utils/base_convertions.h"

// Cf https://datatracker.ietf.org/doc/html/rfc1035#section-4
request *parse_request(PROTOCOL proto, void *raw)
{
    message *m = message_init();
    m->questions = question_array_init();

    request *req = request_init();

    // TODO rework validate request with "raw" rather than "req_bits"
    /*RCODE rcode = validate_request(req_bits, proto, raw);
    if (rcode == NOT_IMPL || rcode == FORMAT_ERR)
    {
        req->msg = m;
        req->msg->rcode = rcode;
        return req;
    }*/

    size_t b = 0;
    // 1. Header section
    parse_request_headers(proto, m, raw, &b);
    // 2. Question section
    parse_request_question(m, raw, &b);
    // 3. Answer section (just skip it since it's a request)
    // TODO
    // 4. Authority section (skip it for now)
    // TODO
    // 5. Additional section
    // TODO

    req->msg = m;
    return req;
}

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

string *parse_whole_qname(void *raw, size_t *b)
{
    string *res = string_init();
    uint8_t *qn = raw;
    uint8_t cur;
    while ((cur = qn[(*b)++]) != 0)
    {
        if (cur > 0 && cur < 63 && cur != '-')
        {
            if (!string_is_empty(res))
                string_add_char(res, '.');
        }
        else
            string_add_char(res, cur);
    }
    return res;
}

void parse_request_headers(PROTOCOL proto, message *m, void *raw, size_t *b)
{
    uint16_t *bits = raw;
    if (proto == TCP)
        (*b)++;

    // ID: 16 bits
    m->id = ntohs(bits[(*b)++]);

    uint16_t fl = ntohs(bits[(*b)++]);
    // QR (1 bit)
    m->qr = fl & 0x8000 ? RESPONSE : REQUEST;
    // OPCode (4 bits)
    m->opcode = (fl & 0x7800) >> 11;
    // AA (1 bit) -> required in responses (ignore here)
    m->aa = fl & 0x0400;
    // TC (1 bit)
    m->tc = fl & 0x0200;
    // RD (1 bit) -> optional
    m->rd = fl & 0x0100;
    // RA (1 bit)
    m->ra = fl & 0x0080;
    // Z (3 req_bits) AND RCODE (4 req_bits) -> ignore in request

    // QD/AN/NS/AR COUNT (16 bits each)
    m->qdcount = ntohs(bits[(*b)++]);
    m->ancount = ntohs(bits[(*b)++]);
    m->nscount = ntohs(bits[(*b)++]);
    m->arcount = ntohs(bits[(*b)++]);

    // Since we will pass on from uint16_t arrays to uint8_t arrays next
    (*b) *= 2;
}

void parse_request_question(message *m, void *raw, size_t *b)
{
    for (int j = 0; j < m->qdcount; ++j)
    {
        // 2.1. QNAME (domain name)
        string *qname = parse_whole_qname(raw, b);
        question *q = question_init();
        string_copy(&q->qname, qname);

        uint16_t *bits = (uint16_t *)((uint8_t *)raw + *b);
        // 2.2. QTYPE (16 req_bits) AAAA = 28; A = 1; etc -> Cf https://en.wikipedia.org/wiki/List_of_DNS_record_types
        q->qtype = record_type_to_int(ntohs(bits[0]));
        *b += 2;
        // 2.3. QCLASS (16 req_bits) -> IN class = 1 (ignore other classes)
        q->qclass = ntohs(bits[1]);
        *b += 2;

        question_array_add_question(m->questions, q);

        string_free(qname);
    }
}
