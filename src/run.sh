#!/bin/sh

#PBS -N dotprod_120_block
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r662:ppn=48
#PBS -q mei

module load papi/5.5.0
module load gcc/7.2.0

cd $PBS_O_WORKDIR

for run in {1..8}; do
    ./dotprod_120_block
done
