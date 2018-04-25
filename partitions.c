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

	/*Redirection de stderr sur stdout pour éviter l'affichage d'éventuels messages d'erreur*/
	fp = popen("sinfo -o '%P' 2>&1", "r");

	/*lecture du résultat de la commande dans buffer*/
	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);

	rewind(fp);

	buffer = calloc( 1, size+1);
	fread(buffer, size, 1, fp);

	/*ne renvoie 0 que lorsqu'il n'y a eu aucune erreur*/
	if (WEXITSTATUS(pclose(fp) != 0)) {
		printf("slurm probably not present on this system\n");
		return(-1);
	}
	else {
		printf("Slurm detected\n");
	}

	/* Compte le nombre de retours à la ligne
	pour connaitre le nombre de partitions */
	int i=0;
	char *pch=strchr(buffer, '\n');
	while (pch!=NULL) {
		i++;
		pch=strchr(pch+1,'\n');
	}

	/* Sépare le noms des partitions aux \n
	dans un array pour pouvoir accéder aux noms*/
	int j=0;
	char *p = strtok(buffer, "\n");
	char *array[i];

	while (p != NULL) {
		array[j++] = p;
		p = strtok(NULL, "\n");
	}
	char **partitionArr = array + 1;

	/*Affiche les partitions*/

	printf("%i available partitions:\n", i-1);
	for (j=0; j<i-1;++j)
		printf("%s\n", partitionArr[j]);

	free(buffer);

	return 0;
}
