#include <stdlib.h>

#include "messages/response/response.h"
#include "messages/response/resp_headers.h"
#include "messages/response/resp_question.h"
#include "messages/response/resp_answer.h"

#include "config/record.h"

#include "utils/string.h"
#include "utils/base_convertions.h"

#include "parser/parse_request.h"

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
    resp->msg = message_copy(req->msg);

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
                if (string_equals(qname, r->domain))
                {
                    hit_domain = true;
                    if (qtype == r->type || qtype == ANY)
                        record_array_add_copied_record(r_arr, r);
                }
            }
        }
    }
    if (resp->msg->rcode == NO_ERR)
        resp->msg->rcode = hit_domain ? NO_ERR : NXDOMAIN;

    resp->msg->answers = r_arr;
    resp->msg->ancount = resp->msg->answers->size;

    return resp;
}

void write_domain_name_in_response(string *s, string *cur)
{
    bool hit = false;
    for (size_t i = 0; i < cur->size; ++i)
    {
        char c = cur->arr[i];
        int count = 0;
        if ((!hit && i == 0) || c == '.')
        {
            if (!hit && i == 0)
            {
                hit = true;
                --i;
            }
            size_t temp_i = i + 1;
            while (temp_i < cur->size && cur->arr[temp_i] != '.')
            {
                count++;
                temp_i++;
            }
        }
        else
            count = cur->arr[i];

        string *tmp_qname = decimal_to_binary(count);
        string_pad_zeroes(&tmp_qname, 8);
        string_add_str(s, tmp_qname->arr);
        string_free(tmp_qname);
    }
}

string *response_to_bits(PROTOCOL proto, response *resp)
{
    string *s = string_init();

    // 1. Header section
    response_headers_to_bits(resp, s);
    // 2. Question section
    response_question_to_bits(resp, s);
    // 3. Answer section
    response_answer_to_bits(resp, s);

    string *res = NULL;
    if (proto == TCP)
    {
        string *sizeString = decimal_to_binary(s->size / 8);
        string_pad_zeroes(&sizeString, 16);
        string_add_str(sizeString, s->arr);
        res = binary_bits_to_ascii_string(sizeString);
        string_free(sizeString);
    }
    else if (proto == UDP)
    {
        res = binary_bits_to_ascii_string(s);
    }
    string_free(s);
    return res;
}
