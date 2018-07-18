#!/bin/bash
if [[  $# != 0 ]]  ; then
	echo "usage: ./cut.sh ) "
	exit
fi

mass=10

class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "nh" "2f_z")
classes_length=${#class_names[@]}

pol_profile=("lr" "rl" "ll" "rr")
pol_length=${#pol_profile[@]}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"


data_direction=${DIR}/../results_${mass}
macro_direction=${DIR}/../macro

all_file_name=()
all_pol_weight=()
all_short_name=()

echo "root_file:"
for (( i=0; i<$(( $classes_length )); i++ )) 
do

	class_name=${class_names[i]}
	if [[ ${class_name} = "nh" ]] ; then
		final_states=( ${mass} )
	elif [[ ${class_name} = "2f_z" ]] ; then
		final_states=("l" "bha")
	else
		final_states=("l" "sl" )
	fi	


	fs_length=${#final_states[@]}
	for (( k=0; k<$(( $fs_length)); k++ )) 
	do
		final_state=${final_states[k]}

		if [[ ${class_name} == "4f_ww" ]] ; then
			short_class_name="4f_{ww}^{$final_state}"
		elif [[ ${class_name} == "4f_zz" ]] ; then
			short_class_name="4f_{zz}^{$final_state}"
		elif [[ ${class_name} == "4f_zzww" || ${class_name} == "4f_zzwwmix" ]] ; then
			short_class_name="4f_{zzww}^{$final_state}"
		elif [[ ${class_name} == "4f_szee" || ${class_name} == "4f_singlezee" ]] ; then
			short_class_name="4f_{szee}^{$final_state}"
		elif [[ ${class_name} == "4f_sznn" || ${class_name} == "4f_singleznunu" ]] ; then
			short_class_name="4f_{sznn}^{$final_state}"
		elif [[ ${class_name} == "4f_szsw" || ${class_name} == "4f_singlezsinglewmix" ]] ; then
			short_class_name="4f_{szsw}^{$final_state}"
		elif [[ ${class_name} == "4f_sw" || ${class_name} == "4f_singlew" ]] ; then
			short_class_name="4f_{sw}^{$final_state}"
		elif [[ ${class_name} == "2f_z" ]] ; then
			short_class_name="2f_{z}^{$final_state}"
		elif [[ ${class_name} == "higgs" ]] ; then
			short_class_name="h_{125}^{$final_state}"
		elif [[ ${class_name} == "nh" || ${class_name} == "np-light-higgs" ]] ; then
			short_class_name="nh_{$mass}^{$mass}"
		elif [[ ${class_name} == "aa" ]] ; then
			short_class_name="aa_{}^{$final_state}"
		fi

		exist=0
		echo "     ${short_class_name} :"
		for (( j=0; j<$(( $pol_length )); j++ ))
		do
			pol=${pol_profile[j]}
			if [[ ${pol} == "lr" ]] ; then
				polarization="eL.pR"
			elif [[ ${pol} == "rl" ]] ; then
				polarization="eR.pL"
			elif [[ ${pol} == "ll" ]] ; then
				polarization="eL.pL"
			elif [[ ${pol} == "rr" ]] ; then
				polarization="eR.pR"
			fi

			pol_weight_aux=( ` eval " ${DIR}/get_weight.sh  ${class_name} ${final_state} ${pol} weight " ` )

			if [[ ! -z ${pol_weight_aux} ]] ; then
				file_name=( ${data_direction}/${class_name}_${final_state}_${polarization}.root )
				pol_weight=( ${pol_weight_aux} )
				echo "          ${file_name}  :  ${pol_weight}"
				exist=$(( exist+1 ))
			fi
		done

		if [[ ${exist} == 0 ]] ; then
			echo "          no such class: 0"
		fi
	done
done

