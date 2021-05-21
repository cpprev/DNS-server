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
