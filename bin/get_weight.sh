#!/bin/bash
if [[  $# != 4 ]]  ; then
	echo "usage: ./get_weight.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) pol (lr/rl/ll/rr) content (file/weight)"
	exit
fi

if [[  $4 != "file" && $4 != "weight" ]]  ; then
	echo "usage: ./get_weight.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) pol (lr/rl/ll/rr) content (file/weight)"
	exit
fi

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

input_class_name=${1,,}
input_final_state=${2,,}
pol=${3,,}
content=${4,,}

if [[ ${input_class_name} = "higgs" ]] ; then
	class_name="higgs"
elif [[ ${input_class_name} = "nh" ]] ; then
	class_name="nh"
elif [[ ${input_class_name} = "2f_z" ]] ; then
	class_name="2f_Z"
elif [[ ${input_class_name} == "4f_ww" ]] ; then
	class_name="4f_WW"
elif [[ ${input_class_name} == "4f_zz" ]] ; then
	class_name="4f_ZZ"
elif [[ ${input_class_name} == "4f_zzww" || ${input_class_name} == "4f_zzwwmix" ]] ; then
	class_name="4f_ZZWWMix"
elif [[ ${input_class_name} == "4f_szee" || ${input_class_name} == "4f_singlezee" ]] ; then
	class_name="4f_singleZee"
elif [[ ${input_class_name} == "4f_sznn" || ${input_class_name} == "4f_singleznunu" ]] ; then
	class_name="4f_singleZnunu"
elif [[ ${input_class_name} == "4f_szsw" || ${input_class_name} == "4f_singlezsinglewmix" ]] ; then
	class_name="4f_singleZsingleWMix"
elif [[ ${input_class_name} == "4f_sw" || ${input_class_name} == "4f_singlew" ]] ; then
	class_name="4f_singleW"
fi


if [[ ${input_final_state} = "l" ]] ; then
	final_states=("leptonic")
elif [[ ${input_final_state} = "bha" ]] ; then
	final_states=("bhabhag")
elif [[ ${input_final_state} = "sl" ]] ; then
	final_states=("semileptonic")
elif [[ ${input_final_state} = "h" ]] ; then
	final_states=("hadronic")
else
	final_states=("${input_final_state}")
fi


direction=${DIR}
if [[ -a ${direction}/all_events.dat ]] ; then
	file_name=${direction}/all_events.dat
else
	echo "no all_events.dat in the folder."${direction}
fi

#class_content=" sed -n -e \"/^class_name:  ${class_name}$/,/^end of class:  ${class_name}$/ { /^class name:  ${class_name}$/d; /^end of class:  ${class_name}$/d; p;} \"  "
class_content=" sed -n \"/^class name:\ \ ${class_name}$/,/^end of class:\ \ ${class_name}$/p\"  "
final_state_content=" sed -n \"/^final_state name:\ \ ${final_states}$/,/^end of final_state:\ \ ${final_states}$/p\"  "
pol_content=" sed -n \"/^pol:\ \ ${pol}$/,/^end of pol:\ \ ${pol}$/p\"  "
file_content=" sed -n -e \"/^file:\ \ ${class_name}$/,/^end of file:\ \ ${class_name}$/ { /^file:\ \ ${class_name}$/d; /^end of file:\ \ ${class_name}$/d; p; } \"  "
weight_content=" sed -n -e \"/^weight:\ \ ${class_name}$/,/^end of weight:\ \ ${class_name}$/ { /^weight:\ \ ${class_name}$/d; /^end of weight:\ \ ${class_name}$/d; p; } \" " 
xection_content=" sed -n -e \"/^xection:\ \ ${class_name}$/,/^end of xection:\ \ ${class_name}$/ { /^xection:\ \ ${class_name}$/d; /^end of xection:\ \ ${class_name}$/d; p; } \" " 
if [[  ${content} == "file" ]]  ; then
	eval ${class_content} ${file_name} | eval ${final_state_content} | eval ${pol_content} | eval ${file_content}
elif [[  ${content} == "weight" ]]  ; then
	eval ${class_content} ${file_name} | eval ${final_state_content} | eval ${pol_content} | eval ${xection_content}
else
	echo "wrong for the 4th argument."
fi
#eval ${pol_content}

