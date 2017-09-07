#!/bin/bash
if [[  $# != 3 ]]  ; then
	echo "usage: ./analysis.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) "
	exit
fi

class_name=${1,,}
final_state=${2,,}
mass=${3,,}



DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

data_direction=${DIR}/../results_${mass}
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

outfile=" \"${DIR}/../data_${mass}/${class_name}_${final_state}_${mass}.dat\" "
cd ${macro_direction} 

if [[ ! -d "../data_${mass}" ]] ; then
	mkdir "../data_${mass}"
fi

if [[ ! -d "../plot_${mass}" ]] ; then
	mkdir "../plot_${mass}"
fi

if [[ ${class_name} == "4f_ww" ]] ; then
	short_class_name="\"4f_{ww}^${final_state}\""
elif [[ ${class_name} == "4f_zz" ]] ; then
	short_class_name="\"4f_{zz}^${final_state}\""
elif [[ ${class_name} == "4f_zzww" || ${class_name} == "4f_zzwwmix" ]] ; then
	short_class_name="\"4f_{zzww}^${final_state}\""
elif [[ ${class_name} == "4f_szee" || ${class_name} == "4f_singlezee" ]] ; then
	short_class_name="\"4f_{sze}^${final_state}\""
elif [[ ${class_name} == "4f_sznn" || ${class_name} == "4f_singleznunu" ]] ; then
	short_class_name="\"4f_{sznu}^${final_state}\""
elif [[ ${class_name} == "4f_szsw" || ${class_name} == "4f_singlezsinglewmix" ]] ; then
	short_class_name="\"4f_{szsw}^${final_state}\""
elif [[ ${class_name} == "4f_sw" || ${class_name} == "4f_singlew" ]] ; then
	short_class_name="\"4f_{sw}^${final_state}\""
elif [[ ${class_name} == "2f_z" ]] ; then
	short_class_name="\"2f_{z}^${final_state}\""
elif [[ ${class_name} == "higgs" ]] ; then
	short_class_name="\"h^${final_state}\""
elif [[ ${class_name} == "nh" || ${class_name} == "np-light-higgs" ]] ; then
	short_class_name="\"nh_{$mass}\""
elif [[ ${class_name} == "aa" ]] ; then
	short_class_name="\"aa^${final_state}\""
fi

if [[ -a analysis_total.C ]] ; then
root -l -q  analysis_total.C"(${file},"${short_class_name}",${weight},${outfile},${mass})"
fi
