/* procfs.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/procfs.h"

int read_proc_info(int pid, char *nom, char *commande)
{
    char  chemin[256];
    FILE *f;
    int   i;
    int   longueur;

    sprintf(chemin, "/proc/%d/status", pid);
    f = fopen(chemin, "r");
    if (f == NULL)
    {
        printf("Erreur : le processus %d n'existe pas\n", pid);
        return (-1);
    }
    fscanf(f, "Name: %s", nom);
    fclose(f);

    sprintf(chemin, "/proc/%d/cmdline", pid);
    f = fopen(chemin, "r");
    if (f == NULL)
        return (-1);
    longueur = fread(commande, 1, 255, f);
    for (i = 0; i < longueur; i++)
        if (commande[i] == '\0')
            commande[i] = ' ';
    commande[longueur] = '\0';
    fclose(f);
    return (0);
}

int parse_maps_line(char *ligne, MapEntry *entree)
{
    int resultat;

    entree->nom[0] = '\0';
    resultat = sscanf(ligne,
        "%lx-%lx %5s %lx %11s %lu %255s",
        &entree->debut,
        &entree->fin,
        entree->perms,
        &entree->offset,
        entree->device,
        &entree->inode,
        entree->nom);
    if (resultat < 6)
        return (-1);
    entree->taille_kb = (entree->fin - entree->debut) / 1024;
    entree->rss   = 0;
    entree->dirty = 0;
    return (0);
}

int append_entry(MapTable *table, MapEntry *entree)
{
    MapEntry *nouveau;

    if (table->nombre >= table->capacite)
    {
        table->capacite = table->capacite * 2;
        nouveau = realloc(table->entrees,
                  table->capacite * sizeof(MapEntry));
        if (nouveau == NULL)
        {
            printf("Erreur : plus de memoire\n");
            return (-1);
        }
        table->entrees = nouveau;
    }
    table->entrees[table->nombre] = *entree;
    table->nombre++;
    return (0);
}

int read_maps(int pid, MapTable *table)
{
    char     chemin[256];
    FILE    *f;
    char     ligne[512];
    MapEntry entree;

    table->nombre   = 0;
    table->capacite = 64;
    table->entrees  = malloc(64 * sizeof(MapEntry));
    if (table->entrees == NULL)
        return (-1);
    sprintf(chemin, "/proc/%d/maps", pid);
    f = fopen(chemin, "r");
    if (f == NULL)
    {
        printf("Erreur : impossible de lire /proc/%d/maps\n", pid);
        free(table->entrees);
        return (-1);
    }
    while (fgets(ligne, sizeof(ligne), f) != NULL)
    {
        if (parse_maps_line(ligne, &entree) == 0)
        {
            if (append_entry(table, &entree) == -1)
            {
                fclose(f);
                return (-1);
            }
        }
    }
    fclose(f);
    return (0);
}

void free_table(MapTable *table)
{
    free(table->entrees);
    table->entrees  = NULL;
    table->nombre   = 0;
    table->capacite = 0;
}
