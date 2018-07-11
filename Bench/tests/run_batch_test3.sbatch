#!/bin/bash
#SBATCH -p long
#SBATCH -n 1
#SBATCH -t 40:00:00
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

for N in 1000 10000 
do
	for R in 1000 10000
	do	
		SEED=$(printf "%06d" $RANDOM); 
		FILENAME="./profiling/neutral_run_timings"

		#echo "Task ID $SLURM_ARRAY_TASK_ID - seed $SEED"

		ALL_PARAMS="-s $SEED -d n_ind=$N -d size_rho = $R"
		FORMAT="$N $R $SEED : %E / kernel: %S / user: %U / mem: %M"
		
		/usr/bin/time --format=$FORMAT --output=$FILENAME
			echo "WE IN DIS FOR N = $N and R = $R"
		echo "Done!"
	done
done

