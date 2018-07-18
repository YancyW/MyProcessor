#!/bin/bash
if [[  $# != 2 ]]  ; then
	echo "usage: ./run_all.sh code/no subjob/no) "
	exit
fi
code=$1
subjob=$2



mass_profile=(10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100 105 110 115 120)
#mass_profile=(45 50 55 60 65 70 75 80 85 90 95 100 105 110 115 120)
mass_length=${#mass_profile[@]}

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"
cd ${DIR}
echo "-------- enter ${DIR} -------"


for (( j=0; j<$(( $mass_length)); j++ )) 
do
	mass=${mass_profile[j]}

	if [[ $1 == "code" ]] ; then
		./action.sh
		wait
	fi

	if [[ ! -d  /afs/desy.de/user/y/ywang/dust/light_higgs_10_120_1807/results_${mass} ]]; then
	    mkdir /afs/desy.de/user/y/ywang/dust/light_higgs_10_120_1807/results_${mass};
	fi


	if [[ ${mass} != 125 ]];then
		class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "nh" "2f" "higgs" "aa_2f" )
	else
		class_names=("4f_ww" "4f_zz" "4f_zzww" "4f_szee" "4f_sznn" "4f_szsw" "4f_sw" "higgs" "2f" "aa_2f")
	fi
	classes_length=${#class_names[@]}


	for (( i=0; i<$(( $classes_length )); i++ )) 
	do
		class_name=${class_names[i]}

		echo "--------${class_name} ${mass}  -------"

		./channel.sh ${class_name} ${subjob}  ${mass}
		sleep 1 
	done
done

