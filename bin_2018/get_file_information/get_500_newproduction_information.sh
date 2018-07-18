#!/bin/bash

if [[ $# != 3 ]] ; then
	echo "usage: ./get_500_newproduction_information input_class_name (higgs/4f_ww/4f_zz/2f_z..) final_state (leptonic/hadronic/e2/e1..) pol_in (LR/lr/RL...) "
	exit
fi


class_name=$1
final_state=$2
pol_in=$3


energy=500
ILD_version=ILD_l5_o1_v02
reconstruction_version=v02-00-01
simulation_version=v02-00-01



./get_500_newproduction_file_name.sh ${energy} ${class_name} ${final_state} ${pol_in} ${ILD_version} ${reconstruction_version} ${simulation_version} 
