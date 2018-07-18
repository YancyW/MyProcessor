#!/bin/zsh
#this is zsh not bash !!!
#array count from 1 not 0
#can cal float directly

if [[  $# == 5 ]]  ; then
	elec_pol=$4
	posi_pol=$5
elif [[  "$3" == "file"       &&  $# == 3  ]] ; then
elif [[  "$3" == "info"       &&  $# == 3  ]] ; then
elif [[  "$3" == "info_num"   &&  $# == 3  ]] ; then
else
	echo "usage: ./get_all_polarization input_class_name (all/2f/4f/higgs/4f_WW/4f_ZZ..) final_state (l/sl/h/lep/e2/e1..) info_sort (file/info/info_num) [elec pol posi_pol (number between -1 and 1 )---not necessary for info_sort=file] "
	exit
fi

class_name=$1
final_state=$2
info_sort=$3

pol_profile=("lr" "rl" "ll" "rr")
pol_xecton=(0 0 0 0)
pol_num=(0 0 0 0)





#for i in "${pol_profile[@]}" 
k=0
pol_length=${#pol_profile[@]}
for (( i=1; i<$(( $pol_length+1 )); i++ )) 
do
	pol_in=${pol_profile[$i]}
	info=" ./get_250_dbd_information.sh  ${class_name} ${final_state} ${pol_in} "
	if [[ ${info_sort} == "file" ]] ; then
		echo ""
		echo "pol:  "${pol_in}
		xection=` eval $info | grep "xection is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		if [[ ! -z $xection ]] ; then
			file_name=` eval $info | grep "file_name" | cut -d ":" -f2 | sed -e "s/ //g"  `
			echo "file:  "${class_name}
			echo ${file_name}
			echo "end of file:  "${class_name}
			tot_filenum=` eval $info | grep "total_file_number" | cut -d ":" -f2 | sed -e "s/ //g"  `
			(( weight_of_each_number=xection / tot_filenum ))
			echo "weight:  "${class_name}
			echo ${weight_of_each_number}
			echo "end of weight:  "${class_name}
			echo "xection:  "${class_name}
			echo ${xection}
			echo "end of xection:  "${class_name}
		fi
		echo "end of pol:  "${pol_in}
	fi
	if [[ ${info_sort} == "info" || ${info_sort} == "info_num" ]] ; then
		channel_info=` eval $info | grep "channel is" | cut -d ":" -f2 `
		xection=` eval $info | grep "xection is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		xection_err=` eval $info | grep "xection err is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		luminosity=` eval $info | grep "luminosity is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		totnum=` eval $info | grep "totnum is" | cut -d ":" -f2 | sed -e "s/ //g"  `
		filenum=` eval $info | grep "file_number" | cut -d ":" -f2 | sed -e "s/ //g"  `
		tot_filenum=` eval $info | grep "total_file_number" | cut -d ":" -f2 | sed -e "s/ //g"  `


		if [[ ! -z $xection ]] ; then
			(( weight_of_each_number=xection / tot_filenum ))
			echo "channel is:      "${channel_info}
			echo "xection is:      "${xection}
			echo "xection err is:  "${xection_err}
			echo "luminosity is:   "${luminosity}
			echo "file number is:  "${filenum}
			echo "total file number is:  "${tot_filenum}
			echo "weight:  " ${weight_of_each_number}
			pol_xecton[$i]=$xection
			if [[ ${info_sort} == "info_num" ]] ; then
				pol_num[$i]=$totnum
			fi
			k=$((k+1))
		fi
	fi
done

if [[ ${info_sort} == "info" || ${info_sort} == "info_num" ]] ; then
	if [ ! $k -eq 0 ]; then
		elecm=$(( (1 - elec_pol)/2 ))
		elecp=$(( (1 + elec_pol)/2 ))
		posim=$(( (1 - posi_pol)/2 ))
		posip=$(( (1 + posi_pol)/2 ))
		anverage_xectionlr=$(( elecm * posip *   pol_xecton[1]  ))
		anverage_xectionrl=$(( elecp * posim *   pol_xecton[2]  ))
		anverage_xectionll=$(( elecm * posim *   pol_xecton[3]  ))
		anverage_xectionrr=$(( elecp * posip *   pol_xecton[4]  ))
		anverage_xection=$(( anverage_xectionlr + anverage_xectionrl + anverage_xectionll + anverage_xectionrr ))

		echo "the anverage xection is: "${anverage_xection} 
		if [[ ${info_sort} == "info_num" ]] ; then
			all_pol_num=$(( pol_num[1] + pol_num[2] + pol_num[3] + pol_num[4]))
			echo "the pol lr num is: "${pol_num[1]}
			echo "the pol rl num is: "${pol_num[2]}
			echo "the pol ll num is: "${pol_num[3]}
			echo "the pol rr num is: "${pol_num[4]}
			echo "the tot num for all pol is: "${all_pol_num} 
		fi

	fi
fi
