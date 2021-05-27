#ifndef BASE_CONVERTIONS_H
#define BASE_CONVERTIONS_H

#include "utils/string.h"

int binary_to_decimal(string *in);

int binary_to_decimal_unsigned(string *in);

string *decimal_to_binary(int in);

string *hexa_to_binary(string *hex);

#endif
