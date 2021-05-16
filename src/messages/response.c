#include <stdlib.h>

#include "messages/response.h"

#include "utils/base_convertions.h"

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
    // TODO Handle authority and additional sections in answer

    response *resp = response_init();
    resp->msg = message_init();
    record_array *r_arr = record_array_init();
    for (size_t i = 0; req->msg->questions[i] != NULL; ++i)
    {
        question *q = req->msg->questions[i];
        string *qname = q->qname;
        RECORD_TYPE qtype = q->qtype;
        for (size_t j = 0; cfg->zones->arr[j]; ++j)
        {
            zone *z = cfg->zones->arr[j];
            for (size_t k = 0; z->records->arr[k]; ++k)
            {
                record *r = z->records->arr[k];
                if (string_equals(qname, r->domain) && qtype == r->type)
                {
                    if (!resp->msg->aa)
                    {
                        resp->msg->aa = true;
                        resp->msg->rcode = NO_ERR;
                    }
                    record_array_add_copied_record(r_arr, r);
                }

            }
        }
    }
    resp->msg->id = req->msg->id;
    resp->msg->answer = answer_init();
    resp->msg->answer->records = r_arr;
    return resp;
}

int *response_to_bits(response *resp)
{
    string *s = string_init();
    // 1. Header section
    // Id (16 bits)
    string *id = decimal_to_binary(resp->msg->id);
    printf("ID: %s\n", id->arr);
    string_pad_zeroes(&id, 16);
    string_add_str(s, id->arr);
    // QR (1 bit)
    string *qr = decimal_to_binary(RESPONSE);
    string_pad_zeroes(&qr, 1);
    printf("QR: %s\n", qr->arr);
    string_add_str(s, qr->arr);
    // OPCode (4 bits)
    string *opcode = decimal_to_binary(resp->msg->opcode);
    string_pad_zeroes(&opcode, 4);
    printf("OPCODE: %s\n", opcode->arr);
    string_add_str(s, opcode->arr);
    // AA (1 bit) -> required in responses (ignore here)
    string *aa = decimal_to_binary(resp->msg->aa);
    string_pad_zeroes(&aa, 1);
    printf("AA: %s\n", aa->arr);
    string_add_str(s, aa->arr);
    // TC (1 bit)
    string *tc = decimal_to_binary(resp->msg->tc);
    string_pad_zeroes(&tc, 1);
    printf("TC: %s\n", tc->arr);
    string_add_str(s, tc->arr);
    // RD (1 bit)
    string *rd = decimal_to_binary(resp->msg->rd);
    string_pad_zeroes(&rd, 1);
    printf("RD: %s\n", rd->arr);
    string_add_str(s, rd->arr);
    // RA (1 bit)
    string *ra = decimal_to_binary(resp->msg->ra);
    string_pad_zeroes(&ra, 1);
    printf("RA: %s\n", ra->arr);
    string_add_str(s, ra->arr);
    // Z (3 bits)
    string *z = string_init();
    string_pad_zeroes(&z, 3);
    printf("Z: %s\n", z->arr);
    string_add_str(s, z->arr);
    // RCODE (4 bits) TODO cases other than NO_ERR
    string *rcode = decimal_to_binary(resp->msg->rcode);
    string_pad_zeroes(&rcode, 4);
    printf("RCODE: %s\n", rcode->arr);
    string_add_str(s, rcode->arr);
    // QDCOUNT (16 bits)
    string *qdcount = string_init();
    string_pad_zeroes(&qdcount, 16);
    printf("QDCOUNT: %s\n", qdcount->arr);
    string_add_str(s, qdcount->arr);
    // ANCOUNT (16 bits)
    string *ancount = decimal_to_binary(resp->msg->answer->records->size);
    string_pad_zeroes(&ancount, 16);
    printf("ANCOUNT: %s\n", ancount->arr);
    string_add_str(s, ancount->arr);
    // NSCOUNT (16 bits) TODO if we have to handle Authority section in response
    string *nscount = decimal_to_binary(resp->msg->nscount);
    string_pad_zeroes(&nscount, 16);
    printf("NSCOUNT: %s\n", nscount->arr);
    string_add_str(s, nscount->arr);
    // ARCOUNT (16 bits) TODO if we have to handle Additional section in response
    string *arcount = decimal_to_binary(resp->msg->arcount);
    string_pad_zeroes(&arcount, 16);
    printf("ARCOUNT: %s\n", arcount->arr);
    string_add_str(s, arcount->arr);

    // 2. Question section (only 8 zeroes ?) TODO Figure out
    string *question = string_init();
    string_pad_zeroes(&question, 8);
    string_add_str(s, question->arr);
    // QTYPE ? TODO
    string *qtype = string_init();
    string_pad_zeroes(&qtype, 16);
    string_add_str(s, qtype->arr);
    // QCLASS ? TODO
    string *qclass = string_init();
    string_pad_zeroes(&qclass, 16);
    string_add_str(s, qclass->arr);

    // 3. Answer section
    for (size_t k = 0; resp->msg->answer->records->arr[k]; ++k)
    {
        record *r = resp->msg->answer->records->arr[k];
        // NAME
        bool hit = false;
        for (size_t i = 0; i < r->domain->size; ++i)
        {
            char c = r->domain->arr[i];
            int count = 0;
            if ((!hit && i == 0) || c == '.')
            {
                if (!hit && i == 0)
                {
                    hit = true;
                    --i;
                }
                size_t temp_i = i + 1;
                while (temp_i < r->domain->size && r->domain->arr[temp_i] != '.')
                {
                    count++;
                    temp_i++;
                }
            }
            else
            {
                count = r->domain->arr[i];
            }
            string *cur_qname = decimal_to_binary(count);
            string_pad_zeroes(&cur_qname, 8);
            string_add_str(s, cur_qname->arr);
            string_free(cur_qname);
        }
        // Null byte after name
        string *nb = string_init();
        string_pad_zeroes(&nb, 8);
        string_add_str(s, nb->arr);
        string_free(nb);
        // TYPE
        string *rtype = decimal_to_binary(r->type);
        string_pad_zeroes(&rtype, 16);
        string_add_str(s, rtype->arr);
        printf("TYPE: %s\n", rtype->arr);
        string_free(rtype);
        // CLASS
        string *class = decimal_to_binary(r->class);
        string_pad_zeroes(&class, 16);
        string_add_str(s, class->arr);
        printf("CLASS: %s\n", class->arr);
        string_free(class);
        // TTL
        string *ttl = decimal_to_binary(r->ttl);
        string_pad_zeroes(&ttl, 32);
        string_add_str(s, ttl->arr);
        printf("TTL: %s\n", ttl->arr);
        string_free(ttl);
        // RDLENGTH (RDATA len)

        // RDATA
    }

    int *res = malloc((s->size + 1) * sizeof(int));
    for (size_t i = 0; i < s->size; ++i)
        res[i] = s->arr[i] - '0';
    res[s->size] = -1;

    // Free memory
    string_free(qtype);
    string_free(qclass);
    string_free(question);
    string_free(nscount);
    string_free(arcount);
    string_free(qdcount);
    string_free(ancount);
    string_free(z);
    string_free(rcode);
    string_free(rd);
    string_free(ra);
    string_free(aa);
    string_free(tc);
    string_free(opcode);
    string_free(qr);
    string_free(id);
    string_free(s);
    return res;
}
