#!/bin/bash

#PBS -N mpi-pt2pt
#PBS -l walltime=0:10:00
#SBATCH --nodes=2 --ntasks-per-node=1
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

for tool in osu_bw osu_bibw osu_latency ; do

    echo "# --------------- " $tool " ---------------" >> $case.log
    echo " zone t=\"" $case " - " $tool "\"" >> $case.log

    #for run in $(seq 0 1); do

         mpiexec_mpt _inst/bin/$tool >> $case.log

    #done
done
