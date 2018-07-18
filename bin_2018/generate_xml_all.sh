#!/bin/bash
class_names=("4f_WW" "4f_ZZ" "4f_ZZWWMix" "4f_singleZee" "4f_singleZnunu" "4f_singleZsingleWMix" "4f_singleW" "2f_Z" "higgs")
classes_length=${#class_names[@]}
pol_profile=("lr" "rl" "ll" "rr")
pol_length=${#pol_profile[@]}
mass_profile=(10 30 50 70 90 115)
mass_length=${#mass_profile[@]}


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

xml_direction=${DIR}/../xml_isr

for (( k=0; k<$(( $mass_length)); k++ )) 
do
	mass=${mass_profile[j]}
	for (( i=0; i<$(( $classes_length )); i++ ))
	do
		class_name=${class_names[i]}
		if [[ ${class_name} = "higgs" ]] ; then
			final_states=( "e2" )
		elif [[ ${class_name} = "nh" ]] ; then
			final_states=( ${mass} )
		elif [[ ${class_name} = "2f_Z" ]] ; then
			final_states=("l" "h" "bha")
		else
			final_states=("l" "sl" "h")
		fi	

		for (( j=0; j<$(( $pol_length )); j++ ))
		do
			pol_in=${pol_profile[$j]}
			NewXml=${class_name}_${final_state}_${pol_in}_${mass}.xml
			xml_file=${xml_direction}/${NewXml}
			${DIR}/set_xml.sh ${class_name} ${final_state} ${pol} ${mass}
			wait
		done
	done
done
