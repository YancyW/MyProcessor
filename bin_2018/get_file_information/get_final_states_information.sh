#!/bin/zsh

if [[  $# == 5 ]]  ; then
	elec_pol=$4
	posi_pol=$5
elif [[  "$3" == "file"       &&  $# == 3  ]] ; then
elif [[  "$3" == "info"       &&  $# == 3  ]] ; then
elif [[  "$3" == "info_num"   &&  $# == 3  ]] ; then
else
	echo "usage: ./get_final_states_infomation input_class_name (all/2f/4f/higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/lep/e2/e1..) info_sort (file/info/info_num) [elec pol posi_pol (number between -1 and 1 )---not necessary for info_sort=file] "
	exit
fi


input_class_name=${1}
input_final_state=${2}
info_sort=${3}




if [[ ${input_class_name} = "higgs" ]] ; then
	if [[ ${input_final_state} = "all" ]] ; then
		final_states=("e1" "e2" "e3" "n" "q" "f")
	else
		final_states=${input_final_state}
	fi
elif [[ ${input_class_name} = "nh" ]] ; then
	if [[ ${input_final_state} = "all" ]] ; then
		final_states=("10" "15" "20" "25" "30" "35" "40" "45" "50" "55" "60" "65" "70" "75" "80" "85" "90" "95" "100" "105" "110" "115" "120")
	else
		final_states=${input_final_state}
	fi
elif [[ ${input_class_name} = "2f_Z" ]] ; then
	if [[ ${input_final_state} = "all" ]] ; then
		final_states=("leptonic" "semileptonic" "hadronic" "bhabhag")
	elif [[ ${input_final_state} = "lep" ]] ; then
		final_states=("leptonic" "bhabhag")
	elif [[ ${input_final_state} = "l" ]] ; then
		final_states=("leptonic")
	elif [[ ${input_final_state} = "sl" ]] ; then
		final_states=("semileptonic")
	elif [[ ${input_final_state} = "h" ]] ; then
		final_states=("hadronic")
	else
		final_states=${input_final_state}
	fi
elif [[ ${input_class_name} = "aa_2f" ]] ; then
	if [[ ${input_final_state} = "all" ]] ; then
		final_states=("ee" "ll" "xx" "yy" )
	elif [[ ${input_final_state} = "2f" ]] ; then
		final_states=("ee" "ll" "xx" "yy")
	else
		final_states=${input_final_state}
	fi
elif [[ ${input_class_name} = "aa_minijet" ]] ; then
	if [[ ${input_final_state} = "all" ]] ; then
		final_states=("min0410m1" "min1020m1" "min2040m1" "min40xxm1" "min0410m4" "min1020m4" "min2040m4" "min40xxm4" )
	elif [[ ${input_final_state} = "minijet" ]] ; then
		final_states=("min0410m1" "min1020m1" "min2040m1" "min40xxm1" "min0410m4" "min1020m4" "min2040m4" "min40xxm4")
	else
		final_states=${input_final_state}
	fi
else
	if [[ ${input_final_state} = "all" ]] ; then
		final_states=("leptonic" "semileptonic" "hadronic")
	elif [[ ${input_final_state} = "l" ]] ; then
		final_states=("leptonic")
	elif [[ ${input_final_state} = "sl" ]] ; then
		final_states=("semileptonic")
	elif [[ ${input_final_state} = "h" ]] ; then
		final_states=("hadronic")
	else
		final_states=("${input_final_state}")
	fi
fi	


class_name=${input_class_name}


tot_xection=0
if [[ ${info_sort} == "info_num" ]] ; then
	tot_class_num=0
	lr_class_num=0
	rl_class_num=0
	ll_class_num=0
	rr_class_num=0
fi

for j in "${final_states[@]}"
do
	final_state=${j}

	if [[ ${info_sort} ==  "file" ]] ; then
		echo "final_state name:  "${final_state}
		info=" ./get_all_polarization.sh ${class_name} ${final_state} ${info_sort} "
		eval $info
		echo "end of final_state:  "${final_state}
	elif [[ ${info_sort} == "info" || ${info_sort} == "info_num" ]] ; then
		info=" ./get_all_polarization.sh ${class_name} ${final_state} ${info_sort} ${elec_pol} ${posi_pol} "
		eval $info

		anverage_xection=` eval $info | grep "anverage xection is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		if [[ ! -z $anverage_xection ]] ; then
			(( tot_xection= tot_xection + anverage_xection ))
			if [[ ${info_sort} == "info_num" ]] ; then
				class_num=` eval $info | grep "the tot num for all pol is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num1=` eval $info | grep "the pol lr num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num2=` eval $info | grep "the pol rl num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num3=` eval $info | grep "the pol ll num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num4=` eval $info | grep "the pol rr num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				(( tot_class_num= tot_class_num + class_num))
				(( lr_class_num= lr_class_num + class_num1))
				(( rl_class_num= rl_class_num + class_num2))
				(( ll_class_num= ll_class_num + class_num3))
				(( rr_class_num= rr_class_num + class_num4))
			fi
		fi
	fi
	echo 
done


echo "the class cross section is: "${tot_xection}
if [[ ${info_sort} == "info_num" ]] ; then
	echo "the class lr num is:        "${lr_class_num}
	echo "the class rl num is:        "${rl_class_num}
	echo "the class ll num is:        "${ll_class_num}
	echo "the class rr num is:        "${rr_class_num}
	echo "the class events num is:    "${tot_class_num}
fi
