#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/options.h"
#include "../include/maps_reader.h"
#include "../include/format.h"

void print_proc_header(Options *opts)
{
    char path[64];
    char name[256];
    FILE *f;

    if (opts->quiet)
        return;

    snprintf(path, sizeof(path), "/proc/%d/comm", opts->pid);
    f = fopen(path, "r");
    if (f) {
        if (fgets(name, sizeof(name), f))
            name[strcspn(name, "\n")] = '\0';
        else
            name[0] = '\0';
        fclose(f);
    } else {
        snprintf(name, sizeof(name), "???");
    }
    printf("%d:   %s\n", opts->pid, name);
}

const char *get_mapping_name(Options *opts, MapEntry *e)
{
    static char resolved[512];

    if (e->mapping[0] == '\0')
        return "";
    if (opts->show_path && e->mapping[0] == '/') {
        if (realpath(e->mapping, resolved))
            return resolved;
    }
    return e->mapping;
}

int addr_in_range(Options *opts, MapEntry *e)
{
    if (!opts->filter_addr)
        return 1;
    return (e->addr_start >= opts->addr_lo &&
            e->addr_start <= opts->addr_hi);
}