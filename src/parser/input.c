#include <string.h>

#include "parser/input.h"
#include "parser/input_state.h"

#include "utils/file.h"

server_config *parse_server_config(const char *path)
{
    string *in = read_file(path);
    server_config *cfg = NULL;
    string *dummy = string_init();
    INPUT_STATE state = NONE;
    size_t zone_ind = -1;
    for (size_t i = 0; i < in->size; ++i)
    {
        char c = in->arr[i];
        if (c == '\n' || i == in->size - 1)
        {
            if (c != '\n' && i == in->size - 1)
                string_add_char(dummy, c);

            if (strcmp("[server]", dummy->arr) == 0)
                state = SERVER;
            else if (strcmp("[zone]", dummy->arr) == 0)
            {
                state = ZONE;
                ++zone_ind;
            }
            else
                server_config_set_attribute(&cfg, dummy, state, zone_ind);
            string_flush(dummy);
        }
        else
            string_add_char(dummy, c);
    }
    string_free(in);
    string_free(dummy);
    return cfg;
}
