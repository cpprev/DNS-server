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
                printf("%s\t", r->string_domain->arr);
                print_record_type(r->type);
                printf("\t%d\t%s\n", r->ttl, r->string_value->arr);
            }
        }
    }
}

void print_request(PROTOCOL proto, request* req)
{
    if (proto == TCP)
        printf("---\nTCP request received :\n");
    else if (proto == UDP)
        printf("---\nUDP request received :\n");
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
    printf("QR : %s | OPCODE : %s | AA : %d | TC : %d | RD : %d | RA : %d | Z : 0 | RCODE : %s\n",
           qr_to_string(msg->qr), opcode_to_string(msg->opcode), msg->aa, msg->tc, msg->rd, msg->ra, rcode_to_string(msg->rcode));
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
        printf("%s\t%d\t%s\t%s", r->string_domain->arr, r->ttl, class_type_to_string(r->class), record_type_to_string(r->type));
        if (r->string_value != NULL)
            printf("\t%s\n", r->string_value->arr);
        else
            printf("\n");
    }

    // Authority section
    printf("Authority section :\n");
    for (size_t i = 0; msg->authority && msg->authority->arr[i]; ++i)
    {
        record *r = msg->authority->arr[i];
        printf("Authority [%d] :\n", (int)i + 1);
        printf("%s\t%d\t%s\t%s", r->string_domain->arr, r->ttl, class_type_to_string(r->class), record_type_to_string(r->type));
        if (r->string_value != NULL)
            printf("\t%s\n", r->string_value->arr);
        else
            printf("\n");
    }

    // Additional section
    printf("Additional section :\n");
    for (size_t i = 0; msg->additional && msg->additional->arr[i]; ++i)
    {
        record *r = msg->additional->arr[i];
        printf("Additional [%d] :\n", (int)i + 1);
        printf("%s\t%d\t%s\t%s", r->string_domain->arr, r->ttl, class_type_to_string(r->class), record_type_to_string(r->type));
        if (r->string_value != NULL)
            printf("\t%s\n", r->string_value->arr);
        else
            printf("\n");
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
