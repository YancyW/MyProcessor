#!/bin/bash



if [[  $# == 2 ]]  ; then
	input_class_name=${1,,}
	input_final_state=${2,,}
elif [[ $# == 3 && $1 == "2f_z" ]] ; then
	input_final_state2=${3,,}
else
	echo "usage: ./analysis.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) [final state 2 (only for 2f_Z channel)] "
	exit
fi



DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
input_direction=${DIR}/../data/
out_name=${input_direction}/${input_class_name}_rc.dat



if [[ $# == 2 ]] ; then
	cal_name=${input_direction}/${input_class_name}_${input_final_state}_rc.dat
	if [[ -a ${cal_name} ]] ; then
		${DIR}/cal_event ${cal_name}  ${out_name}
		echo "done"
	else
		echo "no such file ${input_direction}/higgs_e2.dat"
		exit
	fi
elif [[ $# == 3 ]] ; then
	cal_name=${input_direction}/${input_class_name}_${input_final_state}_rc.dat
	cal_name2=${input_direction}/${input_class_name}_${input_final_state2}_rc.dat
	if [[ -a ${cal_name} && -a ${cal_name2} ]] ; then
		${DIR}/cal_event ${cal_name} ${cal_name2} ${out_name}
		echo "done"
	else
		echo "no such file ${input_direction}/higgs_e2.dat"
		exit
	fi
fi


