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

SEED=27467
N=10000
size=10000
#echo "Parameters for SLiM run:"
#echo "  $ALL_PARAMS"

TIME_MEM_FILE_N=unordered_neutral_time_arg.N$N"."size$size".out"
TIME_MEM_FILE_P=topt_unordered_pedigree_time_arg.N$N"."size$size".out"

#echo "Task ID $SLURM_ARRAY_TASK_ID - seed $SEED"

#/usr/bin/time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' --output="./Profiles/"$TIME_MEM_FILE_N \
#     ../SLiM/bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ./BenchRecipes/benchmarks_neutral.slim

/usr/bin/time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' --output="./Profiles/"$TIME_MEM_FILE_P \
     ../SLiM/bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ./BenchRecipes/benchmarks_pedigree.slim
     

