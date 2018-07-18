#include "MyProcessor.h"
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>

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

using namespace lcio;
using namespace marlin;

void MyProcessor::TrackGetSource(std::vector<ReconstructedParticle*> &source, std::vector<std::vector<MCParticle*> >  &to, LCRelationNavigator* &relation)
{
	for( unsigned int i = 0; i < source.size(); i++ ){
		std::vector<MCParticle*> to_tmp;

		LCObjectVec frompars = relation->getRelatedToObjects(source[i]);

		for( unsigned int j = 0; j < frompars.size(); j++ ){
			MCParticle* recpfo = dynamic_cast< MCParticle* >( frompars[j] );
			to_tmp.push_back(recpfo);
		}
		to.push_back(to_tmp);
		to_tmp.clear();
	}

	return;
}



void MyProcessor::TrackGetObject(std::vector<MCParticle*> &source, std::vector<std::vector<ReconstructedParticle*> >  &to, LCRelationNavigator* &relation)
{
	for( unsigned int i = 0; i < source.size(); i++ ){
		std::vector<ReconstructedParticle*> to_tmp;

		LCObjectVec frompars = relation->getRelatedToObjects(source[i]);

		for( unsigned int j = 0; j < frompars.size(); j++ ){
			ReconstructedParticle* recpfo = dynamic_cast< ReconstructedParticle* >( frompars[j] );
			to_tmp.push_back(recpfo);
		}
		to.push_back(to_tmp);
		to_tmp.clear();
	}

	return;
}



int MyProcessor::TrackAnaMC(int event_num,  std::vector<MCParticle*> MCs, LCRelationNavigator* navmc, Infomation &mcinfo) {
	std::vector<std::vector<ReconstructedParticle*> >  MCToPFO;

	std::vector<MCParticle*> mc_muonminus=checkMCParticleType(MCs,13);
	std::vector<MCParticle*> mc_muonplus =checkMCParticleType(MCs,-13);
	std::vector<MCParticle*> mc_muon;
	mc_muon=mc_muonminus+mc_muonplus;

	stable_sort(mc_muon.begin(),mc_muon.end());
	reverse(mc_muon.begin(),mc_muon.end());


	if(mc_muon.size()<2){
		counter.pass_mc_num_false=1;
		return(event_num);
	}
	else{
		int kcounter=0;
		for( unsigned int i = 0; i < mc_muon.size(); i++ ){
			if(calEnergy(mc_muon[i])>5){
				kcounter++;
			}
		}
		if(kcounter<2){
			counter.pass_mc_energy_false=1;
			return(event_num);
		}
	}


	TrackGetObject(mc_muon,MCToPFO,navmc );
	for( unsigned int i = 0; i < mc_muon.size(); i++ ){
		bool ktrue=false;
		for( unsigned int j = 0; j < MCToPFO[i].size(); j++ ){
			if(std::abs(MCToPFO[i][j]->getType())==13){
				ktrue=true;
				break;
			}
		}
		if(ktrue!=true){
			counter.pass_mc_npfo++;
			mcinfo.data_muon_de.track1_e                =calEnergy(mc_muon[i]);
			mcinfo.data_muon_de.lep_pair_costheta_track1=calCosTheta(mc_muon[i]);
			return(event_num);
		}
	}

	return(0);
}




void MyProcessor::TrackPayBackMC(int event_num,  std::vector<MCParticle*> MCs, LCRelationNavigator* navmc, Infomation &mcinfo) {

	std::vector<std::vector<ReconstructedParticle*> >  MCToPFO;

	std::vector<MCParticle*> mc_muonminus=checkMCParticleType(MCs,13);
	std::vector<MCParticle*> mc_muonplus =checkMCParticleType(MCs,-13);
	std::vector<MCParticle*> mc_muon;
	mc_muon=mc_muonminus+mc_muonplus;

	stable_sort(mc_muon.begin(),mc_muon.end());
	TrackGetObject(mc_muon,MCToPFO,navmc );

	for( unsigned int i = 0; i < mc_muon.size(); i++ ){
		for( unsigned int j = 0; j < MCToPFO[i].size(); j++ ){
		}
	}
}




int MyProcessor::TrackAnaRC(int event_num,  std::vector<ReconstructedParticle*> PFOs, LCRelationNavigator* navpfo, Infomation &rcinfo) {

	std::vector<std::vector<MCParticle*> >             PFOFromMC;


	std::vector<ReconstructedParticle*> muonminus=checkPOParticleType(PFOs,13);
	std::vector<ReconstructedParticle*> muonplus=checkPOParticleType (PFOs,-13);
	std::vector<ReconstructedParticle*> muon;
	muon=muonminus+muonplus;

	stable_sort(muon.begin(),muon.end());
	reverse(muon.begin(),muon.end());

	TrackGetSource(muon,PFOFromMC,navpfo);
	bool ktrue=false;

	for( unsigned int i = 0; i < muon.size(); i++ ){
		for( unsigned int j = 0; j < PFOFromMC[i].size(); j++ ){
			if(std::abs(PFOFromMC[i][j]->getPDG())==13){
				ktrue=true;
				break;
			}
		}
		if(ktrue!=true){
			debug.Message(2,52,"RC Particles event_num",event_num);
			rcinfo.data_muon_de.track1_e                =calEnergy  (muon[i]);
			rcinfo.data_muon_de.lep_pair_costheta_track1=calCosTheta(muon[i]);
			return(event_num);
		}
	}

	if(muon.size()<2){
		counter.pass_po_num_false=1;
		return(event_num);
	}
	else{
		int kcounter=0;
		for( unsigned int i = 0; i < muon.size(); i++ ){
			if(calEnergy(muon[i])>5){
				kcounter++;
			}
		}
		if(kcounter<2){
			counter.pass_po_energy_false=1;
			return(event_num);
		}
	}


	return(0);

}



bool MyProcessor::TrackAnaISL(int event_num,  LCRelationNavigator* navpfo, std::vector<ReconstructedParticle*> pfoWOleps, std::vector<ReconstructedParticle*> leps, Infomation &poinfo) {

	std::vector<std::vector<MCParticle*> >             ISLFromMC;


	std::vector<ReconstructedParticle*> muonminus_isl=checkPOParticleType(leps,13);
	std::vector<ReconstructedParticle*> muonplus_isl =checkPOParticleType(leps,-13);
	std::vector<ReconstructedParticle*> muon_isl;
	muon_isl=muonminus_isl+muonplus_isl;


	if(muon_isl.size()<2){
		counter.pass_po_num_false=1;
	}
	else{
		int kcounter=0;
		for( unsigned int i = 0; i < muon_isl.size(); i++ ){
			if(calEnergy(muon_isl[i])>5){
				kcounter++;
			}
		}
		if(kcounter<2){
			counter.pass_po_energy_false=1;
		}
	}


	TrackGetSource(muon_isl,ISLFromMC,navpfo);
	for( unsigned int i = 0; i < muon_isl.size(); i++ ){
		bool ktrue=false;
		for( unsigned int j = 0; j < ISLFromMC[i].size(); j++ ){
			if(std::abs(ISLFromMC[i][j]->getPDG())==13){
				ktrue=true;
				break;
			}
		}
		if(ktrue!=true){
			counter.pass_isl_nmc++;
		}
		poinfo.data_muon_de.track1_e                =calEnergy  (muon_isl[i]);
		poinfo.data_muon_de.lep_pair_costheta_track1=calCosTheta(muon_isl[i]);
	}

}
