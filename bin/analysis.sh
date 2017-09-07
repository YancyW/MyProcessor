#!/bin/bash
if [[  $# != 2 ]]  ; then
	echo "usage: ./analysis.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) "
	exit
fi

class_name=${1,,}
final_state=${2,,}



DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

data_direction=${DIR}/../results
macro_direction=${DIR}/../macro

pol_profile=("lr" "rl" "ll" "rr")
pol_length=${#pol_profile[@]}
file_name=()
pol_weight=()

for (( i=0; i<$(( $pol_length )); i++ )) 
do
	pol_in=${pol_profile[$i]}

	if [[ ${pol_in} == "lr" ]] ; then
		polarization="eL.pR"
	elif [[ ${pol_in} == "rl" ]] ; then
		polarization="eR.pL"
	elif [[ ${pol_in} == "ll" ]] ; then
		polarization="eL.pL"
	elif [[ ${pol_in} == "rr" ]] ; then
		polarization="eR.pR"
	fi

	pol_weight_aux=( ` eval " ${DIR}/get_weight.sh  ${class_name} ${final_state} ${pol_in} weight " ` )

	if [[ ! -z ${pol_weight_aux} ]] ; then
		file_name+=( \"${data_direction}/${class_name}_${final_state}_${polarization}.root\" )
		file_name+=( "," )
		pol_weight+=( \"${pol_weight_aux}\" )
		pol_weight+=( "," )
	fi
done


file_length=${#file_name[@]}
if [[ ${file_length} == 0 ]] ; then
	echo "wrong input parameter!"
	exit
fi


unset file_name[${#file_name[@]}-1]
unset pol_weight[${#pol_weight[@]}-1]


file_length=${#file_name[@]}


for (( i=0; i<$(( $file_length )); i++ )) 
do
	names=` echo ${file_name[i]} | sed -e "s/\"//g " `
	if [ -a ${names} ]; then
		i=$(( i+1 ))
	else
		echo "no such file: "${names}
		exit
	fi
done

file=" $( IFS=$'\n'; echo "{" ; echo "${file_name[*]}" ; echo "}") "
weight=" $( IFS=$'\n'; echo "{" ; echo "${pol_weight[*]}" ; echo "}") "

outfile=" \"${DIR}/../data/${class_name}_${final_state}.dat\" "
cd ${macro_direction} 

if [[ -a analysis.C ]] ; then
root -l -q  analysis.C"(${file},${weight},${outfile})"
fi
