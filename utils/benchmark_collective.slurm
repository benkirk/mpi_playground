#!/bin/bash

#SBATCH --job-name=mpi-collective
#SBATCH -n 512
#SBATCH --time=00:30:00
##SBATCH --constraint=[wes|san|has|bro|sky]
#SBATCH --constraint=[wes|san|has|bro|sky]

case=$SLURM_JOB_NAME.$SLURM_JOBID

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

for tool in osu_barrier osu_bcast osu_gather osu_allgather osu_alltoall; do

    echo "# --------------- " $tool " ---------------" >> $case.log
    echo " zone t=\"" $case " - " $tool "\"" >> $case.log

    #for run in $(seq 0 1); do

         mpiexec_mpt _inst/bin/$tool >> $case.log

    #done
done
