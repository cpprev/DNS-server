#include <stdlib.h>
#include <string.h>

#include "config/record.h"

#include "utils/file.h"
#include "utils/string.h"

record *record_init()
{
    record *r = malloc(sizeof(record));

    return r;
}

void record_free(record *r)
{
    if (r == NULL)
        return;
    free(r);
}
