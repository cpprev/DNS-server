#include <stdlib.h>
#include <string.h>

#include "utils/file.h"

#include "parser/options.h"

options *options_init()
{
    options *o = malloc(sizeof(options));
    o->check = false;
    o->verbose = false;
    o->file = NULL;
    return o;
}

void options_free(options *o)
{
    if (o == NULL)
        return;
    if (o->file != NULL)
        string_free(o->file);
    free(o);
}

options *parse_options(int argc, char *argv[], string **error)
{
    options *o = options_init();
    for (int i = 1; i < argc; ++i)
    {
        if (strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--check") == 0)
            o->check = true;
        else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
            o->verbose = true;
        else
        {
            o->file = string_init();
            string_add_str(o->file, argv[i]);
            if (!is_file(o->file->arr))
            {
                string_add_str(*error, "[Runtime error] Input file cannot be found.");
                options_free(o);
                return NULL;
            }
        }
    }
    if (argc == 1 || o->file == NULL)
    {
        string_add_str(*error, "[Usage] ./dns [Input file] [-t/--check] [-v/--verbose] ");
        options_free(o);
        return NULL;
    }
    return o;
}
