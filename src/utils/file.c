#include <unistd.h>

#include "utils/string.h"
#include "utils/file.h"

bool is_file(const char* path)
{
    if (access(path, F_OK) == 0)
        return true;
    return false;
}

string *read_file(const char* path)
{
    string *s = string_init();
    char c;
    FILE *file = fopen(path, "r");
    if (file)
    {
        while ((c = getc(file)) != EOF)
        {
            string_add_char(s, c);
        }
        fclose(file);
    }
    return s;
}
