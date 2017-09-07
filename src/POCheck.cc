#include "MyProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>
#include <math.h>

//LCIO
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
#include "TMVA/Reader.h"
#include "Fvector.h"
using namespace TMVA;

using namespace lcio;
using namespace marlin;

std::vector<ReconstructedParticle*> MyProcessor::checkPOFinalState(LCCollection* AllPOs) {
	std::vector<ReconstructedParticle*> FS;
	int npo = AllPOs->getNumberOfElements();
	for( int i = 0; i < npo; i++ ){
		ReconstructedParticle* po= dynamic_cast< ReconstructedParticle* >( AllPOs->getElementAt(i) );
		FS.push_back(po);
	}
	return(FS);
}


//template <class TPAIREVENT>
//void MyProcessor::getPOParticlePairEvent( std::vector<TPAIREVENT> PFOs,int PID, PAIR_EVENT<TPAIREVENT> &pair) {
//void MyProcessor::getPOParticlePairEvent( std::vector<ReconstructedParticle*> PFOs,int PID, PAIR_EVENT<ReconstructedParticleImpl*> &pair) {
//	pair.plus  = checkPOParticleType(PFOs,PID);
//	debug.Message(2,32,"in CheckPOParticlePairType:  check particle+, the particle plus size is",pair.plus.size());
//	pair.minus = checkPOParticleType(PFOs,-1*PID);
//	debug.Message(2,32,"in CheckPOParticlePairType:  check particle+-, the particle minus size is",pair.minus.size());
//	pair.pair  = pair.minus+pair.plus;
//	debug.Message(2,32,"in CheckPOParticlePairType:  the particle pair size is",pair.pair.size());
//
//	pair.setNum();
//	return;
//}


std::vector<ReconstructedParticle*> MyProcessor::CheckPOParticlePairType( std::vector<ReconstructedParticle*> PFOs,int PID, int counter_minus, int counter_plus, int counter_pair) {
	std::vector<ReconstructedParticle*> plus = checkPOParticleType(PFOs,PID);
	debug.Message(2,32,"in CheckPOParticlePairType:  check particle+, the particle plus size is",plus.size());
	std::vector<ReconstructedParticle*> minus = checkPOParticleType(PFOs,-1*PID);
	debug.Message(2,32,"in CheckPOParticlePairType:  check particle+-, the particle minus size is",minus.size());
	std::vector<ReconstructedParticle*> pair;
	pair=minus+plus;
	debug.Message(2,32,"in CheckPOParticlePairType:  the particle pair size is",pair.size());
	counter_pair  = pair.size();
	counter_plus  = plus.size();
	counter_minus = minus.size();
	return(pair);
}


std::vector<ReconstructedParticle*> MyProcessor::checkPOParticleType(std::vector<ReconstructedParticle*> POs,int PDG) {
	int num = POs.size();
	std::vector<ReconstructedParticle*> output;
	for( int i = 0; i < num; i++ ){
		if(POs[i]->getType()==PDG){
			output.push_back(POs[i]);
		}
	}
	return(output);
}


ReconstructedParticle* MyProcessor::getVisible(std::vector<ReconstructedParticle*> in){

	int num=in.size();

	ReconstructedParticle* visible;
    ReconstructedParticle* tmp;
    ReconstructedParticle* tmp1;
    if(num<=0){
    	visible=NewParticle(0,0,0,0,visible);
    	return(visible);
    }

    tmp1=Equal(in[0]);
    for(int i=1;i<num;i++){
    	tmp     = Add(tmp1,in[i]); 
    	delete tmp1;
    	tmp1    = tmp;
    }

    visible = tmp1;
	return(visible);

}



ReconstructedParticle* MyProcessor::getInVisible(std::vector<ReconstructedParticle*> in){
	ReconstructedParticle* visible = getVisible(in);
	ReconstructedParticle* RCCollider=NewParticle(250*sin(0.014/2.0),0,0,250,in[0]);
	ReconstructedParticle* invisible= Minus(RCCollider,visible);
	delete RCCollider;
	delete visible;
	return(invisible);
}



