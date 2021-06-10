#include <arpa/inet.h>

#include "messages/response/resp_question.h"

#include "utils/base_convertions.h"

void message_question_to_bits(message *msg, void *raw, size_t *b)
{
    if (msg->questions == NULL)
        return;
    if (msg->questions->size > 0)
    {
        uint8_t *bits = raw;
        uint8_t *raw_questions = msg->raw_questions;
        for (size_t i = 0; i < msg->raw_questions_size; ++i)
            bits[(*b)++] = raw_questions[i];
        // TODO RM later
        /*for (int k = 0; msg->questions->arr[k]; ++k)
        {
            question *cur_question = msg->questions->arr[k];
            // QNAME
            string *cur_qname = cur_question->qname;
            domain_name_to_bits(cur_qname, bits, b);
            // Empty byte after QNAME
            bits[(*b)++] = 0;
            // QTYPE & QCLASS
            uint16_t *flags = (uint16_t *)((uint8_t *) raw + *b);
            flags[0] = htons(cur_question->qtype);
            *b += 2;
            flags[1] = htons(cur_question->qclass);
            *b += 2;
        }*/
    }
}
