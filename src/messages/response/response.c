#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "messages/response/response.h"
#include "messages/response/resp_headers.h"
#include "messages/response/resp_question.h"
#include "messages/response/resp_answer.h"

#include "config/record.h"

#include "utils/string.h"
#include "utils/base_convertions.h"

#include "parser/parse_request.h"

#include "server/udp_listen.h"

response *response_init()
{
    response *r = malloc(sizeof(response));
    r->msg = NULL;
    return r;
}

void response_free(response *r)
{
    if (r == NULL)
        return;
    if (r->msg != NULL)
        message_free(r->msg);
    free(r);
}

response *build_response(server_config *cfg, request *req)
{
    // TODO Handle authority and additional sections in answer_array

    response *resp = response_init();
    resp->msg = req->msg;

    resp->msg->id = req->msg->id;

    resp->msg->qr = RESPONSE;

    resp->msg->ra = true;

    // TODO handle authority and additional section

    resp->msg->tc = false;
    resp->msg->arcount = 0;
    resp->msg->nscount = 0;

    // Handle weird requests here
    if (req->msg->rcode == NOT_IMPL || req->msg->rcode == FORMAT_ERR)
    {
        // TODO flush questions/answers ?
        // Since we copied the pointer to resp->msg, we don't want to free it twice later on
        req->msg = NULL;
        return resp;
    }

    record_array *r_arr = record_array_init();
    bool hit_domain = false;
    for (size_t i = 0; req->msg->questions && req->msg->questions->arr[i]; ++i)
    {
        question *q = req->msg->questions->arr[i];
        string *qname = q->qname;
        RECORD_TYPE qtype = q->qtype;
        for (size_t j = 0; cfg->zones->arr[j]; ++j)
        {
            zone *z = cfg->zones->arr[j];
            for (size_t k = 0; z->records->arr[k]; ++k)
            {
                record *r = z->records->arr[k];
                if (string_equals(qname, r->string_domain))
                {
                    hit_domain = true;
                    if (qtype == r->type || qtype == ANY)
                    {
                        resp->msg->aa = true;
                        record_array_add_copied_record(r_arr, r);
                    }
                }
            }
        }
    }
    if (resp->msg->rcode == NO_ERR)
        resp->msg->rcode = hit_domain ? NO_ERR : NXDOMAIN;

    resp->msg->answers = r_arr;
    resp->msg->ancount = resp->msg->answers->size;

    // Since we copied the pointer to resp->msg, we don't want to free it twice later on
    req->msg = NULL;
    return resp;
}

void domain_name_to_bits(string *qname, void *raw, size_t *b)
{
    uint8_t *bits = raw;
    bool hit = false;
    for (size_t i = 0; i < qname->size; ++i)
    {
        char c = qname->arr[i];
        uint8_t count = 0;
        if ((!hit && i == 0) || c == '.')
        {
            if (!hit && i == 0)
            {
                hit = true;
                --i;
            }
            size_t temp_i = i + 1;
            while (temp_i < qname->size && qname->arr[temp_i] != '.')
            {
                count++;
                temp_i++;
            }
        }
        else
            count = qname->arr[i];

        bits[(*b)++] = count;
    }
}

void message_to_bits(PROTOCOL proto, message *msg, void **bits, size_t *b)
{
    if (proto == UDP)
        *bits = malloc(UDP_MESSAGE_SIZE + 1);
    else
        *bits = malloc(32768);

    *b = proto == UDP ? 0 : 1;

    // 1. Header section
    message_headers_to_bits(msg, *bits, b);
    // 2. Question section
    message_question_to_bits(msg, *bits, b);
    // 3. Answer section
    message_answer_to_bits(msg, *bits, b);
    // TODO 4. Authority section
    // TODO 5. Additional section

    uint16_t *bits16 = *bits;
    if (proto == TCP)
        bits16[0] = htons(*b - 2);
}