bool MyProcessor::POCutDetail(std::vector<ReconstructedParticle*> &PFOsWithoutIsoleps, std::vector<ReconstructedParticle*> &Isoleps,std::vector<ReconstructedParticle*> &choosed_lep,  Variable &vari){

	debug.Message(2,33,"in POCutDetail : get visible_vec, size of pfo_wo_lep",PFOsWithoutIsoleps.size());
	debug.Message(2,33,"in MCCutDetail : get visible_vec, size of lep",Isoleps.size());


	ReconstructedParticle* Collider=NewParticle(250*sin(0.014/2.0),0,0,250,choosed_lep[0]);
	ReconstructedParticle* pair    =Add(choosed_lep[0],choosed_lep[1]);
	debug.Message(2,12,"in obvRecoil: the pair are ",pair);
	debug.Message(2,12,"in obvRecoil: the Collider are ",Collider);
	ReconstructedParticle* recoil;
	recoil	=Minus(Collider,pair);
	debug.Message(2,12,"in obvRecoil: the recoil are ",recoil);

	float zmass=pair->getMass();
	debug.Message(2,33,"in POCutDetail : get zmass",zmass);
	vari.kcut_zmass=zmass;

	float zpt=calPT(pair);
	debug.Message(2,33,"in POCutDetail : get zpt",zpt);
	vari.kcut_zpt=zpt;


	std::vector<ReconstructedParticle*> visible_vec=PFOsWithoutIsoleps+Isoleps;
	debug.Message(2,33,"in POCutDetail : get visible_vec 1",visible_vec.size());

	debug.Message(2,33,"in POCutDetail : visible_vec 1",visible_vec);
	ReconstructedParticle* visible=getVisible(visible_vec);
	debug.Message(2,33,"in POCutDetail : get visible",visible);




	std::vector<ReconstructedParticle*> invisible_vec=PFOsWithoutIsoleps+Isoleps;
	invisible_vec.push_back(choosed_lep[0]);
	invisible_vec.push_back(choosed_lep[1]);
	debug.Message(2,33,"in POCutDetail : get invisible_vec",invisible_vec.size());

	ReconstructedParticle* invisible=getInVisible(invisible_vec);
	debug.Message(2,33,"in POCutDetail : get invisible",invisible);

	float invis_costheta=calCosTheta(invisible);
	vari.kcut_invis_costheta=invis_costheta;
	debug.Message(2,33,"in POCutDetail : get invis_costheta",invis_costheta);


	vari.lep_pair_costheta=calCosTheta(pair);
	vari.lep_pair_costheta_pair=calCosTheta(choosed_lep[0],choosed_lep[1]);


	if(choosed_lep[0]->getCharge()>0&&choosed_lep[1]->getCharge()<0){
		vari.lep_pair_costheta_track1=calCosTheta(choosed_lep[0]);
		vari.track1_pt               =calPT      (choosed_lep[0]);
		vari.track1_e                =choosed_lep[0]->getEnergy();
		vari.lep_pair_costheta_track2=calCosTheta(choosed_lep[1]);
		vari.track2_pt               =calPT      (choosed_lep[1]);
		vari.track2_e                =choosed_lep[1]->getEnergy();
	}
	else if(choosed_lep[0]->getCharge()<0&&choosed_lep[1]->getCharge()>0){
		vari.lep_pair_costheta_track1=calCosTheta(choosed_lep[1]);
		vari.track1_pt               =calPT      (choosed_lep[1]);
		vari.track1_e                =choosed_lep[1]->getEnergy();
		vari.lep_pair_costheta_track2=calCosTheta(choosed_lep[0]);
		vari.track2_pt               =calPT      (choosed_lep[0]);
		vari.track2_e                =choosed_lep[0]->getEnergy();
	}
	else{
		debug.Message(2,33,"in POCutDetail : track1/2 wrong charge");
	}


	float  recoil_mass=recoil->getMass();
	vari.kcut_recoil_mass=recoil_mass;
	debug.Message(2,33,"in POCutDetail : get recoil_mass",recoil_mass);


	float vis_e=visible->getEnergy();
	vari.kcut_vis_e=vis_e;
	debug.Message(2,33,"in POCutDetail : get visible_energy",vis_e);

	delete visible;
	delete invisible;
	delete Collider;
	delete pair;
	delete recoil;
	return(true);

}



