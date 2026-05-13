   /* procfs.h — lecture de /proc/PID/{maps,smaps,status,cmdline}
 */

#ifndef PROCFS_H
#define PROCFS_H

#include <sys/types.h>
#include <stddef.h>

#define MAX_PATH         256
#define MAX_CMDLINE     1024
#define INITIAL_CAPACITY  16

/* Une entree de /proc/PID/maps, enrichie par /proc/PID/smaps */
typedef struct {
    /* Champs maps */
    unsigned long addr_start, addr_end;
    char          perms[5];
    unsigned long offset;
    unsigned int  dev_major, dev_minor;
    unsigned long inode;
    char          pathname[MAX_PATH];

    /* Champs smaps (en kB) */
    unsigned long size_kb, rss_kb, pss_kb;
    unsigned long shared_clean_kb, shared_dirty_kb;
    unsigned long private_clean_kb, private_dirty_kb;
    unsigned long referenced_kb, anonymous_kb, swap_kb;
} MapEntry;

/* Metadonnees du processus */
typedef struct {
    pid_t pid;
    char  name[MAX_PATH];
    char  cmdline[MAX_CMDLINE];
} ProcInfo;

/* Tableau dynamique de mappings */
typedef struct {
    MapEntry *entries;
    size_t    count;
    size_t    capacity;
} MapArray;

/* API publique */
int  read_maps(pid_t pid, MapArray *maps);
int  parse_maps_line(const char *line, MapEntry *entry);
int  append_entry(MapArray *maps, const MapEntry *entry);
void free_maps(MapArray *maps);
int  read_proc_info(pid_t pid, ProcInfo *info);
int  read_smaps(pid_t pid, MapArray *maps);

#endif
