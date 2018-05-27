#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>


/*Permet d'avoir la liste des partitions disponibles de façon exploitable dans un array.
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

	char* found_ch = NULL;
	char opt[512] = "";
	char value[512] = "";
	char cpy_subopt[1024] = "";

	strcpy(cpy_subopt, subopt);

	found_ch = strchr(cpy_subopt, '=');

	if(found_ch != NULL)
		*found_ch = ' ';

	// printf("subopt2: %s\n", cpy_subopt);

	sscanf(cpy_subopt, "%s", opt);
	sscanf(cpy_subopt, "%*s %s", value);

	// printf("opt: %s\n", opt);
	// printf("value: %s\n", value);

	if(strlen(opt) == 1){
		strcpy(formated_subopt, "-");
		strcat(opt, " ");
	}
	else{
		strcpy(formated_subopt, "--");
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

	//printf("copy: %s\n", cpy_cluster_opts);

	token = strtok(cpy_cluster_opts, ",");

	while(token != NULL){
		char subopt[1024] = "";
		
		format_cluster_subopt(token, subopt);

		strcat(formated_opts, subopt);
		strcat(formated_opts, " ");


		//printf("token: %s\n", token);
		token = strtok(NULL, ",");
	}

	return 1;
}

int main(int argc, char *argv[])
{
	int opt, opt_index = 0;
	int cluster_mode = 0;

	char make_cluster_command[1024] = "";
	
	char* cluster_options = NULL;
	char make_options[1024] = "";
	char srun_options[1024] = "";

    static struct option long_options[] =
    {
      {"cluster", optional_argument, 0, 'c'},
      {0, 0, 0, 0}
    };

	while ((opt = getopt_long(argc, argv, "c:", long_options, &opt_index)) != -1){
		
		switch (opt){
			case 'c':
				cluster_mode = 1;
				cluster_options = optarg;
				printf("cluster_opts %s\n", optarg);

				break;
			// An opt with arg have no arg.
            case '?':
            	if(optopt == 'c')
            		printf("option -%c with no arguments\n", optopt);
            	break;
			default:
				break;
		}
	}


	if(cluster_mode){

		strcat(make_cluster_command, "srun ");

		// formate les options associés au cluster en options pour srun et les stockent dans la chaine srun_options 
		if(cluster_options != NULL)
			format_cluster_opts(cluster_options, srun_options);
		//else
			//find_partition()

		// copy toutes les options du make (sans les options cluster) dans la chaine make_options 
		
		for (int i = 1; i < argc; ++i)
		{
			if(strcmp(argv[i], "-c") != 0 && strcmp(argv[i], "--cluster") != 0 && strcmp(argv[i], cluster_options) != 0)
				strcat(make_options, argv[i]); 
		}

		strcat(make_cluster_command, srun_options);
		strcat(make_cluster_command, "./make ");
		strcat(make_cluster_command, make_options);
	}

	printf("Cluster mode: %d\n", cluster_mode);
	printf("Command: %s\n", make_cluster_command);

/*
	if(cluster_mode)
		system(make_cluster_command);
	else
		execvp("./make", argv+1);
*/
	return 0;
}

/*



./cluster ;
./cluster -p lirmm7 "$EXECS; $OPTIONS; $DATA;"
./cluster -p lirmm7 "somme division estpremier gcc; null "-i" "-I 10-20 --list" "-W -Wall -o nom" ; "x y z" "x y" "x y" "fichier1 fichier2 fichier2"
./cluster -p lirmm7 ./solveur ; "-i -cul" ; %"fichier1 fichier2 fichier3" ;

le symbole % signifie qu'on  execute le programme sur chaque données.

Option pour le programme global
et pour les sous-programmes qui s'execute a partir du programme global
penser au cas ou une variable est null (n'as pas de données)

On parse la ligne de command global en recupérant les options et la chaine de traitements "$EXECS;$OPTIONS;$DATA;"
 
On decoupe la chaine de traitements en tokens ';' qu'on reparties dans les strings
EXECS_STRING;
OPTIONS_STRING;
DATAS_STRING;

On decoupe les strings en tokens ' ' qu'on reparti dans les tableaux de strings
EXECS
OPTIONS
DATAS

pour chaque EXECS[i] on fait

si DATAS[I] ne commence pas par % -> ça veut dire qu'on parrallélise l'execution des datas;
	system(srun GLOBAL_OPTIONS EXECS[I] + OPTIONS[I] + DATAS[I] + &)
sinon
	on decoupe DATA[i] en tokens ' ' -> TOKENS
	tant qu'il y a des tokens alors
		system(srun GLOBAL_OPTIONS EXECS[I] + OPTIONS[I] + TOKENS + &)

on parse la ligne de commande:
on recupère le nombre de variables et on fait un tableau de Variables;

dans notre exemple on à 4 variables

VARS[5]


 */