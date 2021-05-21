#include "config/class_type.h"

const char *class_type_to_string(CLASS_TYPE type)
{
    switch (type)
    {
        case IN:
            return "IN";
        default:
            return "Unknown class type";;
    }
}
