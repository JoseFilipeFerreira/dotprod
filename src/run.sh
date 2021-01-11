#!/bin/sh

#PBS -N dotprod_fast_af
#PBS -l walltime=1:00:00
#PBS -l nodes=1:r662:ppn=48
#PBS -q mei

module load gcc/7.2.0
module load papi/5.5.0

export LD_LIBRARY_PATH=$PAPI_DIR/lib: $LD_LIBRARY_PATH

cd $PBS_O_WORKDIR

echo "Running..."
./dotprod

echo "Cleaning..."
make clean
