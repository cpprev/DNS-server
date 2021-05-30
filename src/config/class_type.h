#ifndef CLASS_TYPE_H
#define CLASS_TYPE_H

#include <stdbool.h>

#include "utils/string.h"

typedef enum
{
    CLASS_NONE = -1,
    IN = 1,
    CHAOS = 3
} CLASS_TYPE;

const char *class_type_to_string(CLASS_TYPE type);

CLASS_TYPE class_from_int(int c);

bool is_supported_class(CLASS_TYPE class);

#endif
