#!/bin/bash

#PBS -N MPI-Benchmark-IO-RW
#PBS -l walltime=0:10:00
#PBS -j oe
#SBATCH -n 48 --ntasks-per-node=4
##SBATCH --constraint=[wes|san|has|bro]
#SBATCH --constraint=bro

case=$SLURM_JOB_NAME

#
# Initialize the log file
echo " -------------------------------------------------------------------------------" > $case.log
echo "| Started:       `date`"              >> $case.log
echo "| Case name:     $case"               >> $case.log
echo "| Job ID:        $SLURM_JOBID"        >> $case.log
echo "| Nodes:         $SLURM_JOB_NODELIST" >> $case.log
echo "| Working Dir:   $SLURM_SUBMIT_DIR"   >> $case.log
echo "| # of Procs:    $SLURM_NPROCS"       >> $case.log
echo " -------------------------------------------------------------------------------" >> $case.log

env

echo "" >> $case.log
echo "Write test:" >> $case.log
mpiexec_mpt _inst/bin/test_io -x -W -S 4e9 >> $case.log

echo "" >> $case.log
echo "Read test:" >> $case.log
mpiexec_mpt _inst/bin/test_io -x -R -S 4e9 >> $case.log

echo "" >> $case.log
echo "...finished $case..." >> $case.log
