#include <stdlib.h>

#include "utils/base_convertions.h"

#include "parser/parse_request.h"

#include "build_req.h"

request *build_request()
{
    request *req = request_init();
    req->msg = message_init();

    // Header section
    req->msg->id = rand() % 65535;
    req->msg->qr = REQUEST;
    req->msg->opcode = OPCODE_QUERY;
    req->msg->aa = 0;
    req->msg->tc = 0;
    req->msg->rd = 0;
    req->msg->ra = 0;
    req->msg->rcode = 0;
    req->msg->qdcount = 1;
    req->msg->ancount = 0;
    req->msg->nscount = 0;
    req->msg->arcount = 0;

    // Question section
    req->msg->questions = question_array_init();
    question *q = question_init();
    q->qtype = A;
    q->qclass = IN;
    string *domain = string_init();
    string_add_str(domain, "example.com");
    q->qname = domain;
    question_array_add_question(req->msg->questions, q);

    return req;
}
