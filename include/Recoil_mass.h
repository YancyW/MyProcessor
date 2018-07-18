#include "MyProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>


#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Vertex.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include "UTIL/PIDHandler.h"
#include "LCIOSTLTypes.h"

//----- include for verbosity dependend logging ---------
#include "marlin/VerbosityLevels.h"

//ROOT
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TVector3.h"
#include "TLorentzVector.h"

#include "TRandom.h"
#include "MCOperator.h"

using namespace lcio;
using namespace marlin;

template<typename TRECOIL>
class CRecoil{
	private:
		float _recoil_mass;
		float _pair_mass;
		std::vector<TRECOIL> choosedpair;
	public:
		CRecoil(){
			_recoil_mass = 0;
			_pair_mass   = 0;
		}
		void setRecoilMass(float in){
			_recoil_mass=in;
		}

		void setPairMass(float in){
			_pair_mass=in;
		}

		void checkRecoil(std::vector<TRECOIL> final_particles){
			float recoil_mass_aux=0,pair_mass_aux=0;
			bool Jrecoil=obvRecoil(final_particles,choosedpair,recoil_mass_aux,pair_mass_aux);
			if(Jrecoil){
				setRecoilMass(recoil_mass_aux);
				setPairMass  (pair_mass_aux  );
			}
		}
};


template<typename TOBVRECOIL>
bool MyProcessor::obvRecoil(std::vector<TOBVRECOIL> in, std::vector<TOBVRECOIL> &out) {
	debug.Message(2,11,"in obvRecoil: begin");
	int num=in.size();

	debug.Message(2,11,"in obvRecoil: begin transfer data to TLorentzVector ");
	std::vector<TLorentzVector> inVector;
	for(int i=0;i<num;i++){
		TLorentzVector in_tmp= TLorentzVector( in[i]->getMomentum(), in[i]->getEnergy() );
		inVector.push_back(in_tmp);
	}

	debug.Message(2,11,"in obvRecoil: init variables ");
	float chi_min= 1000000.0;
	float chi_tmp=chi_min;
	float recoil_mass_final = 999.99;
	float pair_mass_final = 999.99;

	TLorentzVector collider;
	TLorentzVector recoil_mom;
	TLorentzVector pair_mom;
	collider.SetPxPyPzE            (_cmenergy*sin(0.014/2.0),0,0,_cmenergy);

	int choosei=1000,choosej=1000;
	debug.Message(2,13,"in obvRecoil: begin loop for minimum chi, number of charge lepton is ",num);
	for(int i=0;i<num-1;i++){
		for(int j=1;j<num;j++){
			pair_mom = inVector[i]+inVector[j];
			debug.Message(2,11,"in obvRecoil: the loop particle are",i,inVector[i]);
			debug.Message(2,11,"in obvRecoil: the loop particle are",j,inVector[j]);
			if(std::abs(pair_mom.M()-91.187)>40.0){
				debug.Message(2,13,"in obvRecoil: zmass distance",std::abs(pair_mom.M()-91.187));
				continue;
			}
			if((in[i]->getCharge())*(in[j]->getCharge())>0){//Should be different sign lepton
				debug.Message(2,13,"in obvRecoil: different charge",in[i]->getCharge(),in[j]->getCharge());//Should be different sign lepton
				continue;
			}
			recoil_mom = collider-inVector[i]-inVector[j];
			float  recoil_mass=recoil_mom.M();
			float  pair_mass=pair_mom.M();
			float  sigma_pair=1;
			float  sigma_recoil=1;
		
			if(fabs(_hmass-10)<0.00001){
					sigma_pair  =2.06;
					sigma_recoil=4.26;
			}
			else if(fabs(_hmass-15)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=3.70;
			}
			else if(fabs(_hmass-20)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=3.20;
			}
			else if(fabs(_hmass-25)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=2.7;
			}
			else if(fabs(_hmass-30)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=2.17;
			}
			else if(fabs(_hmass-35)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=2.07;
			}
			else if(fabs(_hmass-40)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.97;
			}
			else if(fabs(_hmass-45)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.87;
			}
			else if(fabs(_hmass-50)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.79;
			}
			else if(fabs(_hmass-55)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.72;
			}
			else if(fabs(_hmass-60)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.66;
			}
			else if(fabs(_hmass-65)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.61;
			}
			else if(fabs(_hmass-70)<0.00001){
					sigma_pair  =2.06;
					sigma_recoil=1.55;
			}
			else if(fabs(_hmass-75)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.52;
			}
			else if(fabs(_hmass-80)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.49;
			}
			else if(fabs(_hmass-85)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.46;
			}
			else if(fabs(_hmass-90)<0.00001){
					sigma_pair  =2.07;
					sigma_recoil=1.43;
			}
			else if(fabs(_hmass-95)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.38;
			}
			else if(fabs(_hmass-100)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.33;
			}
			else if(fabs(_hmass-105)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.28;
			}
			else if(fabs(_hmass-110)<0.00001){
					sigma_pair  =2.03;
					sigma_recoil=1.24;
			}
			else if(fabs(_hmass-115)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.20;
			}
			else if(fabs(_hmass-120)<0.00001){
					sigma_pair  =2.05;
					sigma_recoil=1.13;
			}
			else if(fabs(_hmass-125)<0.00001){
					sigma_pair  =2.03;
					sigma_recoil=1.06;
			}
			else{
				ShowMessage(2,"wrong hmass:",_hmass);
				
			}
			chi_tmp = std::pow((pair_mass-91.187)/sigma_pair,2)+std::pow((recoil_mass-_hmass)/sigma_recoil,2);

			debug.Message(2,11,"in obvRecoil: the loop variable recoil_mass",recoil_mass);
			debug.Message(2,11,"in obvRecoil: the loop variable pair_mass"  ,pair_mass);
			debug.Message(2,11,"in obvRecoil: the loop variable chi_tmp"  ,chi_tmp);

			if(chi_tmp<chi_min){
				chi_min = chi_tmp;
				choosei=i;
				choosej=j;
				recoil_mass_final=recoil_mass;
				pair_mass_final=pair_mass;
			}
		}
	}

	debug.Message(2,11,"in obvRecoil: check whether has correct muon pair ");
	if(choosei!=1000 && choosej!=1000){
		debug.Message(2,12,"in obvRecoil: the muon pair are ",choosei,choosej);
		debug.Message(2,12,"in obvRecoil: the size of in vector are ",in.size());
    	out.push_back(in[choosei]);
    	out.push_back(in[choosej]);
		return(true);
	}
	debug.Message(2,11,"in obvRecoil: no correct pair, return ");
	return(false);
}


