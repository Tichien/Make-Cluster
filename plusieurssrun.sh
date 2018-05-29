#!/bin/sh

for i in `seq 30 60`; do
	srun --partition=lirmm1 sleep $i &
done
 
 