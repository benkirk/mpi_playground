#!/bin/bash

#PBS -N MPI-libMesh
#PBS -l walltime=0:10:00
#PBS -j oe
#SBATCH -n 128
#SBATCH --constraint=[san|has|bro|sky]

case=${SLURM_JOB_NAME}-${SLURM_JOBID}

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

env | sort >> $case.log

# optional bits
[ -x _inst/bin/hello ] && \
    mpiexec _inst/bin/hello >> $case.log 2>&1 || \
    { echo "no _inst/bin/hello found!\n"; exit 1; }

sleep $((3*60))

echo "...finished $case..." >> $case.log
