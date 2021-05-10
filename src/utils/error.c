#include <err.h>

#include "error.h"

void exit_if_true(bool condition, const char *error_message)
{
    if (condition)
        errx(1, "%s", error_message);
}
