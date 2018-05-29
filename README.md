# Make-Cluster

Un make compatible avec les clusters SLURM.

# Presentation

Utillise la puissance des clusters pour executer les makefile

# Nouvelle option

l'execution sur les cluster ce fait via l'ajout d'une nouvelle options,

-c|--cluster=[OPTION[0],OPTION[1],...]

OPTION correspond à une option de srun en enlevant les '-' ou '--', elles doivent etre collées entre elles par des ','.

On peut enchainer les options de la manière suivante:

```bash
make -c partition=NAME,ntask=1,nodes=1
```
=> execute make sur le cluster avec la partition NAME 1 fois sur 1 seul des noeuds disponibles

La commande suivante est strictement equivalente:

```bash
make -c p=NAME,n=1,N=1
```
=> la partition doit forcement être precisé pour executé la commande

# Exemples

Executer make sur le cluster avec la partition NAME:

```bash
make -c partition=NAME
```
Executer make sur le cluster avec la partition NAME, en parallelisant les taches sur tout les coeurs du clusters necessaires:

```bash
make -c partition=NAME -j
```
Executer make sur le cluster avec la partition NAME, en parallelisant les taches sur 10 coeurs du clusters à la fois:

```bash
make -c partition=NAME -j10
```

Utiliser la commande 'sinfo' pour voir les partitions disponibles du cluster

# Liste des ajouts

### Nouveaux fichiers:

**cluster.h:**
	* declarations des fonctions pour l'utilisation du cluster

**cluster.c:**
	* definitions des fonctions pour l'utlilisation du cluster

### Fichiers modifiés:

**main.c:**
	* ajout d'une entrées pour l'option '-c' dans le tableau des usages L 337 à 418
	* ajout d'un patch pour reconnaitre l'options '-c' dans la fonction 'main' L 1076 à 1119

**job.c:**
 	* ajout du patch au début de la fonction 'child_execute_job' L 2147 à 2177

**build.sh:**
 	* ajout du fichier cluster.c à la compilation du programme make

**Makefile:**
	* ajout du fichier cluster.c à la compilation du programme make

# Compilation du make-cluster

Executez ces commandes successivement dans le dossier make-4.2:

```bash
./configure
./build.sh
```



