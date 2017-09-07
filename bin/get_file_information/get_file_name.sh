#!/bin/bash
#this script can get the slcio file name, and how many files and basic information about the channel; 

if [[ $# != 7 ]] ; then
	echo "usage: ./get_file_name.sh energy(250/350/500/1000) input_class_name (higgs/4f_WW/4f_ZZ/2f_Z..) final_state (leptonic/hadronic/e2/e1..) pol_in (LR/lr/RL...)  ILD_version, reconstruction_version,simulation_version "
	echo "recommand to use small letter for class name and final state name and polarization."
	exit
fi

energy=${1}
energy_version=${energy}-TDR_ws
input_class_name=${2,,}
input_final_states=${3,,}
pol_in=${4,,}
ILD_version=$5
reconstruction_version=$6_${energy}
simulation_version=$7





if [[  ${pol_in} == "lr" ]] ; then
	polarization="eL.pR"
elif [[ ${pol_in} == "rl" ]] ; then
	polarization="eR.pL"
elif [[ ${pol_in} == "ll" ]] ; then
	polarization="eL.pL"
elif [[ ${pol_in} == "rr" ]] ; then
	polarization="eR.pR"
else
	echo "no such input pol"
fi

if [[ ${input_class_name} == "4f_ww" ]] ; then
	class_name="4f_WW"
	short_class_name="4f_ww"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106581
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106582
		else
			exit
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106577
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106578
		else
			exit
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106551
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106552
		else
			exit
		fi
	else
		echo "no such 4f_ww final state"
		exit
	fi

elif [[ ${input_class_name} == "4f_zz" ]] ; then
	class_name="4f_ZZ"
	short_class_name="4f_zz"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106579
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106580
		else
			exit
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106575
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106576
		else
			exit
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106573
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106574
		else
			exit
		fi
	else
		echo "no such 4f_zz final state"
		exit
	fi

elif [[ ${input_class_name} == "4f_zzww" || ${input_class_name} == "4f_zzwwmix" ]] ; then
	class_name="4f_ZZWWMix"
	short_class_name="4f_zzorww"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106721
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106722
		else
			exit
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106553
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106554
		else
			exit
		fi
	else
		echo "no such 4f_zzww final state"
		exit
	fi

elif [[ ${input_class_name} == "4f_szee" || ${input_class_name} == "4f_singlezee" ]] ; then
	class_name="4f_singleZee"
	short_class_name="4f_sze"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106556
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106558
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106555
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106557
		else
			exit
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106560
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106562
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106559
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106561
		else
			exit
		fi
	else
		echo "no such 4f_szee final state"
		exit
	fi

elif [[ ${input_class_name} == "4f_sznn" || ${input_class_name} == "4f_singleznunu" ]] ; then
	class_name="4f_singleZnunu"
	short_class_name="4f_sznu"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106589
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106590
		else
			exit
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106571
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106572
		else
			exit
		fi
	else
		echo "no such 4f_sznn final state"
		exit
	fi

elif [[ ${input_class_name} == "4f_szsw" || ${input_class_name} == "4f_singlezsinglewmix" ]] ; then
	class_name="4f_singleZsingleWMix"
	short_class_name="4f_szeorsw"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106568
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106570
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106567
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106569
		else
			exit
		fi
	else
		echo "no such 4f_szsw final state"
		exit
	fi

elif [[ ${input_class_name} == "4f_sw" || ${input_class_name} == "4f_singlew" ]] ; then
	class_name="4f_singleW"
	short_class_name="4f_sw"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106586
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106588
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106585
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106587
		else
			exit
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106564
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106566
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106563
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106565
		else
			exit
		fi
	else
		echo "no such 4f_sw final state"
		exit
	fi

elif [[ ${input_class_name} == "2f_z" ]] ; then
	class_name="2f_Z"
	short_class_name="2f_z"
	info_class_name="2f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106605
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106606
		else
			exit
		fi
	elif [[ ${input_final_states} == "bhabhag" || ${input_final_states} == "bha" ]] ; then
		final_states="bhabhag"
		short_final_states="_bhabhag"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106602
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106604
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106601
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106603
		else
			exit
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106607
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106608
		else
			exit
		fi
	else
		echo "no such 2f_z final state"
		exit
	fi

elif [[ ${input_class_name} == "higgs" ]] ; then
	class_name="higgs"
	short_class_name=""
	info_class_name="higgs"
	if [[ ${input_final_states} == "e1" ]] ; then
		final_states="e1"
		short_final_states=e1e1h
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106476
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106477
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106475
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106478
		else
			exit
		fi
	elif [[ ${input_final_states} == "e2" ]] ; then
		final_states="e2"
		short_final_states=e2e2h
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106479
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106480
		else
			exit
		fi
	elif [[ ${input_final_states} == "e3" ]] ; then
		final_states="e3"
		short_final_states=e3e3h
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106481
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106482
		else
			exit
		fi
	elif [[ ${input_final_states} == "n" ]] ; then
		final_states="n"
		short_final_states=nnh
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106483
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106484
		else
			exit
		fi
	elif [[ ${input_final_states} == "q" ]] ; then
		final_states="q"
		short_final_states=qqh
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106485
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106486
		else
			exit
		fi
	elif [[ ${input_final_states} == "f" ]] ; then
		final_states="f"
		short_final_states=ffh
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106487
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106488
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106473
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106474
		else
			exit
		fi
	else
		echo "no such higgs final state"
		exit
	fi

elif [[ ${input_class_name} == "nh" || ${input_class_name} == "np-light-higgs" ]] ; then
	class_name="np-light-higgs"
	short_class_name=""
	info_class_name="np-light-higgs"
	if [[ ${input_final_states} == "10" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh10_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250200
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250201
		else
			exit
		fi
	elif [[ ${input_final_states} == "30" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh30_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250202
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250203
		else
			exit
		fi
	elif [[ ${input_final_states} == "50" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh50_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250204
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250205
		else
			exit
		fi
	elif [[ ${input_final_states} == "70" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh70_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250206
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250207
		else
			exit
		fi
	elif [[ ${input_final_states} == "90" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh90_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250208
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250209
		else
			exit
		fi
	elif [[ ${input_final_states} == "115" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh115_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250210
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250211
		else
			exit
		fi
	else
		echo "no such np-light-higgs final state"
		exit
	fi

elif [[ ${class_name} == "aa" ]] ; then
	short_class_name="aa"
	if [[ ${input_final_states} == "2f" ]] ; then
		final_states="2f"
		short_final_states="ee.eW.pW"
		info_class_name="aa_2f"
	elif [[ ${input_final_states} == "minijet" ]] ; then
		final_states="minijet"
		short_final_states="aa"
		info_class_name="aa_minijet"
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="aa"
		info_class_name="aa_lowpt"
	else
		echo "no such aa final state"
		exit
	fi
else
	echo "no such class name"

fi





data_source=/pnfs/desy.de/ilc/prod/ilc/mc-dbd/ild/dst-merged
info_source=/pnfs/desy.de/ilc/prod/ilc/mc-dbd.log/generated 
if [[ ${class_name} == "higgs" ]] ; then
	data_folder=${data_source}/${energy_version}/${class_name}_"ffh"/${ILD_version}/${reconstruction_version}
elif [[ ${class_name} == "np-light-higgs" ]] ; then
	data_folder=${data_source}/${energy_version}/${class_name}/${ILD_version}/${reconstruction_version}
else
	data_folder=${data_source}/${energy_version}/${class_name}_${final_states}/${ILD_version}/${reconstruction_version}
fi
info_folder=${info_source}/${energy_version}





short_name=${short_class_name}${short_final_states}
if [[ ${class_name} == "np-light-higgs" ]] ; then
	file_name_aux1=.d_dstm_8171_
else
	file_name_aux1=""
fi
file_name_basic=r${reconstruction_version}.s${simulation_version}.m${ILD_version}.E${energy_version}.I${file_id}.P${short_name}.${polarization}${file_name_aux1}
if [[ ${class_name} == "np-light-higgs" ]] ; then
	file_name_ext=.slcio
else
	file_name_ext=DST.slcio
fi

info_name=${info_folder}/${info_class_name}/E${energy_version}.P${short_name}.Gwhizard-1_95.${polarization}.I${file_id}.txt


if [ ! -d ${data_folder} ] ; then
	echo "no such folder, please check!"
	exit
fi

if [[ ${class_name} == "np-light-higgs" ]] ; then
	if [ ! -f "${data_folder}/${file_name_basic}1${file_name_ext}" ] ; then
		echo "file does not exist ${data_folder}/${file_name_basic}1${file_name_ext}"   
		exit
	fi
else
	if [ ! -f "${data_folder}/${file_name_basic}-00001-${file_name_ext}" ] ; then
		echo "file does not exist ${data_folder}/${file_name_basic}-00001-${file_name_ext}"   
		exit
	fi
fi

fnum=0

for j in ${data_folder}/${file_name_basic}* 
do
	fnum=$[fnum+1]
done







echo "channel is:  ${class_name} ${final_states} ${pol_in} is--------"
echo "info_name:    "${info_name}
xection=` grep "cross_section_in_fb" ${info_name} | cut -d "=" -f2 | sed -e "s/ //g" `
xection_err=` grep "cross_section_error_in_fb" ${info_name} | cut -d "=" -f2 | sed -e "s/ //g" `
lumi=` grep "luminosity" ${info_name} | cut -d "=" -f2 | sed -e "s/ //g" `
totnum=` grep "total_number_of_events" ${info_name} | cut -d "=" -f2 | sed -e "s/ //g" `
echo "xection is:      "${xection}
echo "xection err is:  "${xection_err}
echo "luminosity is:   "${lumi}
echo "totnum is:       "${totnum}


slcio_totnum=0
echo  "filenumberis:  "$fnum
for (( i=1; i<=$fnum; i++ )) 
do
	if [[ ${class_name} == "np-light-higgs" ]] ; then
		file_number="${i}"
		full_file_name=${data_folder}/${file_name_basic}${file_number}${file_name_ext}
	else
		if [[ $i -lt $(( 10 )) ]] ; then
			file_number="0000${i}"
		elif [[ $i -gt $(( 9 )) && $i -lt $(( 100 )) ]] ; then
			file_number="000${i}"
		fi
		full_file_name=${data_folder}/${file_name_basic}-${file_number}-${file_name_ext}
	fi
	echo "file_name:    "${full_file_name}
	anainfo=" anajob $full_file_name 1 "
	slcio_num=` eval $anainfo | grep "number of events" | cut -d "," -f2 | cut -d ":" -f2  | sed -e "s/ ]//g" `
	echo "file_number:  " $slcio_num
	(( slcio_totnum = slcio_totnum + slcio_num ))
done
echo "total_file_number:  " $slcio_totnum

