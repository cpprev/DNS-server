#include "utils/printer.h"

void print_server_config(server_config* cfg)
{
    printf("Server IP : %s\n", cfg->ip->arr);
    printf("Server Port : %d\n", cfg->port);
    if (cfg->zones != NULL)
    {
        for (int i = 0; cfg->zones->arr[i]; ++i)
        {
            zone *z = cfg->zones->arr[i];
            printf("---\nZone [%d] : %s\n---\n", i + 1, z->name->arr);
            for(int j = 0; z->records->arr[j]; ++j)
            {
                record *r = z->records->arr[j];
                printf("%s\t", r->domain->arr);
                print_record_type(r->type);
                printf("\t%d\t%s\n", r->ttl, r->value->arr);
            }
        }
    }
}

void print_request(request* req)
{
    printf("---\nRequest received :\n");
    print_message(req->msg);
    printf("---\n");
}

void print_response(response* resp)
{
    printf("---\nResponse :\n");
    print_message(resp->msg);
    printf("---\n");
}

void print_message(message *msg)
{
    // Header section
    printf("Header section :\n");
    printf("ID: %d\n", msg->id);
    printf("QR : %d | OPCODE : %d | AA : %d | TC : %d | RD : %d | RA : %d | Z : 0 | RCODE : %d\n", msg->qr, msg->opcode, msg->aa, msg->tc, msg->rd, msg->ra, msg->rcode);
    printf("QDCOUNT : %d\n", msg->qdcount);
    printf("ANCOUNT : %d\n", msg->ancount);
    printf("NSCOUNT : %d\n", msg->nscount);
    printf("ARCOUNT : %d\n", msg->arcount);

    // Question section
    printf("Question section :\n");
    for (size_t i = 0; msg->questions && msg->questions->arr[i]; ++i)
    {
        question *q = msg->questions->arr[i];
        printf("Question [%d] :\n", (int)i + 1);
        printf("qname = %s ; qtype = %s ; qclass = %s\n", q->qname->arr, record_type_to_string(q->qtype), class_type_to_string(q->qclass));
    }

    // Answer section
    printf("Answer section :\n");
    for (size_t i = 0; msg->answers && msg->answers->arr[i]; ++i)
    {
        record *r = msg->answers->arr[i];
        printf("Answer [%d] :\n", (int)i + 1);
        printf("%s\t%d\t%s\t%s\t%s\n", r->domain->arr, r->ttl, class_type_to_string(r->class), record_type_to_string(r->type), r->value->arr);
    }
}

void string_print(string *s)
{
    for (size_t i = 0; i < s->size; ++i)
        printf("%c", s->arr[i]);
    printf("\n");
}

void print_record_type(RECORD_TYPE type)
{
    printf("%s", record_type_to_string(type));
}
