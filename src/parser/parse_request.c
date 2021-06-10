#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h>

#include "parser/parse_request.h"

#include "messages/question.h"
#include "messages/message.h"
#include "messages/request/request.h"
#include "messages/question_array.h"

#include "utils/string.h"
#include "utils/base_convertions.h"

// Cf https://datatracker.ietf.org/doc/html/rfc1035#section-4
request *parse_request(PROTOCOL proto, void *raw, size_t size)
{
    message *m = message_init();
    m->questions = question_array_init();

    request *req = request_init();

    // TODO rework validate request with "raw" rather than "req_bits"

    size_t b = 0;
    // 1. Header section
    parse_request_headers(proto, m, raw, &b);
    // 2. Question section
    parse_request_question(m, raw, &b, size);
    // 3. Answer section (just skip it since it's a request)
    m->answers = parse_request_records(m->ancount, raw, &b);
    // 4. Authority section (skip it for now)
    m->authority = parse_request_records(m->nscount, raw, &b);
    // 5. Additional section
    m->additional = parse_request_records(m->arcount, raw, &b);

    req->msg = m;
    return req;
}

string *parse_whole_qname(void *raw, size_t *b, uint8_t *raw_questions, size_t *raw_questions_b)
{
    string *res = string_init();
    uint8_t *qn = raw;
    uint8_t cur;
    while ((cur = qn[(*b)++]) != 0)
    {
        if (raw_questions != NULL)
            raw_questions[(*raw_questions_b)++] = cur;
        if (cur > 0 && cur < 63 && cur != '-')
        {
            if (!string_is_empty(res))
                string_add_char(res, '.');
        }
        else
            string_add_char(res, cur);
    }
    if (raw_questions != NULL)
        raw_questions[(*raw_questions_b)++] = 0;
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

void parse_request_question(message *m, void *raw, size_t *b, size_t size)
{
    uint8_t *raw_questions = malloc((size + 1 - 96 / 8) * sizeof(uint8_t));
    size_t raw_questions_b = 0;
    for (int j = 0; j < m->qdcount; ++j)
    {
        // 2.1. QNAME (domain name)
        string *qname = parse_whole_qname(raw, b, raw_questions, &raw_questions_b);
        question *q = question_init();
        q->qname = qname;

        uint16_t *bits = (uint16_t *)((uint8_t *)raw + *b);
        // 2.2. QTYPE (16 req_bits) AAAA = 28; A = 1; etc -> Cf https://en.wikipedia.org/wiki/List_of_DNS_record_types
        q->qtype = record_type_to_int(ntohs(bits[0]));
        *b += 2;
        uint16_t *raw_questions16 = (uint16_t *)((uint8_t *)raw_questions + raw_questions_b);
        raw_questions16[0] = bits[0];
        raw_questions_b += 2;
        // 2.3. QCLASS (16 req_bits) -> IN class = 1 (ignore other classes)
        q->qclass = ntohs(bits[1]);
        *b += 2;
        raw_questions16[1] = bits[1];
        raw_questions_b += 2;

        question_array_add_question(m->questions, q);
    }
    m->raw_questions = raw_questions;
    m->raw_questions_size = raw_questions_b;
}

record_array *parse_request_records(int count, void *raw, size_t *b)
{
    record_array *res = record_array_init();
    for (int i = 0; i < count; ++i)
    {
        record *r = record_init();
        string *qname = parse_whole_qname(raw, b, NULL, NULL);
        r->string_domain = qname;
        uint16_t *cur = (uint16_t *)((uint8_t *) raw + *b);
        uint16_t type = ntohs(cur[0]);
        r->type = record_type_to_int(type);
        *b += 2;
        uint16_t class = ntohs(cur[1]);
        r->class = class;
        *b += 2;
        uint32_t *ttl_bits = (uint32_t *)((uint8_t *)raw + *b);
        uint32_t ttl = ntohl(ttl_bits[0]);
        r->ttl = ttl;
        *b += 4;
        uint16_t rdlength = htons(cur[4]);
        *b += 2;
        *b += rdlength;
    }
    return res;
}
