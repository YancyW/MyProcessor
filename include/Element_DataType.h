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

		void makeNTuple(TTree* _dataTree){
		}
};

//Stored data 
class Variable{
	public:
		//************************
		//inserted by process file
		//************************
		float  pfo_num                    ;
		float  kcut_zmass                 ;
		float  kcut_zpt                   ;
		float  kcut_invis_costheta        ;
		float  kcut_mva                   ;
		float  kcut_recoil_mass           ;
		float  kcut_vis_e                 ;
		float  lep_pair_costheta          ;
		float  lep_pair_costheta_pair     ;
		float  lep_pair_costheta_track1   ;        
		float  track1_pt                  ;        
		float  track1_e                   ;        
		float  track2_pt                  ;        
		float  track2_e                   ;        
		float  lep_pair_costheta_track2   ;        

		void init(){
			pfo_num                 =-10.1;
			kcut_zmass              =-10.1;
			kcut_zpt                =-10.1;
			kcut_invis_costheta     =-10.1;
			kcut_mva                =-10.1;
			kcut_recoil_mass        =-10.1;
			kcut_vis_e              =-10.1;

			lep_pair_costheta       =-10.1;
			lep_pair_costheta_pair  =-10.1;
			lep_pair_costheta_track1=-10.1;
			lep_pair_costheta_track2=-10.1;
		    track1_pt               =-10.1;        
		    track1_e                =-10.1;        
		    track2_pt               =-10.1;        
		    track2_e                =-10.1;        
		}
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

};


class Infomation{
	public:
		Variable                        data_muon_gen      ;
		Variable                        data_muon          ;
		Variable                        data_muon_recovery ;
		Variable                        data_elec_gen      ;
		Variable                        data_elec          ;
		Variable                        data_elec_recovery ;

		std::vector<float>              Lepton_cosTheta    ;

		Number                          num_lep           ;
		Number                          num_lep_gen       ;
		Number                          num_lep_recovery  ;
		Number                          num_muon           ;
		Number                          num_muon_gen       ;
		Number                          num_muon_recovery  ;
		Number                          num_elec           ;
		Number                          num_elec_gen       ;
		Number                          num_elec_recovery  ;

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
			data_muon.init();
			data_muon_gen.init();
			data_muon_recovery.init();
			data_elec.init();
			data_elec_gen.init();
			data_elec_recovery.init();
			Lepton_cosTheta.clear();
			num_muon.init();
			num_muon_gen.init();
			num_muon_recovery.init();
			num_elec.init();
			num_elec_gen.init();
			num_elec_recovery.init();
		}


		~Infomation(){
			for (std::vector<TMVA::Reader*>::const_iterator ireader=readers.begin();ireader!=readers.end();++ireader) {
				delete *ireader;
			}
		}
}; 

#endif
