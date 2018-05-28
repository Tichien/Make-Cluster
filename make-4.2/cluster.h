#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

/* Permet d'avoir la liste des partitions disponibles de façon exploitable dans un array.
Vérifie aussi si le systeme est un cluster sous slurm.*/

static char cluster_execution_partition[1024] = "";

/* Permet d'avoir la liste des partitions disponibles de façon exploitable dans un array.
Vérifie aussi si le systeme est un cluster sous slurm.*/

int find_partition(char* partition);

int format_cluster_subopt(const char* subopt, char* formated_subopt);

int format_cluster_opts(const char* cluster_opts, char* formated_opts);

/* Renvoie 1 si l'option du cluster est detecté est stocke les options dans cluster_opts, sinon renvoie 0 */
int get_cluster_opt(int argc, char* argv[], char* cluster_opts);

/* Si une partition a été préciser dans les options renvoie 1 sinon, essaie de trouver une partition par default et renvoie 2, sinon renvoie 0 */
int have_partition();