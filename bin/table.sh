#!/bin/bash

if [[  $# != 1 ]]  ; then
	echo "usage: ./table.sh  mass_value(10/30/50/70/90/115/125/all)"
	exit
fi

if [[ $1 == "all" ]] ; then
	mass_profile=(10 30 50 70 90 115)

else
	mass_profile=($1)
fi


out_file=out.dat 

mass_length=${#mass_profile[@]}

if [[ -a ${out_file} ]] ; then
	rm ${out_file}
fi

for (( j=0; j<$(( $mass_length)); j++ )) 
do
	mass=${mass_profile[j]}
	data_folder="../data_${mass}/"
	final_state=("_l_${mass}.dat")
	final_state2=("_sl_${mass}.dat")
	class_names=("${data_folder}nh_${mass}_${mass}.dat" "${data_folder}4f_ww${final_state}" "${data_folder}4f_zz${final_state}" "${data_folder}4f_zzww${final_state}" "${data_folder}4f_szee${final_state}" "${data_folder}4f_sznn${final_state}" "${data_folder}4f_szsw${final_state}" "${data_folder}4f_sw${final_state}" "${data_folder}2f_z${final_state}" "${data_folder}2f_z_bha_${mass}.dat" "${data_folder}4f_ww${final_state2}" "${data_folder}4f_zz${final_state2}"  "${data_folder}4f_szee${final_state2}" "${data_folder}4f_sznn${final_state2}" "${data_folder}4f_sw${final_state2}")
	#class_names=("${data_folder}nh_${mass}_${mass}.dat" "${data_folder}4f_ww${final_state}" "${data_folder}4f_zz${final_state}" "${data_folder}4f_zzww${final_state}" "${data_folder}4f_szee${final_state}" "${data_folder}4f_sznn${final_state}" "${data_folder}4f_szsw${final_state}" "${data_folder}4f_sw${final_state}" "${data_folder}2f_z${final_state}" "${data_folder}2f_z_bha_${mass}.dat" )
	#class_names=("${data_folder}nh_${mass}_${mass}.dat" "${data_folder}4f_ww${final_state}")

	echo ${class_names[@]}
	echo ${out_file}

	#./make_table ${class_names[@]} ${out_file} true 

done
