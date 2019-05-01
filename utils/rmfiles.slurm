#!/bin/bash
#SBATCH --constraint=[wes|san|has|bro|sky]
#SBATCH --job-name=lfs_cleanup      # Job name
#SBATCH --nodes=1                   # Use one node
#SBATCH --mem-per-cpu=1gb           # Memory per processor
#SBATCH --partition=twoday
#SBATCH --time=46:0:00              # Time limit hrs:min:sec
#SBATCH --output=array_%A-%a.out    # Standard output and error log
#SBATCH --array=0-4                 # Array range start-stop:(optional max_run_at_once)

# This is an example script that combines array tasks with
# bash loops to process many short runs. Array jobs are convenient
# for running lots of tasks, but if each task is short, they
# quickly become inefficient, taking more time to schedule than
# they spend doing any work and bogging down the scheduler for
# all users.

echo " -------------------------------------------------------------------------------"
echo "| Started:       `date`"
echo "| Case name:     $case"
echo "| Job ID:        $SLURM_JOBID"
echo "| Nodes:         $SLURM_JOB_NODELIST"
echo "| Working Dir:   $SLURM_SUBMIT_DIR"
echo "| # of Procs:    $SLURM_NPROCS"
echo "| # of Nodes:    $SLURM_NNODES"
echo "| # Tasks/Node:  $SLURM_TASKS_PER_NODE"
echo "|"
echo "| SLURM_ARRAY_JOB_ID=$SLURM_ARRAY_JOB_ID"
echo "| SLURM_ARRAY_TASK_ID=$SLURM_ARRAY_TASK_ID"
echo "| SLURM_ARRAY_TASK_COUNT=$SLURM_ARRAY_TASK_COUNT"
echo "| SLURM_ARRAY_TASK_MIN=$SLURM_ARRAY_TASK_MIN"
echo "| SLURM_ARRAY_TASK_MAX=$SLURM_ARRAY_TASK_MAX"
echo " -------------------------------------------------------------------------------"



cleandir=/nobackup/spedrott/damocles2/main/data_output_archive/

pwd; hostname; date

#Set the number of runs that each SLURM task should do
PER_TASK=1

# Calculate the starting and ending values for this task based
# on the SLURM task and the number of runs per task.
START_NUM=$(( $SLURM_ARRAY_TASK_ID * $PER_TASK ))
END_NUM=$(( $START_NUM + $PER_TASK ))

# Print the task and run range
echo This is task $SLURM_ARRAY_TASK_ID, which will do runs $START_NUM to $END_NUM

# Run the loop of runs for this task.
for (( run=$START_NUM; run<$END_NUM; run++ )); do
  echo This is SLURM task $SLURM_ARRAY_TASK_ID, run number $run

  #Do your stuff here
  cd ${cleandir} || exit 1
  pwd  
  cmd="lfs find ${cleandir} -type f -print0 | xargs -0 -r --max-args=1000 rm -f"
  echo "Running '"${cmd}"'"

  # actual remove, comment out to test
  eval ${cmd}

done

echo "sleeping..."
sleep 10
echo "done."

date
