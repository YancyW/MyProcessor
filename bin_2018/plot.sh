#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}"  )" && pwd  )"

cd ${DIR}/../macro/basic_plot
mass=30

# mass_profile="{ \"../results_${mass}/nh_${mass}_${mass}_${mass}\"
# 				}" 
  mass_profile="{ \"../../results_125/higgs_e2_eR.pL\"  
 	 			}" 
# mass_profile="{ \"../../results_125/higgs_e2_eL.pR\", 
#                 \"../../results_opt/higgs_e2_l4_eL.pR\", 
#                 \"../../results_opt/higgs_e2_s4_eL.pR\", 
#	 			}" 


# mass_profile="{ \"../../results_${mass}/nh_${mass}_${mass}_${mass}\", 
#                 \"../../results_${mass}/2f_z_l_${mass}\", 
#                 \"../../results_${mass}/2f_z_bha_${mass}\", 
#                 \"../../results_${mass}/4f_zz_l_${mass}\",
#                 \"../../results_${mass}/4f_zz_sl_${mass}\",
#                 \"../../results_${mass}/4f_sw_l_${mass}\", 
#                 \"../../results_${mass}/4f_sw_sl_${mass}\", 
#                 \"../../results_${mass}/4f_ww_l_${mass}\" ,
#                 \"../../results_${mass}/4f_ww_sl_${mass}\" ,
#                 \"../../results_${mass}/4f_zzww_l_${mass}\" ,
#                 \"../../results_${mass}/4f_szee_l_${mass}\" ,
#                 \"../../results_${mass}/4f_szee_sl_${mass}\" ,
#                 \"../../results_${mass}/4f_sznn_l_${mass}\" ,
#                 \"../../results_${mass}/4f_sznn_sl_${mass}\" ,
#                 \"../../results_${mass}/4f_szsw_l_${mass}\" 
#	 			}" 




 root -l -q -b plot_histgram.C"(${mass_profile})"
#root -l -q -b kcut_zpt.C"(${mass_profile})"
#root -l -q -b kcut_zmass.C"(${mass_profile})"
#root -l -q -b kcut_invis_costheta.C"(${mass_profile})"
#root -l -q -b kcut_recoil_mass.C"(${mass_profile})"
#root -l -q -b kcut_vis_e.C"(${mass_profile})"
#root -l -q -b kcut_vis_e.C"(${mass_profile})"
