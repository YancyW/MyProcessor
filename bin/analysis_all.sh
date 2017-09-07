#!/bin/bash

if [[  $# != 3 ]]  ; then
	echo "usage: ./analysis_all.sh input_class_name (all/2f/4f/higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/lep/e2/e1..) "
	exit
fi

input_class_name=${1,,}
final_state=${2,,}
mass=${3,,}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

four=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw")
two=("2f_z") 
all=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "2f_z" "nh_${mass}")
higgs=("higgs") 
nh=("nh") 

if [[ ${input_class_name} == "4f" ]] ; then
	class_names=("${four[@]}")
elif [[ ${input_class_name} == "2f" ]] ; then
	class_names=("${two[@]}")
elif [[ ${input_class_name} == "all" ]] ; then
	class_names=("${all[@]}")
elif [[ ${input_class_name} == "higgs" ]] ; then
	class_names=("${higgs[@]}")
else
	class_names=("${input_class_name}")
fi


classes_length=${#class_names[@]}
for (( i=0; i<$(( $classes_length )); i++ )) 
do
	class_name=${class_names[i]}
	${DIR}/analysis_mass.sh ${class_name} ${final_state}  ${mass}
done
