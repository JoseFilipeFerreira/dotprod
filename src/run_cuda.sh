#!/bin/sh

#PBS -N the_fastest_cuda_in_the_west
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r662:k20:ppn=24
#PBS -q mei

module load cuda

cd $PBS_O_WORKDIR

for run in {1..8}; do
    nvprof ./cuda
done
