#!/bin/bash

#PBS -N MPI-Benchmark-IO-Read
#PBS -l walltime=0:10:00
#PBS -j oe
#SBATCH -n 64 --ntasks-per-node=2
#SBATCH --constraint=[wes|san|has|bro]

case=$SLURM_JOB_NAME

#
# Initialize the log file
cat <<EOF > $case.log
 -------------------------------------------------------------------------------
| Started:       `date`
| Case name:     $case
| Job ID:        $SLURM_JOBID
| Nodes:         $SLURM_JOB_NODELIST
| Working Dir:   $SLURM_SUBMIT_DIR
| # of Procs:    $SLURM_NPROCS
| # of Nodes:    $SLURM_NNODES
| # Tasks/Node:  $SLURM_TASKS_PER_NODE
 -------------------------------------------------------------------------------
EOF

#env

echo "" >> $case.log
echo "Read test:" >> $case.log
mpiexec_mpt _inst/bin/test_io -x -R -S 4e9 >> $case.log 2>&1

echo "" >> $case.log
echo "...finished $case..." >> $case.log
