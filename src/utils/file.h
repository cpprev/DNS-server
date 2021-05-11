#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

#include "utils/string.h"

bool is_file(const char* path);

string *read_file(const char* path);

#endif
