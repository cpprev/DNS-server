#ifndef QUESTION_H
#define QUESTION_H

#include "utils/string.h"

#include "config/record_type.h"
#include "config/record.h"

typedef struct
{
    string *qname;
    RECORD_TYPE qtype;
    CLASS qclass;
} question;

question *question_init();

question *question_copy(question *q);

void question_free();

#endif
