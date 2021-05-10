#include <unistd.h>

#include "utils/file.h"

bool is_file(const char* path)
{
    if (access(path, F_OK) == 0)
        return true;
    return false;
}
