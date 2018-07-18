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
reconstruction_version=$6
simulation_version=$7



if [[  ${input_class_name} == "aa_2f" || ${input_class_name} == "aa_minijet" ]] ; then
	if [[  ${pol_in} == "lr" ]] ; then
		polarization="eW.pW"
	elif [[ ${pol_in} == "rl" ]] ; then
		polarization="eW.pB"
	elif [[ ${pol_in} == "ll" ]] ; then
		polarization="eB.pW"
	elif [[ ${pol_in} == "rr" ]] ; then
		polarization="eB.pB"
	else
		echo "no such input pol"
	fi
else
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
fi

if [[ ${input_class_name} == "4f_ww" ]] ; then
	class_name="4f_WW"
	short_class_name="4f_ww"
	info_class_name="4f"
	if [[ ${input_final_states} == "leptonic" || ${input_final_states} == "l" ]] ; then
		final_states="leptonic"
		short_final_states="_l"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250026
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250028
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10306
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10305
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250018
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250020
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10318
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10317
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250006
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250008
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10398
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10397
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
			file_id=250022
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250024
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10310
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10309
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250014
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250016
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10302
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10301
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250002
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250004
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10402
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10401
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
			file_id=250030
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250032
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10314
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10313
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250010
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250012
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10405
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10406
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
			file_id=250034
			if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
				file_postid= 10446
			elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
				file_postid= 10445
			fi
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250035
			if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
				file_postid= 10475
			elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
				file_postid= 10476
			fi
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=250033
			if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
				file_postid= 10442
			elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
				file_postid= 10441
			fi
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=250036
			if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
				file_postid= 10450
			elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
				file_postid= 10449
			fi
		else
			exit
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250038
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250039
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=250037
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=250040
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10421
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10422
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
			file_id=250054
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250056
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10290
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10289
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250058
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250060
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10425
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10426
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
			file_id=250050
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250051
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=250049
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=250052
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10293
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10294
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
			file_id=250046
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250047
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=250045
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=250048
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10267
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10268
		fi
	elif [[ ${input_final_states} == "semileptonic" || ${input_final_states} == "sl" ]] ; then
		final_states="semileptonic"
		short_final_states="_sl"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250042
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250042
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=250041
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=250043
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10417
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10418
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
			file_id=250106
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250108
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10255
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10256
		fi
	elif [[ ${input_final_states} == "bhabhag" || ${input_final_states} == "bha" ]] ; then
		final_states="bhabhag"
		short_final_states="_bhabhag"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250102
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250103
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=250101
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=250104
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10366
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10365
		fi
	elif [[ ${input_final_states} == "hadronic" || ${input_final_states} == "h" ]] ; then
		final_states="hadronic"
		short_final_states="_h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250114
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250116
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10410
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10409
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
			file_id=106516
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106517
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=106515
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=106518
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10263
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10264
		fi
	elif [[ ${input_final_states} == "e2" ]] ; then
		final_states="e2"
		short_final_states=e2e2h
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106519
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106520
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10263
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10264
		fi
	elif [[ ${input_final_states} == "e3" ]] ; then
		final_states="e3"
		short_final_states=e3e3h
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106521
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106522
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10263
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10264
		fi
	elif [[ ${input_final_states} == "n" ]] ; then
		final_states="n"
		short_final_states=nnh
		if [[ ${polarization} == "eL.pR" ]] ; then
			echo "no such file"
			exit
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106524
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10297
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10298
		fi
	elif [[ ${input_final_states} == "q" ]] ; then
		final_states="q"
		short_final_states=qqh
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=106525
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=106526
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10263
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10264
		fi
	elif [[ ${input_final_states} == "f" ]] ; then
		final_states="f"
		short_final_states=ffh
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=108162
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=108163
		elif [[ ${polarization} == "eL.pL"  ]] ; then
			file_id=108161
		elif [[ ${polarization} == "eR.pR"  ]] ; then
			file_id=108164
		else
			exit
		fi
		if [[ ${ILD_version} == "ILD_l5_o1_v02" ]]; then
			file_postid= 10260
		elif [[ ${ILD_version} == "ILD_s5_o1_v02" ]]; then
			file_postid= 10259
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
			file_id=250260
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250261
		else
			exit
		fi
	elif [[ ${input_final_states} == "15" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh15_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250262
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250263
		else
			exit
		fi
	elif [[ ${input_final_states} == "20" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh20_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250264
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250265
		else
			exit
		fi
	elif [[ ${input_final_states} == "25" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh25_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250266
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250267
		else
			exit
		fi
	elif [[ ${input_final_states} == "30" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh30_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250268
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250269
		else
			exit
		fi
	elif [[ ${input_final_states} == "35" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh35_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250270
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250271
		else
			exit
		fi
	elif [[ ${input_final_states} == "40" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh40_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250272
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250273
		else
			exit
		fi
	elif [[ ${input_final_states} == "45" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh45_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250274
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250275
		else
			exit
		fi
	elif [[ ${input_final_states} == "50" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh50_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250276
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250277
		else
			exit
		fi
	elif [[ ${input_final_states} == "55" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh55_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250278
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250279
		else
			exit
		fi
	elif [[ ${input_final_states} == "60" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh60_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250280
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250281
		else
			exit
		fi
	elif [[ ${input_final_states} == "65" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh65_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250282
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250283
		else
			exit
		fi
	elif [[ ${input_final_states} == "70" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh70_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250284
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250285
		else
			exit
		fi
	elif [[ ${input_final_states} == "75" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh75_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250286
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250287
		else
			exit
		fi
	elif [[ ${input_final_states} == "80" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh80_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250288
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250289
		else
			exit
		fi
	elif [[ ${input_final_states} == "85" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh85_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250290
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250291
		else
			exit
		fi
	elif [[ ${input_final_states} == "90" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh90_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250292
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250293
		else
			exit
		fi
	elif [[ ${input_final_states} == "95" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh95_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250294
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250295
		else
			exit
		fi
	elif [[ ${input_final_states} == "100" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh100_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250296
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250297
		else
			exit
		fi
	elif [[ ${input_final_states} == "105" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh105_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250298
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250299
		else
			exit
		fi
	elif [[ ${input_final_states} == "110" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh110_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250300
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250301
		else
			exit
		fi
	elif [[ ${input_final_states} == "115" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh115_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250302
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250303
		else
			exit
		fi
	elif [[ ${input_final_states} == "120" ]] ; then
		final_states="e2"
		short_final_states="np-lmh_mh120_e2e2h"
		if [[ ${polarization} == "eL.pR" ]] ; then
			file_id=250304
		elif [[ ${polarization} == "eR.pL"  ]] ; then
			file_id=250305
		else
			exit
		fi
	else
		echo "no such np-light-higgs final state"
		exit
	fi

elif [[ ${input_class_name} == "aa_2f" ]] ; then
	class_name="aa"
	short_class_name="aa"
	if [[ ${input_final_states} == "ee" ]] ; then
		final_states="2f"
		short_final_states="_ee"
		info_class_name="aa_2f"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37761
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37762
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37763
		elif [[ ${polarization} == "eB.pB"  ]] ; then
			file_id=37764
		else
			exit
		fi
	elif [[ ${input_final_states} == "ll" ]] ; then
		final_states="2f"
		short_final_states="_ll"
		info_class_name="aa_2f"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37765
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37766
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37767
		elif [[ ${polarization} == "eB.pB"  ]] ; then
			file_id=37768
		else
			exit
		fi
	elif [[ ${input_final_states} == "xx" ]] ; then
		final_states="2f"
		short_final_states="_xx"
		info_class_name="aa_2f"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37769
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37770
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37771
		elif [[ ${polarization} == "eB.pB"  ]] ; then
			file_id=37772
		else
			exit
		fi
	elif [[ ${input_final_states} == "yy" ]] ; then
		final_states="2f"
		short_final_states="_yy"
		info_class_name="aa_2f"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37773
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37774
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37775
		elif [[ ${polarization} == "eB.pB"  ]] ; then
			file_id=37776
		else
			exit
		fi
	else
		echo "no such higgs final state"
		exit
	fi
elif [[ ${input_class_name} == "aa_minijet" ]] ; then
	class_name="aa"
	short_class_name="aa"
	if [[ ${input_final_states} == "min0410m1" ]] ; then
		final_states="minijet"
		short_final_states="min_04_10_m1"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37833
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37834
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37835
		else
			exit
		fi
	elif [[ ${input_final_states} == "min1020m1" ]] ; then
		final_states="minijet"
		short_final_states="min_10_20_m1"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37837
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37838
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37839
		else
			exit
		fi
	elif [[ ${input_final_states} == "min2040m1" ]] ; then
		final_states="minijet"
		short_final_states="min_20_40_m1"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37841
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37842
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37843
		else
			exit
		fi
	elif [[ ${input_final_states} == "min40xxm1" ]] ; then
		final_states="minijet"
		short_final_states="min_40_xx_m1"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37845
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37846
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37847
		else
			exit
		fi
	elif [[ ${input_final_states} == "min0410m4" ]] ; then
		final_states="minijet"
		short_final_states="min_04_10_m4"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37849
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37850
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37851
		else
			exit
		fi
	elif [[ ${input_final_states} == "min1020m4" ]] ; then
		final_states="minijet"
		short_final_states="min_10_20_m4"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37853
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37854
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37855
		else
			exit
		fi
	elif [[ ${input_final_states} == "min2040m4" ]] ; then
		final_states="minijet"
		short_final_states="min_20_40_m4"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37857
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37858
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37859
		else
			exit
		fi
	elif [[ ${input_final_states} == "min40xxm4" ]] ; then
		final_states="minijet"
		short_final_states="min_40_xx_m4"
		info_class_name="aa_minijet"
		if [[ ${polarization} == "eW.pW" ]] ; then
			file_id=37861
		elif [[ ${polarization} == "eW.pB"  ]] ; then
			file_id=37862
		elif [[ ${polarization} == "eB.pW"  ]] ; then
			file_id=37863
		else
			exit
		fi
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





data_source=/pnfs/desy.de/ilc/prod/ilc/mc-opt-3/ild/dst-merged
info_source=/pnfs/desy.de/ilc/prod/ilc/mc-opt-3.log/generated 
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
	file_name_aux1=.n001..d_dstm_
else
	file_name_aux1=""
fi

file_name_basic=r${reconstruction_version}.s${simulation_version}.m${ILD_version}.E${energy_version}.I${file_id}.P${short_name}.${polarization}${file_name_aux1}
file_name_ext=.slcio

if [[  ${input_class_name} == "aa_2f" || ${input_class_name} == "aa_minijet" ]] ; then
	info_name=${info_folder}/${info_class_name}/E0${energy_version}.P${short_name}.Gwhizard-1.95.${polarization}.I${file_id}.txt
else
	info_name=${info_folder}/${info_class_name}/E${energy_version}.P${short_name}.Gwhizard-1_95.${polarization}.I${file_id}.txt
fi









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
	if [[ ${input_class_name} == "aa_minijet" ]]; then
		(( slcio_totnum = totnum ))
	else
		anainfo=" anajob $full_file_name 1 "
		slcio_num=` eval $anainfo | grep "number of events" | cut -d "," -f2 | cut -d ":" -f2  | sed -e "s/ ]//g" `
		echo "file_number:  " $slcio_num
		(( slcio_totnum = slcio_totnum + slcio_num ))
	fi
done
echo "total_file_number:  " $slcio_totnum

