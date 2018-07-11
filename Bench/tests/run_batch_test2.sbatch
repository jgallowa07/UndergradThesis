#!/bin/bash
#SBATCH -p long
#SBATCH -n 1
#SBATCH -t 100:00:00
#SBATCH --mem-per-cpu=8G
#SBATCH --ntasks-per-core=1

# change to directory you were in when you submit the job
cd $SLURM_SUBMIT_DIR

#: ${PARAMS?Must define PARAMS}
#if [ -z "$PARAMS" ]
#then
#    echo "Must define PARAMS (is empty)."
#    exit 1
#fi

SEED=$(printf "%06d" $RANDOM); 
NIND="1000"
RHO="1000"
#echo "Parameters for SLiM run:"
#echo "  $ALL_PARAMS"

FILENAME="neutral_run_"$(NIND)"_"$(RHO)"_"$(SEED)

#echo "Task ID $SLURM_ARRAY_TASK_ID - seed $SEED"

ALL_PARAMS="-s $SEED -d n_ind=$NIND -d size_rho = $RHO"

/usr/bin/time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' \
	echo "HOLLA ATCHA BOI" \
    	&>./Profiles/$FILENAME		

echo "Done!"

