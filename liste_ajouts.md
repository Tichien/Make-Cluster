# LA LISTE DES AJOUTS APPORTÉS AU GNU-MAKE DANS LE CADRE DE CE PROJET:

## NOUVEAUX FICHIERS:

cluster.h (declarations des fonctions pour l'utilisation du cluster)
cluster.c (definitions des fonctions pour l'utlilisation du cluster)

## FICHIERS MODIFIES:

main.c 		(ajout du patch pour le cluster au debut de la fonction 'main')
job.c 		(ajout du patch au début de la fonction 'child_execute_job')

build.sh 	(ajout du fichier cluster.c à la compilation du programme make)
Makefile	(ajout du fichier cluster.c à la compilation du programme make)