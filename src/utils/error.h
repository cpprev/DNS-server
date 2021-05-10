#ifndef ERROR_H
#define ERROR_H

#include <stdbool.h>

void exit_if_true(bool condition, const char *error_message);

#endif
