#include "server/wrapper.h"

wrapper wrapper_init(server_config *server_cfg, options *options)
{
    wrapper w;
    w.cfg = server_cfg;
    w.opt = options;
    return w;
}