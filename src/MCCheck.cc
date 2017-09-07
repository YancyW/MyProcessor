#include "MyProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/Vertex.h>
#include <IMPL/MCParticleImpl.h>
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

using namespace lcio;
using namespace marlin;

std::vector<MCParticle*> MyProcessor::checkMCFinalState_GenLevel(LCCollection* AllMC) {
	debug.Message(2,23,"in checkMCFinalState_GenLevel:  begin ");
	std::vector<MCParticle*> FS_GenLevel;
	int nMC = AllMC->getNumberOfElements();
	for( int i = 0; i < nMC; i++ ){
		debug.Message(2,23,"in checkMCFinalState_GenLevel:  change ith particle",i);
		MCParticle* MC = dynamic_cast< MCParticle* >( AllMC->getElementAt(i) );
		if(MC->getGeneratorStatus()==2){
			if(_processList=="nh"){
				if(MC->getParents().size()!=0&&std::abs(MC->getPDG())!=12&&std::abs(MC->getPDG())!=14&&std::abs(MC->getPDG())!=16){
					FS_GenLevel.push_back(MC);
				}
			}
			else{
				if(MC->getParents().size()==0&&std::abs(MC->getPDG())!=12&&std::abs(MC->getPDG())!=14&&std::abs(MC->getPDG())!=16){
					FS_GenLevel.push_back(MC);
				}
			}
		}
		else if(MC->getGeneratorStatus()==1){
			break;
		}
	}
	debug.Message(2,23,"in checkMCFinalState_GenLevel:  end, totally get n particle", FS_GenLevel.size());
	return(FS_GenLevel);
}



std::vector<MCParticle*> MyProcessor::checkMCFinalState_PythiaLevel(LCCollection* AllMC) {
	std::vector<MCParticle*> FS_PythiaLevel;
	int nMC = AllMC->getNumberOfElements();
	for( int i = 0; i < nMC; i++ ){
		MCParticle* MC = dynamic_cast< MCParticle* >( AllMC->getElementAt(i) );
		if(MC->getGeneratorStatus()==1&&std::abs(MC->getPDG())!=12&&std::abs(MC->getPDG())!=14&&std::abs(MC->getPDG())!=16){
			FS_PythiaLevel.push_back(MC);
		}
	}
	return(FS_PythiaLevel);
}


std::vector<MCParticle*> MyProcessor::checkMCParticleType(std::vector<MCParticle*> MCs,signed int PDG) {
	int num = MCs.size();
	std::vector<MCParticle*> output;
	for( int i = 0; i < num; i++ ){
		MCParticle* MC = MCs[i];
		if(MC->getPDG()==PDG){
			output.push_back(MC);
		}
	}
	return(output);
}




MCParticle* MyProcessor::getVisible(std::vector<MCParticle*> in){

	int num=in.size();

	MCParticle* visible;
	MCParticle* tmp;
	MCParticle* tmp1;
	if(num<=0){
		visible=NewParticle(0,0,0,0,visible);
		return(visible);
	}

	TLorentzVector VS(0,0,0,0);
	for(int i=0;i<num;i++){
		TLorentzVector Vnew(in[i]->getMomentum(),in[i]->getEnergy());
		VS+=Vnew;
	}

	visible = NewParticle(VS[0],VS[1],VS[2],VS[3],visible);
	return(visible);

}



MCParticle* MyProcessor::getInVisible(std::vector<MCParticle*> in){
	MCParticle* visible = getVisible(in);
	MCParticle* RCCollider=NewParticle(250*sin(0.014/2.0),0,0,250.0,in[0]);

	MCParticle* invisible= Minus(RCCollider,visible);
	delete RCCollider;
	delete visible;
	return(invisible);
}



