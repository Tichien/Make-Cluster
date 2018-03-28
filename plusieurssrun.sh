#!/bin/sh

for i in `seq 0 39`; do
	srun -n 1 --partition=lirmm1 ./Prime/prime -I 0-10000000 --somme $i&
done
