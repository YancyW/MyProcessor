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

template<typename TRECOIL>
bool MyProcessor::obvRecoil(std::vector<TRECOIL> in, std::vector<TRECOIL> &out, int PID, CEvent<TRECOIL> events) {

	CheckPOParticlePairType(IsoLeps,PID,pair);
	debug.Message(2,41,"in analyseRCParticle:  get the size is",pair.pair.size());

	if(RCCutMuon(pair.pair)){
		debug.Message(2,41,"in analyseRCParticle:  begin recoil for muon pair ");
		float recoil_mass=0,pair_mass=0;
		std::vector<TRECOIL> choosedpair;
		bool Jrecoil=obvRecoil(pair.pair,choosedpair,recoil_mass,pair_mass);
		if(Jrecoil){
			pair.getRecoilMass(recoil_mass);
			pair.getPairMass  (pair_mass  );
		}
	}
	return(false);
}

#endif


