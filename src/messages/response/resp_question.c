#include "messages/response/resp_question.h"

#include "utils/base_convertions.h"

void response_question_to_bits(response *resp, string *s)
{
    if (resp->msg->questions == NULL)
        return;
    if (resp->msg->questions->size > 0)
    {
        for (int k = 0; resp->msg->questions->arr[k]; ++k)
        {
            question *cur_question = resp->msg->questions->arr[k];
            // QNAME
            string *cur_qname = cur_question->qname;
            write_domain_name_in_response(s, cur_qname);
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
}
