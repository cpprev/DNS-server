#include <stdlib.h>

#include "messages/response.h"

#include "utils/base_convertions.h"

#include "parsing/parse_request.h"

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

    resp->msg->ra = true;
    // TODO handle authority and additional section
    resp->msg->arcount = 0;
    resp->msg->nscount = 0;

    record_array *r_arr = record_array_init();
    for (size_t i = 0; req->msg->questions->arr[i]; ++i)
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
                if (string_equals(qname, r->domain) && qtype == r->type)
                {
                    if (!resp->msg->aa)
                    {
                        //resp->msg->aa = true;
                        resp->msg->rcode = NO_ERR;
                    }
                    record_array_add_copied_record(r_arr, r);
                }

            }
        }
    }

    // TODO Copy some bits from request (like QDCOUNT, etc)

    resp->msg->id = req->msg->id;
    resp->msg->answers = r_arr;
    return resp;
}

string *response_to_bits(response *resp)
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
    string *qdcount = decimal_to_binary(resp->msg->qdcount);
    string_pad_zeroes(&qdcount, 16);
    printf("QDCOUNT: %s\n", qdcount->arr);
    string_add_str(s, qdcount->arr);
    // ANCOUNT (16 bits)
    string *ancount = decimal_to_binary(resp->msg->answers->size);
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

    // 2. Question section
    if (resp->msg->questions->size > 0)
    {
        for (int k = 0; resp->msg->questions->arr[k]; ++k)
        {
            question *cur_question = resp->msg->questions->arr[k];
            // QNAME
            string *cur_qname = cur_question->qname;
            bool hit = false;
            for (size_t i = 0; i < cur_qname->size; ++i)
            {
                char c = cur_qname->arr[i];
                int count = 0;
                if ((!hit && i == 0) || c == '.')
                {
                    if (!hit && i == 0)
                    {
                        hit = true;
                        --i;
                    }
                    size_t temp_i = i + 1;
                    while (temp_i < cur_qname->size && cur_qname->arr[temp_i] != '.')
                    {
                        count++;
                        temp_i++;
                    }
                }
                else
                    count = cur_qname->arr[i];

                string *tmp_qname = decimal_to_binary(count);
                string_pad_zeroes(&tmp_qname, 8);
                string_add_str(s, tmp_qname->arr);
                string_free(tmp_qname);
            }
            // Empty byte after QNAME
            string *question = string_init();
            string_pad_zeroes(&question, 8);
            string_add_str(s, question->arr);
            string_free(question);
            // QTYPE
            string *qtype = decimal_to_binary(cur_question->qtype);
            string_pad_zeroes(&qtype, 16);
            string_add_str(s, qtype->arr);
            string_free(qtype);
            // QCLASS
            string *qclass = string_init();
            string_add_str(qclass, "1");
            string_pad_zeroes(&qclass, 16);
            string_add_str(s, qclass->arr);
            string_free(qclass);
        }
    }

    // 3. Answer section
    for (size_t k = 0; resp->msg->answers->arr[k]; ++k)
    {
        record *r = resp->msg->answers->arr[k];
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
                count = r->domain->arr[i];
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
        // RDLENGTH (RDATA len) TODO
        string *rdlength = decimal_to_binary(4); // TODO 4 for now to work with IPv4 (A records)
        string_pad_zeroes(&rdlength, 16);
        string_add_str(s, rdlength->arr);
        printf("RDLENGTH: %s\n", rdlength->arr);
        string_free(rdlength);
        // RDATA TODO only works for A records as of now
        string *tampon = string_init();
        for (size_t i = 0; r->value->arr[i]; ++i)
        {
            char c = r->value->arr[i];
            if (c == '.' || i == r->value->size - 1)
            {
                if (i == r->value->size - 1)
                    string_add_char(tampon, c);
                string *rdata_temp = decimal_to_binary(atoi(tampon->arr));
                string_pad_zeroes(&rdata_temp, 8);
                string_add_str(s, rdata_temp->arr);
                printf("RDATA_TEMP: %s\n", rdata_temp->arr);
                string_free(rdata_temp);
                string_flush(tampon);
            }
            else
                string_add_char(tampon, c);
        }
        string_free(tampon);
    }

    // TODO rename binary_bits_to_ascii_string func better
    //string_print(s);
    string *res = binary_bits_to_ascii_string(s);

    // Free memory
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
