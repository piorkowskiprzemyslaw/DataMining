#!/bin/bash

DM_CV=7
DM_DATASET="../DataMining/data.csv"
DM_EXEC="./DataMining -h -CV $DM_CV -ftrain $DM_DATASET "
AWK_SCRIPTFILE="parser-1.awk" #Script calculating Geometric Mean of Precission and Recall
AWK_EXEC="awk -f $AWK_SCRIPTFILE"

declare -a thresholds_arr=(0.01 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9 0.95)
declare -a runmode_arg_arr=("-DFT " "-MI_MAX -MI " "-MI_AVG -MI " "-CHI_MAX -CHI ")

#Perform experiments for 4 algorithms
for mode in "${runmode_arg_arr[@]}"
do
	for tshld in "${thresholds_arr[@]}"
	do
		RUN_CMD=`$DM_EXEC $mode $tshld | $AWK_EXEC`
		echo "$mode;$tshld;" "$RUN_CMD"
	done
done

#Experiments for AVG_CHI have to be done with different thresholds:
declare -a thresholds_CHIAVG=(
0.0
0.2
0.3
0.1
0.01
0.001
0.00001
0.0001
0.0000001
0.000000001
0.00000000001
0.0000000000001
0.000000000000001
0.0000000000000001
0.00000000000000001
0.0000000000000000001
0.000000000000000000001
0.00000000000000000000001
0.0000000000000000000000001
0.000000000000000000000000001
0.00000000000000000000000000001
0.0000000000000000000000000000001)

mode="-CHI_AVG -CHI "

for tshld in "${thresholds_CHIAVG[@]}"
do
	RUN_CMD=`$DM_EXEC $mode $tshld | $AWK_EXEC`
	echo "$mode;$tshld;" "$RUN_CMD"
done

