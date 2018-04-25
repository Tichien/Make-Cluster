#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Permet d'avoir la liste des partitions disponibles de façon exploitable dans un array.
Vérifie aussi si le systeme est un cluster sous slurm.*/

int main(int argc, char *argv[])
{

	FILE *fp;
	
	char *buffer;
	size_t size = 0;
	
	fp = popen("sinfo -o '%P'", "r");
	
	if (fp == NULL) {
		printf("failed to execute command, slurm probably not present on this system");
		return(-1);
	}
	
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	
	rewind(fp);
	
	buffer = calloc( 1, size+1);
	fread(buffer, size, 1, fp);
	
	/* Compte le nombre de retours à la ligne
	pour connaitre le nombre de partitions */
	int i=0;
	char *pch=strchr(buffer, '\n');
	while (pch!=NULL) {
		i++;
		pch=strchr(pch+1,'\n');
	}
	
	/* Sépare le noms des partitions aux \n
	dans un array pour pouvoir accéder aux noms (le titre de la colonne "PARTITIONS" aussi)*/
	int j=0;
	char *p = strtok(buffer, "\n");
	char *array[i];
	
	while (p != NULL) {
		array[j++] = p;
		p = strtok(NULL, "\n");
	}
	
	/*Affiche les partitions (en omettant le titre)*/
	
	printf("slurm detected, %i available partitions:\n", i-1);
	for (j=1; j<3;++j)
		printf("%s\n", array[j]);
			
	pclose(fp);
	free(buffer);

	return 0;
}
