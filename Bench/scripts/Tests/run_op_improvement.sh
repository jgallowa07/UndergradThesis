#!/bin/bash
SEED=27467
N=10
size=10
#echo "Parameters for SLiM run:"
#echo "  $ALL_PARAMS"



TIME_MEM_FILE_N=neutral_time_arg.N$N"."size$size".out"
TIME_MEM_FILE_I=initial_pedigree_time_arg.N$N"."size$size".out"
TIME_MEM_FILE_U=unordered_map_time_arg.N$N"."size$size".out"

#echo "Task ID $SLURM_ARRAY_TASK_ID - seed $SEED"

time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' --output="../Optimizations/"$TIME_MEM_FILE_N \
     ../../initial_implimentation_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_neutral.slim

time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' --output="../Optimizations/"$TIME_MEM_FILE_I \
     ../../initial_implimentation_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree.slim

time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' --output="../Optimizations/"$TIME_MEM_FILE_U \
     ../../unordered_map_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree.slim

#/usr/bin/time --format='elapsed: %E / kernel: %S / user: %U / mem: %M' --output="./Profiles/"$TIME_MEM_FILE_P \
#     ../SLiM/bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ./BenchRecipes/benchmarks_pedigree.slim
     

