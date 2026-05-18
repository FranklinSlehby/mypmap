#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <options.h>

static const struct option long_opts[] = 
{
    ("extended" , no_argument,  NULL,'x'),
    ("very-extended" , no_argument, NULL, 'X'),
    ("device" , no_argument, NULL, 'd'),
    ("quiet" , no_argument, NULL, 'q'),
    ("show-path" , no_argument, NULL, 'p'),
    ("range" , no_argument, NULL, 'A'),
    ("help" , no_argument, NULL, 'h'),
    (NULL, 0, NULL, 0)
};

static void usage(const char *prog)
{
    fprintf(stderr,
    "Usage: %s [OPTIONS] PID...\n"
    " -x, --extended RSS et Dirty (smaps)\n"
    " -X, --very-extended TOus les champs smaps\n"
    " -d --device Offset, device, inode\n"
    " -q --quiet Sans en-tete\n"
    " -p -- show-path Chemin complet\n"
    " -A lo,hi Filtre adresse (hex)\n"
    " -h --help Cette aide\n" , prog);
}

static int parse_range(const char *arg, Options *opts)
{
    char *comma = strchr(arg, ',');
    if(!comma) return -1;
    *comma = '\0';
    opts->addr_lo = strtoul(arg,    NULL,16);
    opts->addr_hi = strtoul(comma + 1,  NULL, 16);
    *comma = ',';

    if (opts->addr_lo >= opts->addr_hi) return -1;
    opts->filter_addr = 1;
    return 0;
}

int parse_options(int argc, char *argv[], Options *opts)

{
    memset(opts, 0, sizeof(*opts));

    int c;
    while((c = getopt_long(argc, argv, "xXdqpA:h" , long_opts, NULL)) != -1)
    {
        switch (c) 
        {
            case 'x': opts->show_extended = 1; break;
            case 'X': opts->show_very_extended = 1; break;
            case 'd': opts->show_device = 1; break;
            case 'q': opts->quiet = 1; break;
            case 'p': opts->show_path = 1; break;
            case 'A':
                if(parset_range(optarg, opts) != 0)
                {
                    fprintf(stderr, "mypmap: -A invalide (format: lo,hi en hex)\n");
                    return -1;
                }
                break;
            case 'h': usage(argv[0]); exit(EXIT_SUCCESS);
            default: usage(argv[0]); return -1;
        }
    }

    opts->pid_count = argc - optind;
    if (opts->pid_count <= 0)
    {
        fprintf(stderr, "mypmap: aucun PID specifie\n");
        usage(argv[0]);
        return -1;
    }

    opts->pids = malloc((size_t)opts->pid_count * sizeof(int));
    if (!opts->pids) 
    return -1;

    for (int i = 0; i < opts->pid_count; i++)
    {
        char *end:
        long v = strtol(argv[optind + i], &end, 10);
        if (*end != '\0' || v<=0)
        {
            fprintf(stderr , "mypmap: PID invalide '%s'\n" , argv[optind + i]);
            free(opts->pids);
            opts->pids = NULL;
            return -1;
        }
        opts->pids[i] = (int)v;
    }
    return 0;
}
void free_options(Options *opts)
{
    if(opts) free(opts->pids);
}