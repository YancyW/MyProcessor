#!/bin/bash
if [[  $# != 2 ]]  ; then
	echo "usage: ./run_all.sh code/no subjob/no) "
	exit
fi
code=$1
sub=$2



mass_profile=(10 30 50 70 90 115 125)
mass_length=${#mass_profile[@]}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
cd ${DIR}

if [[ $1 == "code" ]] ; then
	./action.sh
	wait

fi
for (( j=0; j<$(( $mass_length)); j++ )) 
do
	mass=${mass_profile[j]}

	if [[ ! -d ../results_${mass} ]]; then
		mkdir ../results_${mass}
	fi


	if [[ ${mass} != 125 ]];then
		#class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "nh" "2f")
		class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" )
	else
		#class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "higgs" "2f")
		class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" )
	fi
	classes_length=${#class_names[@]}


	for (( i=0; i<$(( $classes_length )); i++ )) 
	do
		class_name=${class_names[i]}
		if [[ ${class_name} = "higgs" ]] ; then
			final_states=( "e2" )
		elif [[ ${class_name} = "nh" ]] ; then
			final_states=( ${mass} )
		elif [[ ${class_name} = "2f_z" ]] ; then
			#final_states=("l" "h" "bha")
			final_states=("l" "bha")
		else
			#final_states=("l" "sl" "h")
			final_states=("l" "sl" )
		fi	

		fs_length=${#final_states[@]}
		for (( k=0; k<$(( $fs_length)); k++ )) 
		do
			final_state=${final_states[k]}

			echo "--------${class_name} ${final_state} ${mass}  -------"

			if [[ ${sub} == "subjob" ]] ; then
				qsub ./channel.sub ${class_name} ${final_state} ${code}  "no"  ${mass}
				sleep 5
			else
				./channel.sh ${class_name} ${final_state} ${code}  "no"  ${mass}
				sleep 5
			fi
		done
	done
done

