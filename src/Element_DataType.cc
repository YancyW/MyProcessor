#include "Element_DataType.h"
void Variable::Fill_Data(TTree* tree, std::string prefix){
	tree->Branch( (prefix+"higgs_channle"            ).c_str()    , &higgs_channel                   ,(prefix+"higgs_channel"            ).c_str()        );
	tree->Branch( (prefix+"kcut_zmass"               ).c_str()    , &kcut_zmass                      ,(prefix+"kcut_zmass"               ).c_str()        );
	tree->Branch( (prefix+"kcut_zpt"                 ).c_str()    , &kcut_zpt                        ,(prefix+"kcut_zpt"                 ).c_str()        );
	tree->Branch( (prefix+"kcut_invis_costheta"      ).c_str()    , &kcut_invis_costheta             ,(prefix+"kcut_invis_costheta"      ).c_str()        );
	tree->Branch( (prefix+"kcut_invis_e"             ).c_str()    , &kcut_invis_e                    ,(prefix+"kcut_invis_e"             ).c_str()        );
	tree->Branch( (prefix+"kcut_mva"                 ).c_str()    , &kcut_mva                        ,(prefix+"kcut_mva"                 ).c_str()        );
	tree->Branch( (prefix+"kcut_recoil_mass"         ).c_str()    , &kcut_recoil_mass                ,(prefix+"kcut_recoil_mass"         ).c_str()        );
	tree->Branch( (prefix+"kcut_vis_e"               ).c_str()    , &kcut_vis_e                      ,(prefix+"kcut_vis_e"               ).c_str()        );

	tree->Branch( (prefix+"lep_pair_costheta"        ).c_str()    , &lep_pair_costheta               ,(prefix+"lep_pair_costheta"        ).c_str()        );
	tree->Branch( (prefix+"lep_pair_costheta_pair"   ).c_str()    , &lep_pair_costheta_pair          ,(prefix+"lep_pair_costheta_pair"   ).c_str()        );
	tree->Branch( (prefix+"lep_pair_costheta_track1" ).c_str()    , &lep_pair_costheta_track1        ,(prefix+"lep_pair_costheta_track1" ).c_str()        );        
	tree->Branch( (prefix+"lep_pair_costheta_track2" ).c_str()    , &lep_pair_costheta_track2        ,(prefix+"lep_pair_costheta_track2" ).c_str()        );        
	tree->Branch( (prefix+"lep_pair_azimuth"         ).c_str()    , &lep_pair_azimuth                ,(prefix+"lep_pair_azimuth"         ).c_str()        );
	tree->Branch( (prefix+"lep_pair_azimuth_pair"    ).c_str()    , &lep_pair_azimuth_pair           ,(prefix+"lep_pair_azimuth_pair"    ).c_str()        );
	tree->Branch( (prefix+"lep_pair_azimuth_track1"  ).c_str()    , &lep_pair_azimuth_track1         ,(prefix+"lep_pair_azimuth_track1"  ).c_str()        );        
	tree->Branch( (prefix+"lep_pair_azimuth_track2"  ).c_str()    , &lep_pair_azimuth_track2         ,(prefix+"lep_pair_azimuth_track2"  ).c_str()        );        
	tree->Branch( (prefix+"track1_pt"                ).c_str()    , &track1_pt                       ,(prefix+"track1_pt"                ).c_str()        );        
	tree->Branch( (prefix+"track1_e"                 ).c_str()    , &track1_e                        ,(prefix+"track1_e"                 ).c_str()        );        
	tree->Branch( (prefix+"track2_pt"                ).c_str()    , &track2_pt                       ,(prefix+"track2_pt"                ).c_str()        );        
	tree->Branch( (prefix+"track2_e"                 ).c_str()    , &track2_e                        ,(prefix+"track2_e"                 ).c_str()        );        

	tree->Branch( (prefix+"total_photon_num"         ).c_str()    , &total_photon_number             ,(prefix+"total_photon_num"         ).c_str()        );
	tree->Branch( (prefix+"photon_num"               ).c_str()    , &photon_number                   ,(prefix+"photon_num"               ).c_str()        );
	tree->Branch( (prefix+"photon_energy1"           ).c_str()    , &photon_energy1                  ,(prefix+"photon_energy1"           ).c_str()        );
	tree->Branch( (prefix+"photon_energy2"           ).c_str()    , &photon_energy2                  ,(prefix+"photon_energy2"           ).c_str()        );
	tree->Branch( (prefix+"photon_costheta1"         ).c_str()    , &photon_costheta1                ,(prefix+"photon_costheta1"         ).c_str()        );
	tree->Branch( (prefix+"photon_costheta2"         ).c_str()    , &photon_costheta2                ,(prefix+"photon_costheta2"         ).c_str()        );
	tree->Branch( (prefix+"forward_photon_num"       ).c_str()    , &forward_photon_number           ,(prefix+"forward_photon_num"       ).c_str()        );
	tree->Branch( (prefix+"forward_photon_energy1"   ).c_str()    , &forward_photon_energy1          ,(prefix+"forward_photon_energy1"   ).c_str()        );
	tree->Branch( (prefix+"forward_photon_energy2"   ).c_str()    , &forward_photon_energy2          ,(prefix+"forward_photon_energy2"   ).c_str()        );
	tree->Branch( (prefix+"forward_photon_costheta1" ).c_str()    , &forward_photon_costheta1        ,(prefix+"forward_photon_costheta1" ).c_str()        );
	tree->Branch( (prefix+"forward_photon_costheta2" ).c_str()    , &forward_photon_costheta2        ,(prefix+"forward_photon_costheta2" ).c_str()        );

	tree->Branch( (prefix+"opal_lep_isolation1"      ).c_str()    , &opal_lep_isolation1             ,(prefix+"opal_lep_isolation1"      ).c_str()        );
	tree->Branch( (prefix+"opal_lep_isolation2"      ).c_str()    , &opal_lep_isolation2             ,(prefix+"opal_lep_isolation2"      ).c_str()        );
	tree->Branch( (prefix+"opal_zmass"               ).c_str()    , &opal_zmass                      ,(prefix+"opal_zmass"               ).c_str()        );
	tree->Branch( (prefix+"opal_lep_pair_pz"         ).c_str()    , &opal_lep_pair_pz                ,(prefix+"opal_lep_pair_pz"         ).c_str()        );
	tree->Branch( (prefix+"opal_invis_momentum"      ).c_str()    , &opal_invis_momentum             ,(prefix+"opal_invis_momentum"      ).c_str()        );
	tree->Branch( (prefix+"opal_invis_costheta"      ).c_str()    , &opal_invis_costheta             ,(prefix+"opal_invis_costheta"      ).c_str()        );
	tree->Branch( (prefix+"opal_lep_pair_acoplanarity").c_str()   , &opal_lep_pair_acoplanarity      ,(prefix+"opal_lep_pair_acoplanarity").c_str()        );
	tree->Branch( (prefix+"opal_photon_number"       ).c_str()    , &total_photon_number             ,(prefix+"opal_photon_number"       ).c_str()        );
}

void Number::Fill_Data(TTree* tree, std::string prefix){
	tree->Branch( (prefix+"num"            ).c_str()    , &num      ,(prefix+"num"            ).c_str()        );
	tree->Branch( (prefix+"num_plus"       ).c_str()    , &num_plus ,(prefix+"num_plus"       ).c_str()        );
	tree->Branch( (prefix+"num_minus"      ).c_str()    , &num_minus,(prefix+"num_minus"      ).c_str()        );
}
