#!/bin/bash
if [[  $# != 4 ]]  ; then
	echo "usage: ./set_xml.sh input_class_name (higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/e2/e1..) pol (lr/rl/ll/rr) mass(10/../125)"
	exit
fi

class_name=${1,,}
final_state=${2,,}
pol_in=${3,,}
mass=${4,,}


DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

data_direction=${DIR}/../results_${mass}
xml_direction=${DIR}/../xml


template_file=${DIR}/template.xml
NewXml=${class_name}_${final_state}_${pol_in}_${mass}.xml
xml_file=${xml_direction}/${NewXml}



if [[ ${pol_in} == "lr" ]] ; then
	polarization="eL.pR"
elif [[ ${pol_in} == "rl" ]] ; then
	polarization="eR.pL"
elif [[ ${pol_in} == "ll" ]] ; then
	polarization="eL.pL"
elif [[ ${pol_in} == "rr" ]] ; then
	polarization="eR.pR"
fi

pol_weight_aux=( ` eval " ${DIR}/get_weight.sh  ${class_name} ${final_state} ${pol_in} weight " ` )

if [[ ! -z ${pol_weight_aux} ]] ; then
	cp ${template_file} ${xml_file}
	file=`  ${DIR}/get_weight.sh  ${class_name} ${final_state} ${pol_in} file  ` 
	root_name=${data_direction}/${class_name}_${final_state}_${polarization}.root 
	file_test=` echo -e ${file/'\n'} `
	sed -i  "61c\ <parameter name=\"hmass\" type=\"float\"> ${mass} </parameter>" ${xml_file}
	sed -i  "62c\ <parameter name=\"ProcessListFile\" type=\"string\"> ${class_name} </parameter>" ${xml_file}
	sed -i  "63c\ <parameter name=\"RootFileName\"> ${root_name} </parameter>" ${xml_file}
	sed -i  "11i\ ${file_test}" ${xml_file}
	echo "true"
else
	if [[ -f ${xml_file} ]] ; then
		rm ${xml_file}
	fi
	echo "false"
fi
