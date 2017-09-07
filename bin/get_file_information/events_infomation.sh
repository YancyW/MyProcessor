#!/bin/zsh
#if [[  $# != 4   &&   $# !=3  ]] ; then
if [[  $# == 4 ]]  ; then
	pol=$4
elif [[  "$3" == "file"       &&  $# == 3  ]] ; then
elif [[  "$3" == "info"       &&  $# == 3  ]] ; then
elif [[  "$3" == "info_num"   &&  $# == 3  ]] ; then
else
	echo "usage: ./events input_class_name (all/2f/4f/higgs/4f_WW/4f_zz..) final_state (l/sl/h/lep/e2/e1..) info_sort (file/info/info_num) [pol (l/r)---not necessary for info_sort=file] "
	exit
fi

input_class_name=$1
final_state=$2
info_sort=$3


four=("4f_WW" "4f_ZZ" "4f_ZZWWMix" "4f_singleZee" "4f_singleZnunu" "4f_singleZsingleWMix" "4f_singleW")
two=("2f_Z") 
higgs=("higgs") 
nh=("nh") 
all=("4f_WW" "4f_ZZ" "4f_ZZWWMix" "4f_singleZee" "4f_singleZnunu" "4f_singleZsingleWMix" "4f_singleW" "2f_Z" "higgs" "nh")


if [[ ${info_sort} !=  "file" ]] ; then
	if [[ ${pol} == "l" ]] ; then
		elec_pol=-0.8
		posi_pol=0.3
	elif [[ ${pol} == "r" ]] ; then
		elec_pol=0.8
		posi_pol=-0.3
	else
		echo "wrong input: beam polarization."
	fi
fi

if [[ ${input_class_name} == "4f" ]] ; then
	class_name=("${four[@]}")
elif [[ ${input_class_name} == "2f" ]] ; then
	class_name=("${two[@]}")
elif [[ ${input_class_name} == "all" ]] ; then
	class_name=("${all[@]}")
elif [[ ${input_class_name} == "higgs" ]] ; then
	class_name=("${higgs[@]}")
elif [[ ${input_class_name} == "nh" ]] ; then
	class_name=("${nh[@]}")
else
	class_name=("${input_class_name}")
fi


tot_xection=0
if [[ ${info_sort} == "info_num" ]] ; then
	tot_class_num=0
	lr_class_num=0
	rl_class_num=0
	ll_class_num=0
	rr_class_num=0
fi
class_length=${#class_name[@]}

for (( i=1; i<$(( $class_length +1)); i++ )) 
do
	if [[ ${info_sort} ==  "file" ]] ; then
		echo "class name:  "${class_name[i]}
		info=" ./get_final_states_information.sh ${class_name[i]} ${final_state} ${info_sort} "
		eval $info
		echo "end of class:  "${class_name[i]}
	elif [[ ${info_sort} == "info" || ${info_sort} == "info_num" ]] ; then
		info=" ./get_final_states_information.sh ${class_name[i]} ${final_state} ${info_sort} ${elec_pol}  ${posi_pol} "
		eval $info
		anverage_xection=` eval $info | grep "the class cross section is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		if [[ ! -z $anverage_xection ]] ; then
			(( tot_xection= tot_xection + anverage_xection ))
			if [[ ${info_sort} == "info_num" ]] ; then
				class_num=` eval $info | grep "the class events num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num1=` eval $info | grep "the class lr num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num2=` eval $info | grep "the class rl num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num3=` eval $info | grep "the class ll num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
				class_num4=` eval $info | grep "the class rr num is" | cut -d ":" -f2 | sed -e "s/ //g"  `
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

echo "the total cross section is: "${tot_xection}
if [[ ${info_sort} == "info_num" ]] ; then
	echo "the total lr num is:        "${lr_class_num}
	echo "the total rl num is:        "${rl_class_num}
	echo "the total ll num is:        "${ll_class_num}
	echo "the total rr num is:        "${rr_class_num}
	echo "the total events num is:    "${tot_class_num}
fi
