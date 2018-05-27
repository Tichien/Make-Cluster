#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "prime.h"

static char doc[] = "Ce programme permet d'appliquer plusieurs operations sur les nombres premiers.";

/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 ARG2...";

static struct argp_option options[] = {
	{ "interval", 'I', "FROM-TO", 0, "Interval sur lequel faire les operations", 0 },
	{ "list", 'l', 0, 0, "list les nombres premiers sur [FROM, TO]", 0 },
	{ "naif", 'n', 0,  0, "Utilise un algo naif pour les tests de primalité", 0 },
	{ "somme", 's', 0, 0, "Additionne les nombres sur [FROM, TO]", 0 },
	{ "precision", 'p', "NUMBER", 0, "Precision de l'algorithme MullerRabin (7 de base)", 0 },
	{ 0, 0, 0, 0, 0, 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
	char* args;
	long long from, to;
	int naif, somme, list, precision;
};

/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	char* s = NULL;
  /* Get the input argument from argp_parse, which we
     know is a pointer to our arguments structure. */
	struct arguments *arguments = (struct arguments*)state->input;

	switch (key)
	{
		case 'I':
		if((s = strtok(arg, "-")))
			arguments->from = atoll(s);
			
			if((s = strtok(NULL, "-")))
				arguments->to = atoll(s);
			else 
				printf("Syntaxe error --interval/-I FROM-TO\n");
		break;

		case 'l':
		arguments->list = 1;
		break;

		case 'n':
		arguments->naif = 1;
		break;

		case 's':	
		arguments->somme = 1;
		break;
		
		case 'p':
		arguments->precision = atoi(arg);
		break;

		case ARGP_KEY_ARG:
		strcat(arguments->args, arg);
		strcat(arguments->args, " ");
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
	char* arg = NULL;
	char args[1024] = "";
	bool premier = false;
	long long sum = 0;

	struct arguments arguments = { args, 0, 0, 0, 0, 0, 7 };

	argp_parse(&argp, argc, argv, 0, 0, &arguments);

	arg = strtok(arguments.args, " ");

	while(arg){
		
		long long num = atoll(arg);

		if(arguments.naif)
			premier = estPremierNaif(num);
		else
			premier = MillerRabin(num, arguments.precision);
	
		if(premier)
			printf("%lld est premier\n", num);
		else
			printf("%lld n'est pas premier\n", num);

		arg = strtok(NULL, " ");
	}

	for(long long i = arguments.from ; i <= arguments.to ; i++){

		if(arguments.naif)
			premier = estPremierNaif(i);
		else
			premier = MillerRabin(i, arguments.precision);

		if(premier){

			if(arguments.somme)
				sum += i;

			if(arguments.list)
				printf("%lld\n", i);
		}
	}

	if(sum)
		printf("La somme des nombres premiers de %lld à %lld est %lld\n", arguments.from, arguments.to, sum);
	
	return 0;
}
