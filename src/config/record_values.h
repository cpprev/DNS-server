#ifndef RECORD_VALUES_H
#define RECORD_VALUES_H

#include <stdint.h>

#include "utils/string.h"

void *record_val_to_bits(RECORD_TYPE type, string *val, size_t *b);

void *A_init(string *val, size_t *b);

void *AAAA_init(string *val, size_t *b);

void *NS_init(string *val, size_t *b);

void *CNAME_init(string *val, size_t *b);

void *TXT_init(string *val, size_t *b);

void *SOA_init(string *val, size_t *b);

#endif
