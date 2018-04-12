Commandes utiles:

sacct => affiche les informations d'executions et d'etat pour tous les jobs sur la session
srun --partition=lirmm1 [COMMAND] => 
sbatch => execute un script batch
scontrol show node => affiche tout les noeuds du cluster
scontrol show hostname => affiche tout les noeuds du cluster detaillés
squeue (-s) => affiche la queue d'attente pour l'execution des programmes

SLURM_NPROCS => nombre de coeurs allouées
SLURM_NNODES => nombre de noeuds allouées
SLURM_JOB_ID => numero du job actuel
SLURM_JOB_NODELIST => affiche les informations sur les noeuds allouees de manière synthétique

smap -i 1 => affiche les info sur les jobs avec ncurses toutes les secondes

srun -p lirmm1 -l env => affiche l'environnement du job lancé avec du genre SLURM_JOB_ID

Dans le gnu make voir dans les fichiers job.c/job.h les fonctions start_job_commands & child_execute_job
pour y inserer du code avec la command srun