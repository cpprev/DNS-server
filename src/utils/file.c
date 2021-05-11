#include <unistd.h>

#include "utils/string.h"
#include "utils/file.h"

bool is_file(const char* path)
{
    if (access(path, F_OK) == 0)
        return true;
    return false;
}

bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

string *read_zone_file(const char* path)
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

string *read_json_file(const char* path)
{
    string *s = string_init();
    char c;
    FILE *file = fopen(path, "r");
    if (file)
    {
        int count_quotes = 0;
        while ((c = getc(file)) != EOF)
        {
            if (count_quotes % 2 == 0 && is_whitespace(c))
                continue;
            if (c == '"')
                count_quotes++;
            string_add_char(s, c);
        }
        fclose(file);
    }
    return s;
}
