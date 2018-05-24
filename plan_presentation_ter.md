# MAKE-CLUSTER

# PROBLEMATIQUE

Mr. Delahaye chercheur au lirmm => veut executer un programme avec une durée tres longue sur plusieurs fichier, le mieux pour gagner du temps reste d'executer le programme sur chaque fichier en parallel.

Il fait ça sur son ordi à 20000 core avec un make -j, qui va automatiquement executer en parallel les taches ecritent dans un makefile pour chaque core disponible.

## EXPLICATION DU make -j

1. Fonctionnement
2. Simplicité
3. Efficacité
4. Etc...

Transition => cependant, au lirmm les chercheurs on un cluster a leur disposition qui leur permettrait de gagner encore plus de temps.

## EXPLICATION DU CLUSTER

1. Fonctionnement
2. Difficulté d'utilisation
3. Efficacité
4. Etc...

Transition => l'utillisation du cluster est donc efficace mais la prise en main et difficile et la documentation fastidieuse.

Mr. Delahayed vas donc preferer faire sont make -j quitte a perdre du temps au final

## NOTRE INTERVENTION

c'est la que Mr. Delahaye fait appel a nous, nous devons faire en sorte, d'utiliser la puissance de parallelisation du cluster mais en conservant la simplicité d'un make -j.

On a ainsi realiser MAKE-CLUSTER un patch du GNU make qui rend l'utilisation du cluster SLURM compatible avec make -j

Comme ça il pourra non seulement gagner du temps simplement, mais egalement faire gagner du temps a c'est collègue en partageant le patch.

# REALISATION

(à completer)

# LECTURE DES DOCUMENTATIONS

(à completer)

# LECTURE DU CODE SOURCE MAKE

(à completer)
