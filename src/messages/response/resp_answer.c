#include <stdio.h>
#include <stdlib.h>

#include "messages/response/resp_answer.h"

#include "utils/base_convertions.h"

void response_answer_to_bits(response *resp, string *s)
{
    for (size_t k = 0; resp->msg->answers->arr[k]; ++k)
    {
        record *r = resp->msg->answers->arr[k];
        // NAME
        write_domain_name_in_response(s, r->domain);
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
        int rdlenInt = 0;
        // SOA vals
        string *mname = string_init(), *rname = string_init(), *serial = string_init(), *refresh = string_init(),
                *retry = string_init(), *expire = string_init(), *minimum = string_init();
        switch (r->type)
        {
            case A:
                rdlenInt = 4;
                break;
            case AAAA:
                rdlenInt = 16;
                break;
            case TXT:
                // "+ 1" : for the single length octet required before the TXT string
                rdlenInt = r->value->size + 1;
                break;
            case CNAME:
                // Length of string : <len_octet1>label1<len_octet2>label2,...,<null_octet>
                rdlenInt = r->value->size + 1;
                break;
            case SOA:
                // Length of : MNAME + RNAME + SERIAL + REFRESH + RETRY + EXPIRE + MINIMUM
                get_soa_values(r->value, &mname, &rname, &serial, &refresh, &retry, &expire, &minimum);
                rdlenInt = (mname->size + 1) + (rname->size + 1) + 4 + 4 + 4 + 4 + 4;
                break;
            case NS:
                // Length of string : <len_octet1>label1<len_octet2>label2,...,<null_octet>
                rdlenInt = r->value->size + 1;
                break;
            default:
                break;
        }
        string *rdlength = decimal_to_binary(rdlenInt);
        string_pad_zeroes(&rdlength, 16);
        string_add_str(s, rdlength->arr);
        printf("RDLENGTH: %s\n", rdlength->arr);
        string_free(rdlength);
        if (r->type == A)
        {
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
        else if (r->type == AAAA)
        {
            string *tampon = string_init();
            for (size_t i = 0; r->value->arr[i]; ++i)
            {
                char c = r->value->arr[i];
                if (c == ':' || i == r->value->size - 1)
                {
                    if (i == r->value->size - 1)
                        string_add_char(tampon, c);
                    string *rdata_temp = hexa_to_binary(tampon);
                    printf("HEX: %s\n", rdata_temp->arr);
                    string_pad_zeroes(&rdata_temp, 16);
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
        else if (r->type == CNAME)
        {
            write_domain_name_in_response(s, r->value);
        }
        else if (r->type == TXT)
        {
            // Single length octet (this is equals to length of string, where as
            // RDLENGTH is equals to length of string + 1)
            rdlength = decimal_to_binary(r->value->size);
            string_pad_zeroes(&rdlength, 8);
            string_add_str(s, rdlength->arr);
            string_free(rdlength);

            for (size_t i = 0; r->value->arr[i]; ++i)
            {
                string *temp = decimal_to_binary(r->value->arr[i]);
                string_pad_zeroes(&temp, 8);
                string_add_str(s, temp->arr);
                string_free(temp);
            }
        }
        else if (r->type == SOA)
        {
            write_domain_name_in_response(s, mname);

            write_domain_name_in_response(s, rname);

            string *serialInt = decimal_to_binary(atoi(serial->arr));
            string_pad_zeroes(&serialInt, 32);
            string_add_str(s, serialInt->arr);
            string_free(serialInt);

            string *refreshInt = decimal_to_binary(atoi(refresh->arr));
            string_pad_zeroes(&refreshInt, 32);
            string_add_str(s, refreshInt->arr);
            string_free(refreshInt);

            string *retryInt = decimal_to_binary(atoi(retry->arr));
            string_pad_zeroes(&retryInt, 32);
            string_add_str(s, retryInt->arr);
            string_free(retryInt);

            string *expireInt = decimal_to_binary(atoi(expire->arr));
            string_pad_zeroes(&expireInt, 32);
            string_add_str(s, expireInt->arr);
            string_free(expireInt);

            string *minimumInt = decimal_to_binary(atoi(minimum->arr));
            string_pad_zeroes(&minimumInt, 32);
            string_add_str(s, minimumInt->arr);
            string_free(minimumInt);
        }
        else if (r->type == NS)
        {
            write_domain_name_in_response(s, r->value);
        }

        // SOA vals free
        string_free(mname);
        string_free(rname);
        string_free(serial);
        string_free(refresh);
        string_free(retry);
        string_free(expire);
        string_free(minimum);
    }
}
