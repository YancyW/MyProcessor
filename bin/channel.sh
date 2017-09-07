#!/bin/bash
if [[ $1 == "nh"  && $# == 4 ]] ; then
	echo "begin to cal events!" 
	input_class_name=${1,,}
	info=${2,,}
	mode=${3,,}
	mass=${4,,}
elif [[ $1 == "higgs"  && $# == 4 ]] ; then
	echo "begin to cal events!" 
	input_class_name=${1,,}
	info=${2,,}
	mode=${3,,}
	mass=${4,,}
elif [[ $1 == "2f"  && $# == 4 ]] ; then
	echo "begin to cal events!" 
	input_class_name=${1,,}
	info=${2,,}
	mode=${3,,}
	mass=${4,,}
elif [[ $1 == "4f"  && $# == 4 ]] ; then
	echo "begin to cal events!" 
	input_class_name=${1,,}
	info=${2,,}
	mode=${3,,}
	mass=${4,,}
elif [[ $# == 4 ]] ; then
	echo "begin to cal events!" 
	input_class_name=${1,,}
	info=${2,,}
	mode=${3,,}
	mass=${4,,}
elif [[ $# == 5 ]] ; then
	echo "begin to cal events!" 
	input_class_name=${1,,}
	input_final_state=${2,,}
	info=${3,,}
	mode=${4,,}
	mass=${5,,}
else
	echo "usage: ./run_channel class_name final_state info mode mass" 
	exit
fi



DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
cd ${DIR}



if [[ ${input_class_name} = "higgs" ]] ; then
	class_name_array="higgs"
	input_final_state=("e2")
elif [[ ${input_class_name} = "nh" ]] ; then
	class_name_array="nh"
	input_final_state=(${mass})
elif [[ ${input_class_name} = "2f" ]] ; then
	class_name_array="2f_z"
	input_final_state=("l" "bha")
elif [[ ${input_class_name} = "4f" ]] ; then
	class_name_array=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw")
	input_final_state=("l","sl")
else
	class_name_array=${input_class_name}
	input_final_state=("l","sl")
fi	

pol_profile=("lr" "rl" "ll" "rr")
pol_length=${#pol_profile[@]}

final_state_length=${#input_final_state[@]}
class_name_length=${#class_name_array[@]}

echo ""
echo "has setting all variables!"
echo ""

xml_direction=${DIR}/../xml
if [[ $info == "code" ]] ; then
	echo "begin to generate samples!"
	echo ""
	for (( k=0; k<$(( $class_name_length)); k++ ))
	do
		class_name=${class_name_array[k]}
		for (( i=0; i<$(( $final_state_length )); i++ ))
		do
			final_state=${input_final_state[i]}
			for (( j=0; j<$(( $pol_length )); j++ ))
			do
				pol=${pol_profile[j]}
				NewXml=${class_name}_${final_state}_${pol}_${mass}.xml
				echo "generate samples for: "${NewXml} 
				echo ""
				${DIR}/set_xml.sh ${class_name} ${final_state} ${pol} ${mass}
				if [[ -a ${xml_direction}/${NewXml} ]] ; then
					${DIR}/run.sh ${NewXml} 
				fi
			done
		done
	done
fi




echo "begin to analysis for each pure polarization"
echo ""


for (( k=0; k<$(( $class_name_length)); k++ ))
do
	class_name=${class_name_array[k]}
	if [[ $mode == "rc" ]] ; then
		ana_content=` eval ${DIR}/analysis_rc.sh ${class_name} ${input_final_state} `
		judge=` echo ${ana_content} | grep "no such file" `
		if [[ "x" != "x${judge}" ]] ;then
			echo ${ana_content}
			exit
		fi
		echo "begin to cal for real beam polarization"
		echo ""
		${DIR}/cal_rc.sh ${class_name} ${input_final_state[@]} 
	elif [[ $mode == "recovery" ]] ; then
		ana_content=` eval ${DIR}/analysis_recovery.sh ${class_name} ${input_final_state} `
		judge=` echo ${ana_content} | grep "no such file" `
		if [[ "x" != "x${judge}" ]] ;then
			echo ${ana_content}
			exit
		fi
		echo "begin to cal for real beam polarization"
		echo ""
		${DIR}/cal_recovery.sh ${class_name} ${input_final_state[@]} 
	else
		for (( i=0; i<$final_state_length; i++ ))
		do
			final_state=${input_final_state[i]}
			echo "dealing with: " ${class_name} ${final_state} ${mass} 

			ana_content=` eval ${DIR}/analysis_record.sh ${class_name} ${final_state} ${mass} `
			judge=` echo ${ana_content} | grep "no such file" `
			if [[ "x" != "x${judge}" ]] ;then
				echo ${ana_content}
				exit
			fi
		done

	fi
done
