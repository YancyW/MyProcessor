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
#include "Recoil_mass.h"

using namespace lcio;
using namespace marlin;

bool MyProcessor::analyseTrack(int event_num,  LCCollection* PFOs_col, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &info) {

	debug.Message(2,50,"begin analyseTrack");


	std::vector<ReconstructedParticle*> PFOs     = checkPOFinalState(PFOs_col);
	std::vector<ReconstructedParticle*> leps     = checkPOFinalState(Isoleps);
	std::vector<ReconstructedParticle*> pfoWOleps= checkPOFinalState(PFOsWithoutIsoleps);

	if(leps.size()<2){
		debug.Message(2,51,"lep size < 2, The event number is",event_num);
		debug.Message(2,51,"The PFOs size is",PFOs.size());
	}

////debug.Message(2,50,"PFOs size",PFOs.size());
////for(int i=0;i<PFOs.size();i++){
////	debug.Message(2,51,"Track size",PFOs[i]->getTracks().size());
////	for(int j=0;j<PFOs[i]->getTracks().size();j++){
////		debug.Message(2,51,"Track Type",PFOs[i]->getTracks()[j]->getType());
////	}
////}

	info.data_muon.pfo_num = PFOs.size();




	return(true);
}

