#!/bin/bash
if [[  $# != 0 ]]  ; then
	echo "usage: ./xection.sh  "
	exit
fi

pol_profile=("lr" "rl" "ll" "rr")
pol_length=${#pol_profile[@]}
#mass_profile=(10 30 50 70 90 115)
mass_profile=(10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 105 110 115 120)
mass_length=${#mass_profile[@]}


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

for (( k=0; k<$(( $mass_length)); k++ )) 
do
	mass=${mass_profile[k]}
	data_direction=/afs/desy.de/user/y/ywang/dust/light_higgs_10_120_1807/results_${mass};

	if [[ ${mass} != 125 ]]; then
		class_names=("nh" "4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "aa_2f" "2f_z" "higgs")
	else
		class_names=("higgs" "4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "2f_z" "aa_2f")
	fi
	classes_length=${#class_names[@]}

	echo "root_file_${mass}:"
	for (( i=0; i<$(( $classes_length )); i++ ))
	do
		class_name=${class_names[i]}




		if [[ ${class_name} = "higgs" ]] ; then
			final_states=( "e2" )
		elif [[ ${class_name} = "nh" ]] ; then
			final_states=( ${mass} )
		elif [[ ${class_name} = "2f_z" ]] ; then
			final_states=("l" "bha" "h")
		elif [[ ${class_name} = "aa_2f" ]] ; then
			final_states=("ee" "ll" "xx" "yy" )
		elif [[ ${class_name} = "aa_minijet" ]] ; then
			final_states=("min0410m1" "min1020m1" "min2040m1" "min40xxm1" "min0410m4" "min1020m4" "min2040m4" "min40xxm4" )
		else
			final_states=("l" "sl" "h")
		fi	

		fs_length=${#final_states[@]}
		for (( l=0; l<$(( $fs_length)); l++ )) 
		do
			final_state=${final_states[l]}

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
			elif [[ ${class_name} == "aa_2f" ]] ; then
				short_class_name="aa_{2f}^{$final_state}"
			elif [[ ${class_name} == "aa_minijet" ]] ; then
				short_class_name="aa_{minijet}^{$final_state}"
			fi

			exist=0
			echo "    ${short_class_name}:"
			for (( j=0; j<$(( $pol_length )); j++ ))
			do
				pol_in=${pol_profile[j]}
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
					root_name=${data_direction}/${class_name}_${final_state}_${polarization}.root 
					echo "        ${root_name}  :  ${pol_weight_aux} "
					exist=$(( exist + 1  ))
				fi
			done
			if [[ $exist == 0  ]];then
				echo "        no such class:  0 "
			fi
		done
	done
done
