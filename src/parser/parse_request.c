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
    RCODE rcode = NO_ERR;
    // 1. Header section
    parse_request_headers(proto, m, raw, &b, size, &rcode);
    if (rcode == FORMAT_ERR) goto exit;
    // 2. Question section
    parse_request_question(m, raw, &b, size, &rcode);
    if (rcode == FORMAT_ERR) goto exit;
    // 3. Answer section (just skip it since it's a request)
    m->answers = parse_request_records(m->ancount, raw, &b, size, &rcode);
    if (rcode == FORMAT_ERR) goto exit;
    // 4. Authority section (skip it for now)
    m->authority = parse_request_records(m->nscount, raw, &b, size, &rcode);
    if (rcode == FORMAT_ERR) goto exit;
    // 5. Additional section
    m->additional = parse_request_records(m->arcount, raw, &b, size, &rcode);
    if (rcode == NOT_IMPL || rcode == FORMAT_ERR) goto exit;

    req->msg = m;
    return req;

exit:
    req->msg = m;
    req->msg->rcode = rcode;
    return req;
}

string *parse_whole_qname(void *raw, size_t *b, size_t size, uint8_t *raw_questions, size_t *raw_questions_b, RCODE *rcode)
{
    if (*b + 1 >= size)
        goto format_err;
    string *res = string_init();
    uint8_t *qn = raw;
    uint8_t cur;
    int temp_count = 0;
    while ((cur = qn[(*b)++]) != 0)
    {
        if (raw_questions != NULL)
            raw_questions[(*raw_questions_b)++] = cur;
        if (cur > 0 && cur < 63 && cur != '-')
        {
            if (!string_is_empty(res))
                string_add_char(res, '.');
            temp_count = cur;
        }
        else
        {
            string_add_char(res, cur);
            temp_count--;
        }
        if (*b + 1 >= size || temp_count < 0)
        {
            string_free(res);
            goto format_err;
        }
    }
    if (raw_questions != NULL)
        raw_questions[(*raw_questions_b)++] = 0;
    return res;

format_err:
    *rcode = FORMAT_ERR;
    return NULL;
}

void parse_request_headers(PROTOCOL proto, message *m, void *raw, size_t *b, size_t size, RCODE *rcode)
{
    if ((proto == UDP && size < UDP_HEADER_SIZE) || (proto == TCP && size < TCP_HEADER_SIZE))
        goto format_err;

    uint16_t *bits = raw;
    uint16_t msg_size = 0;
    if (proto == TCP)
        msg_size = ntohs(bits[(*b)++]);

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

    if (!is_opcode_valid(m->opcode) || m->qr != REQUEST || m->ancount != 0)
        goto not_impl;
    if (proto == TCP && msg_size != size - 2)
        goto format_err;
    return;

not_impl:
    *rcode = NOT_IMPL;
    return;
format_err:
    *rcode = FORMAT_ERR;
    return;
}

void parse_request_question(message *m, void *raw, size_t *b, size_t size, RCODE *rcode)
{
    uint8_t *raw_questions = malloc((size + 1 - 96 / 8) * sizeof(uint8_t));
    size_t raw_questions_b = 0;
    for (int j = 0; j < m->qdcount; ++j)
    {
        // 2.1. QNAME (domain name)
        string *qname = parse_whole_qname(raw, b, size, raw_questions, &raw_questions_b, rcode);
        if (rcode != NO_ERR)
        {
            free(raw_questions);
            return;
        }

        question *q = question_init();
        q->qname = qname;

        if (*b + 4 >= size)
        {
            *rcode = FORMAT_ERR;
            question_free(q);
            free(raw_questions);
            return;
        }

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

        if (!is_supported_record_type(q->qtype) || !is_supported_class(q->qclass))
        {
            *rcode = NOT_IMPL;
            question_free(q);
            free(raw_questions);
            return;
        }

        question_array_add_question(m->questions, q);
    }
    m->raw_questions = raw_questions;
    m->raw_questions_size = raw_questions_b;
}

record_array *parse_request_records(int count, void *raw, size_t *b, size_t size, RCODE *rcode)
{
    record_array *res = record_array_init();
    for (int i = 0; i < count; ++i)
    {
        record *r = record_init();
        string *qname = parse_whole_qname(raw, b, size, NULL, NULL, rcode);
        if (rcode != NO_ERR)
        {
            record_array_free(res);
            record_free(r);
            return NULL;
        }
        r->string_domain = qname;

        if (*b + 10 >= size)
        {
            record_array_free(res);
            record_free(r);
            goto format_err;
        }

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

        if (*b + rdlength >= size)
        {
            record_array_free(res);
            record_free(r);
            goto format_err;
        }
        *b += rdlength;
    }
    return res;

format_err:
    *rcode = FORMAT_ERR;
    return NULL;
}
