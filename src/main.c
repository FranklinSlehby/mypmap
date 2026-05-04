#include <stdio.h>
#include <stdlib.h>
#include "procfs.h"
#include "options.h"
#include "format.h"
int main(int argc, char *argv[])
{
    Options opts;
    int     ret = EXIT_SUCCESS;

    /* ---- Parsing des options et des PIDs ---- */
    if (parse_options(argc, argv, &opts) != 0)
        return EXIT_FAILURE;
}