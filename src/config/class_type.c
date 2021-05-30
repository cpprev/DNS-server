#include <string.h>

#include "config/class_type.h"

const char *class_type_to_string(CLASS_TYPE type)
{
    switch (type)
    {
        case IN:
            return "IN";
        case CHAOS:
            return "CHAOS";
        default:
            return "Unknown class type";;
    }
}

CLASS_TYPE class_from_int(int c)
{
    switch (c)
    {
        case 1:
            return IN;
        case 3:
            return CHAOS;
        default:
            return CLASS_NONE;
    }
}

bool is_supported_class(CLASS_TYPE class)
{
    return class == IN || class == CHAOS;
}
