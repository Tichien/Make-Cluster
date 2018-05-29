OBJECTIF:

Modifier le code source du gnu make pour y inserer du code permettant d'executer facilement des taches en parallèles sur le cluster slurm.
Inclure des options permettant de controler et d'obtenir des informations sur les taches en execution.

En ligne de commande cela donnerai:

make --cluster [OPTIONS] ([EXEC1] [DATA1]) ([EXEC2] [DATA2])...

PROTOTYPE:

slurm permet d'interpreter des scripts batch facilitant la parallelisation de taches (sbatch [SCRIPT]).

SCRIPT: parallel [EXECUTABLE] [DONNEES]

DESCRIPTION: le programme va executer en parallèle chaque données de la variable [DONNEES] sur les noeuds disponible du cluster jusqu'à ce que toutes les données soit traités.

ENTREE: [EXECUTABLE] => un programme qui opere sur une donnée
		[DONNES] => les données à traiter par l'executable

SORTIE: un fichier contiendra les données de traitement de l'executable

PROTOTYPE POST RENDEZ-VOUS TER:

Creer le programme de parallelisation avec un script en bash avec la commande srun au lieu d'un script batch.
Verifier que les processus s'executent dès qu'un (ou plusieurs) coeur(s) est(sont) disponible(s)

Au dernier rendez-vous TER un problème est survenu:
Quand trop de processessus sont en cours certains attendent pour s'executer alors que des coeurs sont disponibles.

Trouver un moyen d'integrer le code qui va creer ce script et l'executer a partir du main du GNU make en c avant le prochain rendez-vous.
