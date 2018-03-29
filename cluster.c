#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>

#define STRING_BUF_SIZE 1024

void allocate(char** ARGV){

	ARGV = (char**) calloc(STRING_BUF_SIZE + 1, sizeof(char*));

	for (int i = 0; i < STRING_BUF_SIZE; i++ )
	{
		ARGV[i] = (char*) calloc(STRING_BUF_SIZE, sizeof(char));
		strcpy(ARGV[i], "");
	}

	ARGV[STRING_BUF_SIZE] = NULL;
}

void initialize(char** ARGV, char* ARGV_STRING){

	char* token = NULL;
	//token = strtok(ARGV_STRING, " ");

	int i = 0;

	while(token ){
		strcpy(ARGV[i], "");// =  token;
		
		i++;
		
		token = strtok(NULL, " ");
	}

	ARGV[i + 1] = NULL;
}

void printargv(char** ARGV){

	int i = 0;
	printf("ARGV :\n");
	while(ARGV[i]){
		printf("%s\n", ARGV[i]);
		i++;
	}
}

static char doc[] = "Ce programme permet de paralleliser des taches.";

/* Description des aguments accepter. */
static char args_doc[] = "[OPTIONS] ($EXECS ; $OPTIONS ; $ARGS)";

static struct argp_option options[] = {
	{ "nodes", 'N', "<minnodes[-maxnodes]>", 0, "Nombre de noeuds min par programmes", 0 },
	{ "ntasks", 'n', "<number>", 0, "Nombre de taches par programmes", 0 },
	{ "partition", 'p', "<partition_name>", 0, "Partition sur laquelle executer les programmes", 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

/* Les arguments du programme qui communiquent avec parse_opt. */
struct arguments
{

	//char* exec;
	//char** exec_opts;
	char** EXECS;//[STRING_BUF_SIZE][STRING_BUF_SIZE];
	char** OPTIONS;//[STRING_BUF_SIZE][STRING_BUF_SIZE];
	char** ARGUMENTS;//[STRING_BUF_SIZE][STRING_BUF_SIZE];
	//char** args;
	char* partition;
	int nodes, ntasks;
};

/* Parseur d'options. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	char* EXECS_STRING = NULL;
	char* OPTIONS_STRING = NULL;
	char* ARGUMENTS_STRING = NULL;
	/* Recupère les input à partir de argp_state qui est un pointeur vers les arguments du programme */
	struct arguments *arguments = (struct arguments*)state->input;

	switch (key)
	{
		case 'N':
		arguments->nodes = atoi(arg);
		break;

		case 'n':
		arguments->ntasks = atoi(arg);
		break;

		case 'p':
		arguments->partition = arg;
		break;

		case ARGP_KEY_NO_ARGS:
		argp_usage (state);

		case ARGP_KEY_ARG:

		EXECS_STRING = strtok(arg, ";"); 
		OPTIONS_STRING = strtok(NULL, ";");
		ARGUMENTS_STRING = strtok(NULL, ";");

		printf("EXECS_STRING : %s\n", EXECS_STRING);
		printf("OPTIONS_STRING : %s\n", OPTIONS_STRING);
		printf("ARGUMENTS_STRING : %s\n", ARGUMENTS_STRING);

		allocate(arguments->EXECS);
		allocate(arguments->OPTIONS);
		allocate(arguments->ARGUMENTS);

		initialize(arguments->EXECS, EXECS_STRING);
		initialize(arguments->OPTIONS, OPTIONS_STRING);
		initialize(arguments->ARGUMENTS, ARGUMENTS_STRING);

		printargv(arguments->EXECS);
		printargv(arguments->OPTIONS);
		printargv(arguments->ARGUMENTS);

		exit(0);

     	//arguments->args = &state->argv[state->next];
      	//state->next = state->argc;
		break;

		default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

/* Initialisation du parseur argp. */
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[])
{
	int i = 0;
	char srun_command[STRING_BUF_SIZE] = "srun ";
	char srun_options[STRING_BUF_SIZE] = "";
	
	char** SUB_ARGUMENTS = NULL;
	allocate(SUB_ARGUMENTS);
	
	struct arguments arguments;

	memset(&arguments, 0, sizeof(arguments));

  	/* arguments par defaults */
	arguments.nodes = 1;
	arguments.ntasks = 1;
	arguments.partition = "lirmm1";

  	/* Parse les arguments */
	argp_parse(&argp, argc, argv, 0, 0, &arguments);

  	/* Construit la chaine d'options. */
	if(arguments.nodes)
		sprintf(srun_options, "%s -N %d ", srun_options, arguments.nodes);

	if(arguments.ntasks)
		sprintf(srun_options, "%s -n %d ", srun_options, arguments.ntasks);

	if(arguments.partition)
		sprintf(srun_options, "%s -p %s ", srun_options, arguments.partition);

	/* Ajoute les options à la commande srun */
	strcat(srun_command, srun_options);

	while(arguments.EXECS[i]){
		
		char current_line[STRING_BUF_SIZE] = "";

		strcat(current_line, srun_command);
		strcat(current_line, arguments.EXECS[i]);
		strcat(current_line, " ");
		strcat(current_line, arguments.OPTIONS[i]);
		strcat(current_line, " ");

		//Si ARGUMENTS[i] ne commence pas par '%'
		if(!(arguments.ARGUMENTS[i][0] == '%')){
			
			strcat(current_line, arguments.ARGUMENTS[i]);
			strcat(current_line, " &");

			system(current_line);
		}
		else{
			int k = 0;
			
			initialize(SUB_ARGUMENTS, arguments.ARGUMENTS[i]);

			while(SUB_ARGUMENTS[k]){

				char sub_current_line[STRING_BUF_SIZE] = "";

				strcat(sub_current_line, current_line);
				strcat(sub_current_line, SUB_ARGUMENTS[k]);
				strcat(current_line, " &");

				system(current_line);
				k++;
			}
			
		}

		i++;
	}

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