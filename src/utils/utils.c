#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "utils/utils.h"

void sigint_handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("\nExited\n");
        exit(0);
    }
}

