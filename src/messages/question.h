#ifndef QUESTION_H
#define QUESTION_H

#include "utils/string.h"

#include "config/record_type.h"

typedef struct
{
    string *qname;
    RECORD_TYPE qtype;
} question;

question *question_init();

void question_free();

#endif
