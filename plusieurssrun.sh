#!/bin/sh

for i in `seq 30 60`; do
	srun -n 1 --partition=lirmm1 sleep $i&
done
