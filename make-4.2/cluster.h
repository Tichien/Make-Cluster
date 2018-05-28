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

int find_partition(char* partition){

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
	int errcode = WEXITSTATUS(pclose(fp));
	if (errcode != 0) {
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

	strcat(partition, partitionArr[0]);

	free(buffer);

	return 0;
}

int format_cluster_subopt(const char* subopt, char* formated_subopt){

	char* found_equal = NULL;
	char opt[512] = "";
	char value[512] = "";
	char cpy_subopt[1024] = "";

	strcpy(cpy_subopt, subopt);

	found_equal = strchr(cpy_subopt, '=');

	if(found_equal != NULL)
		*found_equal = ' ';

	sscanf(cpy_subopt, "%s", opt);
	sscanf(cpy_subopt, "%*s %s", value);

	printf("opt: %s\n", opt);
	printf("value: %s\n", value);
	
	if(strcmp(opt, "p") == 0 || strcmp(opt, "partition") == 0){
		strcpy(cluster_execution_partition, value);
	}


	if(strlen(opt) == 1){
		strcpy(formated_subopt, "-");
	}
	else{
		strcpy(formated_subopt, "--");

		if(found_equal != NULL)
			strcat(opt, "=");
	}	
	
	strcat(formated_subopt, opt);
	strcat(formated_subopt, value);

	return 1;
}

int format_cluster_opts(const char* cluster_opts, char* formated_opts){

	char* token;
	char cpy_cluster_opts[1024] = "";
	
	strcpy(cpy_cluster_opts, cluster_opts);

	printf("copy: %s\n", cpy_cluster_opts);

	token = strtok(cpy_cluster_opts, ",");

	while(token != NULL){
		char subopt[1024] = "";
		
		format_cluster_subopt(token, subopt);

		strcat(formated_opts, subopt);
		strcat(formated_opts, " ");

		printf("token: %s\n", token);
		token = strtok(NULL, ",");
	}

	return 1;
}

/* Renvoie 1 si l'option du cluster est detecté est stocke les options dans cluster_opts, sinon renvoie 0 */
int get_cluster_opt(int argc, char* argv[], char* cluster_opts){

	int opt, opt_index = 0;
	int cluster_on = 0;

	/* redirection des messages d'erreurs vers "/dev/null" */
	freopen("/dev/null", "w", stderr);

	static struct option long_options[] =
    {
      {"cluster", optional_argument, 0, 'c'},
      {0, 0, 0, 0}
    };

	while ((opt = getopt_long(argc, argv, "c:", long_options, &opt_index)) != -1){
		
		switch (opt){
			case 'c':
				cluster_on = 1;
				cluster_opts = optarg;
            	printf("cluster mode: activated with option %s\n", cluster_opts);
            	break;
			/* l'option n'est pas reconnu ou sans argument*/
            case '?':
            	if(optopt == 'c'){
            		cluster_on = 1;
            		printf("cluster mode: activated\n");
            	}
            	break;
			default:
				break;
		}
	}

	return cluster_on;
}

/* Si une partition a été préciser dans les options renvoie 1 sinon, essaie de trouver une partition par default et renvoie 2, sinon renvoie 0 */
int have_partition(char* default_partition){


	if(strcmp(cluster_execution_partition, "") != 0)
		return 1;
	else{
		find_partition(default_partition);
	
		if(strcmp(default_partition, "") != 0)
			return 2;
		else
			return 0;	
	}
}