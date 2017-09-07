#!/bin/bash

if [[ $# != 3 ]] ; then
	echo "usage: ./get_250_dbd_information input_class_name (higgs/4f_ww/4f_zz/2f_z..) final_state (leptonic/hadronic/e2/e1..) pol_in (LR/lr/RL...) "
	exit
fi


class_name=$1
final_states=$2
pol_in=$3


energy=250
ILD_version=ILD_o1_v05
reconstruction_version=v01-16-p10
simulation_version=v01-14-01-p00



./get_file_name.sh ${energy} ${class_name} ${final_states} ${pol_in} ${ILD_version} ${reconstruction_version} ${simulation_version} 
