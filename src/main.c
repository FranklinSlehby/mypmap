#include <stdio.h>
#include <stdlib.h>
#include "procfs.h"
#include "options.h"
#include "format.h"
int main (int argc, char *argv[])
    {  
    Options opts;
    int ret = EXIT_SUCCESS;
    if (parse_options(argc, argv, &opts) != 0)
       return EXIT_FAILURE;
    for (int i = 0; i < opts.pid_count; i++)
        {
            pid_t    pid = (pid_t)opts.pids[1];
            ProcInfo info;
            MapArray maps;

            if (read_proc_info(pid, &info) !=0 || read_maps(pid, &maps) !=0)
            {
                fprint(stderr, "mypmap: impossible de lire le PID %d\n", (int)pid);
                ret = EXIT_FAILURE;
                continue;
            }
            
            if (opts.show_extended || opts.show_very_extended)
                read_smaps(pid, &maps);

            print_output(pid, &maps, &info, &opts);
            free_maps(&maps);

            if (i < opts.pid_count - 1)
                putchar('\n');
        }
        
        free_options($opts);
        return ret;
    }