#ifndef ELEMENT_DATATYPE_H
#define ELEMENT_DATATYPE_H  
#include "TTree.h"
#include "Lepton_Class.h"
#include "TMVA/Reader.h"

class Counter{
	public:
		float xsec      ;

		//***********
		//event number
		//***********
		float evt         ;
		float pass_MC     ;
		float pass_RC     ;
		float pass_PO     ;
		float pass_Track  ;
		float pass_all    ;


		float pass_mc_num_false;
		float pass_rc_num_false;
		float pass_po_num_false;


		float pass_mc_energy_false;
		float pass_rc_energy_false;
		float pass_po_energy_false;

		float pass_mc_npfo ;
		float pass_mc_nisl ;
		float pass_pfo_nmc ;
		float pass_pfo_nisl;
		float pass_isl_nmc ;
		float pass_isl_npfo;

		void makeNTuple(TTree* _dataTree){
		}
		void init(){
	        pass_MC  = 0;
	        pass_RC  = 0;
	        pass_PO  = 0;
	        pass_all = 0;

	        pass_mc_num_false = 0;
	        pass_rc_num_false = 0;
	        pass_po_num_false = 0;


	        pass_mc_energy_false= 0;
	        pass_rc_energy_false= 0;
	        pass_po_energy_false= 0;


            pass_mc_npfo  = 0;
            pass_mc_nisl  = 0;
            pass_pfo_nmc  = 0;
            pass_pfo_nisl = 0;
            pass_isl_nmc  = 0;
            pass_isl_npfo = 0;
		}
};

//Stored data 
class Variable{
	public:
		//************************
		//inserted by process file
		//************************
		float  pfo_num                    ;
		float  higgs_channel              ;
		float  kcut_zmass                 ;
		float  kcut_zpt                   ;
		float  kcut_invis_costheta        ;
		float  kcut_invis_e               ;
		float  kcut_mva                   ;
		float  kcut_recoil_mass           ;
		float  kcut_vis_e                 ;
		float  lep_pair_costheta          ;
		float  lep_pair_costheta_pair     ;
		float  lep_pair_costheta_track1   ;        
		float  lep_pair_costheta_track2   ;        
		float  lep_pair_azimuth           ;
		float  lep_pair_azimuth_pair      ;
		float  lep_pair_azimuth_track1    ;        
		float  lep_pair_azimuth_track2    ;        
		float  track1_pt                  ;        
		float  track1_e                   ;        
		float  track2_pt                  ;        
		float  track2_e                   ;        
		float  total_photon_number        ;        
		float  photon_number              ;        
		float  photon_energy1             ;        
		float  photon_costheta1           ;        
		float  photon_energy2             ;        
		float  photon_costheta2           ;        
		float  forward_photon_number      ;        
		float  forward_photon_energy1     ;        
		float  forward_photon_costheta1   ;        
		float  forward_photon_energy2     ;        
		float  forward_photon_costheta2   ;        
		float  opal_lep_isolation1        ;        
		float  opal_lep_isolation2        ;        
		float  opal_zmass                 ;        
		float  opal_lep_pair_pz           ;        
		float  opal_invis_momentum        ;        
		float  opal_invis_costheta        ;        
		float  opal_lep_pair_acoplanarity ;        
		float  opal_photon_number         ;        

		void init(){
			pfo_num                 =-10.1;
			higgs_channel           =-10.1;
			kcut_zmass              =-10.1;
			kcut_zpt                =-10.1;
			kcut_invis_costheta     =-10.1;
			kcut_invis_e            =-10.1;
			kcut_mva                =-10.1;
			kcut_recoil_mass        =-10.1;
			kcut_vis_e              =-10.1;

			lep_pair_costheta       =-10.1;
			lep_pair_costheta_pair  =-10.1;
			lep_pair_costheta_track1=-10.1;
			lep_pair_costheta_track2=-10.1;
			lep_pair_azimuth        =-10.1;
			lep_pair_azimuth_pair   =-10.1;
			lep_pair_azimuth_track1 =-10.1;
			lep_pair_azimuth_track2 =-10.1;
		    track1_pt               =-10.1;        
		    track1_e                =-10.1;        
		    track2_pt               =-10.1;        
		    track2_e                =-10.1;        

			total_photon_number     =-10.1;        
			photon_number           =-10.1;        
			photon_energy1          =-10.1;        
			photon_costheta1        =-10.1;        
			photon_energy2          =-10.1;        
			photon_costheta2        =-10.1;        
			forward_photon_number   =-10.1;        
			forward_photon_energy1  =-10.1;        
			forward_photon_costheta1=-10.1;        
			forward_photon_energy2  =-10.1;        
			forward_photon_costheta2=-10.1;        

		    opal_lep_isolation1       = -10.1;       
		    opal_lep_isolation2       = -10.1;       
		    opal_zmass                = -10.1;       
		    opal_lep_pair_pz          = -10.1;       
		    opal_invis_momentum       = -10.1;       
		    opal_invis_costheta       = -10.1;       
		    opal_lep_pair_acoplanarity= -10.1;       
		    opal_photon_number        = -10.1;       
		}

		void Fill_Data(TTree* tree, std::string prefix);
};

class Number{
	public:
		float num                           ;
		float num_plus                      ;
		float num_minus                     ;

		void init(){
		    num                = -1       ;
		    num_plus           = -1       ;
		    num_minus          = -1       ;
		}

		void Fill_Data(TTree* tree, std::string prefix);
};


class Infomation{
	public:
		Variable                        data_muon_hs       ;
		Variable                        data_muon_py_ex    ;
		Variable                        data_muon_py_im    ;
		Variable                        data_muon_de       ;

		std::vector<float>              Lepton_cosTheta    ;

		Number                          num_muon_hs        ;
		Number                          num_muon_py_ex     ;
		Number                          num_muon_py_im     ;
		Number                          num_muon_de        ;

		CLEPTON_CUT                     cut                ;
		CLEPTON<MCParticle*>            gen_lepton         ;
		CLEPTON<MCParticle*>            mc_lepton          ;
		CLEPTON<ReconstructedParticle*> rec_lepton         ;
		CLEPTON<ReconstructedParticle*> pfo_lepton         ;
		std::vector<TMVA::Reader*>      readers            ;


		Infomation(){
		}


		void init(){
			cut.Read_Cut();
			data_muon_hs   .init();
			data_muon_py_ex.init();
			data_muon_py_im.init();
			data_muon_de   .init();
			Lepton_cosTheta.clear();
			num_muon_hs    .init();
			num_muon_py_ex .init();
			num_muon_py_im .init();
			num_muon_de    .init();
		}


		~Infomation(){
			for (std::vector<TMVA::Reader*>::const_iterator ireader=readers.begin();ireader!=readers.end();++ireader) {
				delete *ireader;
			}
		}
}; 

#endif
