#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>

static char doc[] = "Ce programme permet de paralleliser des taches.";

/* Description des aguments accepter. */
static char args_doc[] = "[OPTIONS] [EXEC(0)] [ARGS(0)..]";

static struct argp_option options[] = {
	{ "nodes", 'N', "<minnodes[-maxnodes]>", 0, "Nombre de noeuds min par programmes", 0 },
	{ "ntasks", 'n', "<number>", 0, "Nombre de taches par programmes", 0 },
	{ "partition", 'p', "<partition_name>", 0, "Partition sur laquelle executer les programmes", 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

/* Les arguments du programme qui communiquent avec parse_opt. */
struct arguments
{
	char* exec;
	char** args;
	char* partition;
	int nodes, ntasks;
};

/* Parseur d'options. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
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

	    arguments->exec = arg;

     	arguments->args = &state->argv[state->next];
      	state->next = state->argc;
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
	char srun_command[1024] = "srun ";
	char srun_options[1024] = "";
  	struct arguments arguments;

  	memset(&arguments, 0, sizeof(arguments));

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

	while(arguments.args[i]){
		
		char current_line[1024] = "";

		strcat(current_line, srun_command);
		strcat(current_line, arguments.exec);
		strcat(current_line, " ");
		strcat(current_line, arguments.args[i]);
		strcat(current_line, " &");

		system(current_line);

		i++;
	}

	return 0;
}