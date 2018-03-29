#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <argp.h>

static char doc[] = "Ce programme permet de paralleliser des taches.";

/* A description of the arguments we accept. */
static char args_doc[] = "[OPTIONS] [EXEC(0)] [ARGS(0)..]";

static struct argp_option options[] = {
	{ "nodes", 'N', "<minnodes[-maxnodes]>", 0, "Nombre de noeuds min par programmes", 0 },
	{ "ntasks", 'n', "<number>", 0, "Nombre de taches par programmes", 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
	char* exec;
	char** args;
	int nodes, ntasks;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
	struct arguments *arguments = (struct arguments*)state->input;

	switch (key)
	{
		case 'N':
		arguments->nodes = atoi(arg);
		break;

		case 'n':
		arguments->ntasks = atoi(arg);
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

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0};

int main(int argc, char *argv[])
{

  	struct arguments arguments;

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	int i = 0;
	char srun_command[1024] = "srun -n 1 --partition=lirmm1 ";
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