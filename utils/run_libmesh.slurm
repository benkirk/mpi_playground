#!/bin/bash

#PBS -N MPI-libMesh
#PBS -l walltime=0:10:00
#PBS -j oe
#SBATCH -n 128
##SBATCH --constraint=[san|has|bro|sky]
##SBATCH --constraint=[bro]

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

env | egrep "MPI_|COMPILER|SLURM" | sort >> $case.log

# optional bits

## hello world
[ -x _inst/bin/hello ] && \
    mpiexec _inst/bin/hello >> $case.log 2>&1 || \
    { echo "no _inst/bin/hello found!\n"; }


## poisson solver
tstart=`date +%s`
[ -x _inst/bin/poisson ] && \
    mpiexec _inst/bin/poisson -d 3 -n 80 -ksp_monitor -log_view >> $case.log 2>&1 || \
    { echo "no _inst/bin/poisson found!\n"; }
tstop=`date +%s`
echo "Poisson: $(($tstop-$tstart)) seconds total." >> $case.log


echo "...finished $case..." >> $case.log
