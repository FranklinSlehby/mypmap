#ifndef OPTIONS_H
#define OPTIONS_H

typedef struct
{
    int show_extended;
    int show_very_extended;
    int show_device;
    int quiet;
    int show_path;
    int filter_addr;
    unsigned long addr_lo, addr_hi;
    int *pids;
    int pid_count;
}
Options;

int parse_options(int argc, char *argv[], Options *opts);
void free_options(Options *opts);

#endif