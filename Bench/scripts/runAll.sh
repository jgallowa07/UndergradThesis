#!/bin/bash

SEED=27467
#SECONDS_TO_KILL=`echo "72*60*60"|bc -l`

for N in 1000 5000 10000
do
	for size in 1000 2500 5000 100000   
	do	
		
		PED_OUTPUT_FILE_NAME=ped_output.N$N"."size$size".out"
		PED_TIME_FILE_NAME=ped_timing.N$N"."size$size".out"
		PED_ADD_TIME_FILE_NAME=ped_add_timing.N$N"."size$size".out"
		PED_ADD_OUTPUT_FILE_NAME=ped_add_output.N$N"."size$size".out"
		PED_TABLE_FILE=$N"_"$size"Tables"

		NEUT_OUTPUT_FILE_NAME=neut_output.N$N"."size$size".out"
		NEUT_TIME_FILE_NAME=neut_timing.N$N"."size$size".out"

		PEDNM_OUTPUT_FILE_NAME=pedNM_output.N$N"."size$size".out"
		PEDNM_TIME_FILE_NAME=pedNM_timing.N$N"."size$size".out"

		REL_OUTPUT_FILE_NAME=rel_output.N$N"."size$size".out"
		REL_TIME_FILE_NAME=rel_timing.N$N"."size$size".out"

		MUTRATE=$(echo "scale=10; ($size / (4.0 * $N)) / 100000000.0" | bc)

		#Neutral Mutations
		/usr/local/Cellar/gnu-time/1.9/bin/gtime --format='%E / %S / %U / %M' \
		  --output="../ComparisonTimes/RealTimes/$NEUT_TIME_FILE_NAME" ../../../SLiM/bin/slim \
		  -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_neutral.slim &> ../Output/Reals/$NEUT_OUTPUT_FILE_NAME

		#Neutral Mutations Release SLiM
		/usr/local/Cellar/gnu-time/1.9/bin/gtime --format='%E / %S / %U / %M' \
		  --output="../ComparisonTimes/RealTimes/$REL_TIME_FILE_NAME" ../../../SLiM2_6/bin/slim \
		  -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_neutral.slim &> ../Output/Reals/$REL_OUTPUT_FILE_NAME

		#Pedigree Recording With Mutations
		/usr/local/Cellar/gnu-time/1.9/bin/gtime --format='%E / %S / %U / %M' \
		  --output="../ComparisonTimes/RealTimes/$PED_TIME_FILE_NAME" ../../../SLiM/bin/slim \
		  -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree.slim &> ../Output/Reals/$PED_OUTPUT_FILE_NAME
		
		#Time To layover neutral mutations over the trees
		/usr/local/Cellar/gnu-time/1.9/bin/gtime --format="%E / %S / %U / %M" \
		  --output="../ComparisonTimes/RealTimes/$PED_ADD_TIME_FILE_NAME" \
		  /usr/local/bin/python3.6 add-mutation.py --mut_rate $MUTRATE --tree_file ../Tables/$PED_TABLE_FILE -o ../VCF_Tables/ &> ../Output/Reals/$PED_ADD_OUTPUT_FILE_NAME  

		#Pedigree Recording W/O Mutations
		/usr/local/Cellar/gnu-time/1.9/bin/gtime --format='%E / %S / %U / %M' \
		  --output="../ComparisonTimes/RealTimes/$PEDNM_TIME_FILE_NAME" ../../../SLiM/bin/slim \
		  -s $SEED -d n_ind=$N -d size_rho=$size ../BenchRecipes/benchmarks_pedigree_nm.slim &> ../Output/Reals/$PEDNM_OUTPUT_FILE_NAME

	done
done




