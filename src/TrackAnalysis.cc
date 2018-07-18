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
#include "MCOperator.h"

using namespace lcio;
using namespace marlin;

bool MyProcessor::analyseTrack(int event_num,  LCCollection* MC_col, LCCollection* PFOs_col, LCRelationNavigator* navpfo, LCRelationNavigator* navmc, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &mcinfo,Infomation &rcinfo, Infomation &poinfo) {

	debug.Message(2,50,"begin analyseTrack");


	debug.Message(2,51,"");
	debug.Message(2,51,"event num",event_num);


	std::vector<ReconstructedParticle*> PFOs     = checkPOFinalState(PFOs_col);
	std::vector<MCParticle*>            MCs      = checkMCFinalState_PythiaLevel(MC_col);

	std::vector<ReconstructedParticle*> leps  ,  pfoWOleps;
	leps     = checkPOFinalState(Isoleps);
	pfoWOleps= checkPOFinalState(PFOsWithoutIsoleps);




	int mctest = TrackAnaMC (event_num,MCs ,navmc ,mcinfo);
	int rctest = TrackAnaRC (event_num,PFOs,navpfo,rcinfo);
	if(rctest!=0){
		TrackPayBackMC (event_num,MCs ,navmc ,mcinfo);
	}
//	TrackAnaISL(event_num,navpfo,pfoWOleps,leps,poinfo);


	return(true);
}

