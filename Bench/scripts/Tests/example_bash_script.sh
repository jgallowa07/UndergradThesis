#!/bin/bash

SEED=27467
N=10000
size=10000

TIME_MEM_FILE_N=neutral_time_arg.N$N"."size$size".out"
TIME_MEM_FILE_I=initial_pedigree_time_arg.N$N"."size$size".out"
TIME_MEM_FILE_U=unordered_map_time_arg.N$N"."size$size".out"
TIME_MEM_FILE_G=genomeID_time_arg.N$N"."size$size".out"

#(time ../../../initial_implimentation_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_neutral.slim &> ../slurm_runs/$TIME_MEM_FILE_N) \
#	&> ../Optimizations/$TIME_MEM_FILE_N


#(time ../../../initial_implimentation_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree.slim &> ../slurm_runs/$TIME_MEM_FILE_I) \
#	&> ../Optimizations/$TIME_MEM_FILE_I


#(time ../../../unordered_map_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree.slim &> ../slurm_runs/$TIME_MEM_FILE_U) \
#	&> ../Optimizations/$TIME_MEM_FILE_U


(time ../../../genomeID_bin/slim -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree.slim &> ../slurm_runs/$TIME_MEM_FILE_G) \
	&> ../Optimizations/$TIME_MEM_FILE_G
