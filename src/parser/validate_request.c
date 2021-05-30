#include "parser/validate_request.h"
#include "parser/parse_request.h"

#include "utils/base_convertions.h"

RCODE validate_request(string *req_bits, PROTOCOL proto)
{
    RCODE res = NO_ERR;

    /// Wrong request cases :
    /// If tcp : wrong request len
    /// QR = 1 (response)
    /// OPCODE > 2
    /// QDCount > actual number of questions
    /// ANcount != 0

    size_t i = 0, until = 0;
    int qdcount = 0;
    res = validate_request_headers(req_bits, proto, &i, &until, &qdcount);

    RCODE prev_res = res;
    res = validate_request_questions(req_bits, &i, &until, qdcount);
    if (res != FORMAT_ERR)
        return prev_res;
    return res;
}

RCODE validate_request_headers(string *req_bits, PROTOCOL proto, size_t *i, size_t *until, int *qdcountint)
{
    RCODE res = NO_ERR;
    string *msg_size = NULL;
    if (proto == TCP)
    {
        msg_size = get_next_field(until, 16, i, req_bits);
    }
    string *id = get_next_field(until, 16, i, req_bits);
    string *qr = get_next_field(until, 1, i, req_bits);
    string *opcode = get_next_field(until, 4, i, req_bits);
    string *aa = get_next_field(until, 1, i, req_bits);
    string *tc = get_next_field(until, 1, i, req_bits);
    string *rd = get_next_field(until, 1, i, req_bits);
    string *ra = get_next_field(until, 1, i, req_bits);
    string *z = get_next_field(until, 3, i, req_bits);
    string *rcode = get_next_field(until, 4, i, req_bits);
    string *qdcount = get_next_field(until, 16, i, req_bits);
    *qdcountint = binary_to_decimal(qdcount);
    string *ancount = get_next_field(until, 16, i, req_bits);
    string *nscount = get_next_field(until, 16, i, req_bits);
    string *arcount = get_next_field(until, 16, i, req_bits);

    if (binary_to_decimal(opcode) > 2 || binary_to_decimal(qr) != 0 || binary_to_decimal(ancount) != 0)
        res = NOT_IMPL;

    if (msg_size && (msg_size->size != 16 || (size_t)binary_to_decimal(msg_size) != (req_bits->size / 8) - 2))
        res = FORMAT_ERR;

    if (id->size != 16 || qr->size != 1 || opcode->size != 4 || aa->size != 1 || tc->size != 1 || rd->size != 1
        || ra->size != 1 || z->size != 3 || rcode->size != 4 || qdcount->size != 16 || ancount->size != 16
        || nscount->size != 16 || arcount->size != 16)
        res = FORMAT_ERR;

    // Free memory
    string_free(z);
    string_free(rcode);
    string_free(aa);
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
    string_free(msg_size);

    return res;
}

RCODE validate_request_questions(string *req_bits, size_t *i, size_t *until, int qdcount)
{
    RCODE res = NO_ERR;
    for (int j = 0; j < qdcount; ++j)
    {
        string *qname = parse_whole_qname(i, until, req_bits);
        question *q = question_init();
        string_copy(&q->qname, qname);
        string *qtype = get_next_field(until, 16, i, req_bits);
        int qtypeInt = binary_to_decimal_unsigned(qtype);
        q->qtype = (RECORD_TYPE) qtypeInt;
        string *qclass = get_next_field(until, 16, i, req_bits);
        q->qclass = class_from_int(binary_to_decimal(qclass));

        if (!is_supported_record_type(q->qtype) || is_supported_class(q->qclass))
        {
            res = NOT_IMPL;
        }

        if (qclass->size != 16 || qtype->size != 16 || qname->size == 0)
        {
            res = FORMAT_ERR;
            string_free(qname);
            string_free(qtype);
            string_free(qclass);
            break;
        }

        string_free(qname);
        string_free(qtype);
        string_free(qclass);
    }
    return res;
}