bool MyProcessor::MCCutDetail(std::vector<MCParticle*> &PFOsWithoutIsoleps, std::vector<MCParticle*> &Isoleps,std::vector<MCParticle*> &choosed_lep,  Variable &vari){

	debug.Message(2,33,"in MCCutDetail : get visible_vec, size of pfo_wo_lep",PFOsWithoutIsoleps.size());
	debug.Message(2,33,"in MCCutDetail : get visible_vec, size of lep",Isoleps.size());


	MCParticle* Collider=NewParticle(250*sin(0.014/2.0),0,0,250,choosed_lep[0]);
	MCParticle* pair    =Add(choosed_lep[0],choosed_lep[1]);
	debug.Message(2,34,"in MCCutDetail: the choosed_lep are ",choosed_lep[0]);
	debug.Message(2,34,"in MCCutDetail: the choosed_lep are ",choosed_lep[1]);
	debug.Message(2,34,"in MCCutDetail: the pair are ",pair);
	debug.Message(2,34,"in MCCutDetail: the Collider are ",Collider);
	MCParticle* recoil;
	recoil	=Minus(Collider,pair);
	debug.Message(2,33,"in MCCutDetail: the recoil are ",recoil);

	float zmass=pair->getMass();
	debug.Message(2,33,"in MCCutDetail : get zmass",zmass);
	vari.kcut_zmass=zmass;

	float zpt=calPT(pair);
	debug.Message(2,33,"in MCCutDetail : get zpt",zpt);
	vari.kcut_zpt=zpt;


	std::vector<MCParticle*> visible_vec=PFOsWithoutIsoleps+Isoleps;
	debug.Message(2,35,"in MCCutDetail : get visible_vec size",visible_vec.size());

	debug.Message(2,35,"in MCCutDetail : visible_vec",visible_vec);
	MCParticle* visible=getVisible(visible_vec);
	debug.Message(2,35,"in MCCutDetail : get visible",visible);




	std::vector<MCParticle*> invisible_vec=PFOsWithoutIsoleps+Isoleps;
	invisible_vec.push_back(choosed_lep[0]);
	invisible_vec.push_back(choosed_lep[1]);
	debug.Message(2,33,"in MCCutDetail : get invisible_vec size",invisible_vec.size());

	debug.Message(2,33,"in MCCutDetail : get invisible_vec ",invisible_vec);

	MCParticle* invisible=getInVisible(invisible_vec);
	debug.Message(2,34,"in MCCutDetail : get invisible",invisible);

	float invis_costheta=calCosTheta(invisible);
	vari.kcut_invis_costheta=invis_costheta;
	debug.Message(2,33,"in MCCutDetail : get invis_costheta",invis_costheta);


	vari.lep_pair_costheta=calCosTheta(pair);
	vari.lep_pair_costheta_pair=calCosTheta(choosed_lep[0],choosed_lep[1]);


	if(choosed_lep[0]->getCharge()>0&&choosed_lep[1]->getCharge()<0){
		vari.lep_pair_costheta_track1=calCosTheta(choosed_lep[0]);
		vari.lep_pair_costheta_track2=calCosTheta(choosed_lep[1]);
	}
	else if(choosed_lep[0]->getCharge()<0&&choosed_lep[1]->getCharge()>0){
		vari.lep_pair_costheta_track1=calCosTheta(choosed_lep[1]);
		vari.lep_pair_costheta_track2=calCosTheta(choosed_lep[0]);
	}
	else{
		debug.Message(2,33,"in MCCutDetail : track1/2 wrong charge");
	}



	float  recoil_mass=recoil->getMass();
	vari.kcut_recoil_mass=recoil_mass;
	debug.Message(2,34,"in MCCutDetail : get recoil",recoil);
	debug.Message(2,34,"in MCCutDetail : get recoil_mass",recoil_mass);


	float vis_e=visible->getEnergy();
	vari.kcut_vis_e=vis_e;
	debug.Message(2,33,"in MCCutDetail : get visible_energy",vis_e);

	delete visible;
	delete invisible;
	delete Collider;
	delete pair;
	delete recoil;
	return(true);

}


bool MyProcessor::checkMCIsoLepCone(std::vector<MCParticle*> IsoLep, std::vector<MCParticle*> PFOs_WO_IsoLep, std::vector<MCParticle*> &NewIsoLep, std::vector<MCParticle*> &NewPFOs_WO_IsoLep, Infomation &info)
{
	debug.Message(2,34,"in checkMCIsoLepCone: IsoLep",IsoLep);
	debug.Message(2,34,"in checkMCIsoLepCone: PFOsWithoutIsoleps",PFOs_WO_IsoLep);
	float Mass;
	int nlep = IsoLep.size();
	NewPFOs_WO_IsoLep = PFOs_WO_IsoLep;
	int npfo = NewPFOs_WO_IsoLep.size();
	if(nlep<1) {
		return false;
	}
	for( int i = 0; i < nlep; i++ )
	{
		MCParticleImpl* NewLep = new MCParticleImpl; 
		float charge  = IsoLep[i]->getCharge();
		float pid     = IsoLep[i]->getPDG();
		float Pnew[3] = {IsoLep[i]->getMomentum()[0],IsoLep[i]->getMomentum()[1],IsoLep[i]->getMomentum()[2]} ;
		float Enew    = IsoLep[i]->getEnergy() ;
		std::vector<MCParticle*>::iterator it = NewPFOs_WO_IsoLep.begin();
		while (it!=NewPFOs_WO_IsoLep.end())
		{
			if (IsInLepCone( IsoLep[i], *it,info)) 
			{
				debug.Message(2,34,"in checkMCIsoLepCone: conbined photon",*it);
				float Ppfo[3] = {(*it)->getMomentum()[0],(*it)->getMomentum()[1],(*it)->getMomentum()[2]};
				float Epfo = (*it)->getEnergy();
				Pnew[0] = Pnew[0] + Ppfo[0];
				Pnew[1] = Pnew[1] + Ppfo[1];
				Pnew[2] = Pnew[2] + Ppfo[2];
				Enew    = Enew    + Epfo   ;
				NewPFOs_WO_IsoLep.erase(it);
			}
			else{
				it++;
			}
		}
		float Mass2=Enew*Enew-Pnew[0]*Pnew[0]-Pnew[1]*Pnew[1]-Pnew[2]*Pnew[2];
		if(Mass2>=0){
			Mass=sqrt(Mass2);
		}
		else{
			Mass=-sqrt(-Mass2);
		}
		NewLep->setMomentum(Pnew);
		NewLep->setMass    (Mass);
		NewLep->setPDG     (pid);
		NewLep->setCharge  (charge);
		NewIsoLep.push_back(dynamic_cast<MCParticle*> (NewLep));
	}
	debug.Message(2,34,"in checkMCIsoLepCone: NewIsoLep",NewIsoLep);
	debug.Message(2,34,"in checkMCIsoLepCone: NewPFOsWithoutIsoleps",NewPFOs_WO_IsoLep);
	return true;
}

bool MyProcessor::IsInLepCone( MCParticle* lep, MCParticle* pfo , Infomation &info) {
	float coneE = 0;
	if(pfo->getPDG()!=22){
		return(false);
	}

	TVector3 P_pfo( pfo->getMomentum() );

	TVector3 P_lep( lep->getMomentum() );

	float cosTheta = P_lep.Dot( P_pfo )/(P_lep.Mag()*P_pfo.Mag());
	debug.Message(2,34,"in IsInLepCone: cosConeAngle",info.cut.cosConeAngle);
	if ( cosTheta >= info.cut.cosConeAngle )
	{
		return true;
	}
	return false;
}

