#!/bin/sh

#PBS -N dotprod_fast_af
#PBS -l walltime=60:00
#PBS -l nodes=1:r662:ppn=48
#PBS -q mei

module load gcc/5.3.0
module load papi/5.4.1

export LD_LIBRARY_PATH=$PAPI_DIR/lib: $LD_LIBRARY_PATH

cd $PBS_O_WORKDIR

echo "Compiling..."
make

echo "Running..."
./dotprod

echo "Cleaning..."
make clean
