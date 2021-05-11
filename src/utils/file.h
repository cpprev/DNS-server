#ifndef FILE_H
#define FILE_H

#include <stdbool.h>

#include "utils/string.h"

bool is_file(const char* path);

string *read_zone_file(const char* path);

string *read_json_file(const char* path);

#endif
