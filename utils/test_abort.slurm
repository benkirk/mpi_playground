#!/bin/bash -l

#PBS -N MPI-Abort
#PBS -l walltime=0:10:00
#PBS -j oe
#SBATCH -n 12
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
| TMP Dir:       $SLURM_JOB_TMPDIR
| # of Procs:    $SLURM_NPROCS
| # of Nodes:    $SLURM_NNODES
| # Tasks/Node:  $SLURM_TASKS_PER_NODE
 -------------------------------------------------------------------------------
EOF

#export MPIEXEC_MPT_DEBUG=1
#export MPI_VERBOSE=1
#export MPI_COREDUMP=INHIBIT
#export MPI_DEBUG_ABORT=10

#export MPI_COREDUMP_VERBOSE=1

#env >> $case.log

module swap intel intel/17.0.2
module list >> $case.log 2>&1


echo "" >> $case.log
echo "Abort test:" >> $case.log

sleep 10

mpiexec_mpt _inst/bin/abort >> $case.log 2>&1
#mpiexec _inst/bin/abort >> $case.log 2>&1

echo "Control returned to script at `date`" >> $case.log 2>&1
