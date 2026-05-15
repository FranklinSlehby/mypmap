/* procfs.h */

#ifndef PROCFS_H
#define PROCFS_H

/*
** MapEntry = une zone memoire du processus
** Quand tu tapes "cat /proc/425/maps" tu vois des lignes comme :
** 5f0a42cbe000-5f0a42cee000 r--p 00000000 08:20 123456 /bin/bash
** Chaque ligne devient une MapEntry
*/
typedef struct
{
    unsigned long debut;        /* adresse de debut  : 5f0a42cbe000 */
    unsigned long fin;          /* adresse de fin    : 5f0a42cee000 */
    char          perms[6];     /* permissions       : r--p          */
    unsigned long offset;       /* offset            : 00000000      */
    char          device[12];   /* peripherique      : 08:20         */
    unsigned long inode;        /* numero inode      : 123456        */
    char          nom[256];     /* nom du fichier    : /bin/bash     */
    unsigned long taille_kb;    /* taille en kB      : 192           */
    unsigned long rss;          /* memoire reelle kB : pour -x       */
    unsigned long dirty;        /* memoire modifiee  : pour -x       */
} MapEntry;

/*
** MapTable = un tableau qui contient toutes les MapEntry
** Il grandit automatiquement quand on ajoute des entrees
*/
typedef struct
{
    MapEntry *entrees;   /* le tableau de zones memoire */
    int       nombre;    /* combien d'entrees on a      */
    int       capacite;  /* taille maximale du tableau  */
} MapTable;

/* Les fonctions qu'on va ecrire dans procfs.c */
int  read_proc_info(int pid, char *nom, char *commande);
int  read_maps(int pid, MapTable *table);
int  parse_maps_line(char *ligne, MapEntry *entree);
int  append_entry(MapTable *table, MapEntry *entree);
void free_table(MapTable *table);

#endif
