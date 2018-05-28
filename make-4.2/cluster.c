#include "cluster.h"

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

	// printf("subopt2: %s\n", cpy_subopt);

	sscanf(cpy_subopt, "%s", opt);
	sscanf(cpy_subopt, "%*s %s", value);

	// printf("opt: %s\n", opt);
	// printf("value: %s\n", value);
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

/* Renvoie 1 si l'option du cluster est detecté est stocke les options dans cluster_opts, sinon renvoie 0 */
int get_cluster_opt(int argc, char* argv[], char* cluster_opts){

	int opt, opt_index = 0;
	int cluster_on = 0;

    /* annule l'affichage des erreurs par getopt_long */
    opterr = 0;

	static struct option long_options[] =
    {
      {"cluster", optional_argument, 0, 'c'},
      {0, 0, 0, 0}
    };

    printf("debut parse\n");
	while ((opt = getopt_long(argc, argv, "c:", long_options, &opt_index)) != -1){
		printf("pass boucle\n");
		switch (opt){
			case 'c':
				cluster_on = 1;
            	printf("cluster with arg");
				strcpy(cluster_opts, optarg);
            	printf("cluster mode: activated with options %s\n", cluster_opts);
            	break;
			/* l'option n'est pas reconnu ou sans argument*/
            case '?':
            	if(optopt == 'c'){
            		fprintf(stderr, "%s: option requires an argument -- 'c'\n", argv[0]);
					exit(-1);
            	}
            	else
            		printf("pass\n");
            	break;
            default:
            	exit(-1);
				break;
		}
	}
    printf("fin parse\n");

	return cluster_on;
}

/* Si une partition a été préciser dans les options renvoie 1, sinon renvoie 0 */
int have_partition(){

	if(strcmp(cluster_execution_partition, "") != 0)
		return 1;
	else
		return 0;
}

/* PATCH CLUSTER */

/*
if(get_cluster_opt(argc, argv, cluster_opts){

	char make_cluster_command[1024] = "srun ";
	char make_options[1024] = "";

	format_cluster_opts(cluster_opts, formated_opts);

	have_partition(default_partition);

	if(strcmp(default_partition, "") != 0){
		strcat(make_cluster_command, "--partition=");
		strcat(make_cluster_command, default_partition);
		strcat(make_cluster_command, " ");
	}

	strcat(make_cluster_command, formated_opts);

	for (int i = 0; i < argc; ++i){
		if(strcmp(argv[i], "-c") != 0 && strcmp(argv[i], "--cluster") != 0 && argv[i] != cluster_options)
			strcat(make_options, argv[i]); 
	}

	strcat(make_cluster_command, make_options);

	system(make_cluster_command);
}
*/

/* FIN PATCH CLUSTER */

/*
int main(int argc, char *argv[])
{
	int opt, opt_index = 0;
	int cluster_mode = 0;
	char default_partition[1024] = "";

	char make_cluster_command[1024] = "";
	
	char* cluster_options = NULL;
	char make_options[1024] = "";
	char srun_options[1024] = "";
	char absolute_dir_path[1024] = "";
	char* found_point = NULL;

	strcpy(absolute_dir_path, argv[0]);

	found_point = strrchr(absolute_dir_path, '.');

	if(found_point)
		*found_point = '\0';

	//getabsolute_dir_path(absolute_dir_path, sizeof(absolute_dir_path));

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
            	if(optopt == 'c'){
            		cluster_mode = 1;
            		printf("cluster mode: activated\n");
            	}
            	break;
			default:
				break;
		}
	}
	int i = 0;
	// copie toutes les options du make (sans les options cluster) dans la chaine make_options 
	for (i = 1; i < argc; ++i){
		if(strcmp(argv[i], "-c") != 0 && strcmp(argv[i], "--cluster") != 0 && argv[i] != cluster_options)
			strcat(make_options, argv[i]); 
	}

	// seulement si le mode cluster est activé (-c, --cluster).
	if(cluster_mode){

		strcat(make_cluster_command, "srun ");

		// formate les options associés au cluster en options pour srun et les stockent dans la chaine srun_options 
		if(cluster_options != NULL)
			format_cluster_opts(cluster_options, srun_options);
		else{
			if(find_partition(default_partition) != -1){
				strcat(make_cluster_command, "-p ");
				strcat(make_cluster_command, default_partition);
				strcat(make_cluster_command, " ");
			}
		}


		strcat(make_cluster_command, srun_options);
		strcat(make_cluster_command, absolute_dir_path);
		strcat(make_cluster_command, "./make -j ");
	}
	else{
		strcat(make_cluster_command, absolute_dir_path);
		strcat(make_cluster_command, "./make ");
	}

	strcat(make_cluster_command, make_options);
	
	//printf("Cluster mode: %d\n", cluster_mode);
	printf("Command: %s\n", make_cluster_command);
	//printf("argv[0]: %s\n", argv[0]);
	//printf("WorkingDir: %s\n", absolute_dir_path);

	system(make_cluster_command);

	return 0;
}
*/
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