#!/bin/bash

#PBS -N MPI-Benchmark-IO-RW
#PBS -l walltime=0:10:00
#PBS -j oe
#SBATCH --nodes=8
#SBATCH --ntasks-per-node=12
#SBATCH --constraint=[san|has|bro|sky]

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

env | sort >> $case.log


echo "" >> $case.log
echo "Write test:" >> $case.log
mpiexec _inst/bin/test_io -x -W -S 5e8 >> $case.log 2>&1

echo "" >> $case.log
echo "Read test:" >> $case.log
mpiexec _inst/bin/test_io -x -R -S 5e8 >> $case.log 2>&1

echo "" >> $case.log


# optional bits
[ -x _inst/bin/hello ] && mpiexec _inst/bin/hello >> $case.log 2>&1

file=_inst/bin/mpi_io_collective_write
[ -f ${file} ] && \
    mpiexec ${file} >> $case.log || \
    echo "No ${file} found." >> $case.log

file=_inst/bin/mpi_io_collective_read
[ -f ${file} ] && \
    mpiexec ${file} >> $case.log || \
    echo "No ${file} found." >> $case.log

echo "...finished $case..." >> $case.log
