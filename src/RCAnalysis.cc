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

bool MyProcessor::analyseRCParticle( LCCollection* PFOCol,Infomation &info) {

	std::vector<ReconstructedParticle*> PFOs=checkPOFinalState(PFOCol);

	debug.Message(2,41,"in analyseRCParticle:  the FS PFOs size is",PFOs.size());

	std::vector<ReconstructedParticle*> leps,pfos;
	std::vector<ReconstructedParticle*> leps_recovery,pfos_recovery,choosedmuon_recovery;
	std::vector<ReconstructedParticle*> leps_left;
	checkRCIsoLeptons(PFOs,leps,pfos,info);
	debug.Message(2,41,"in analyseRCParticle:  get the ISO PFOs_size and WO_IsoLep size is",leps.size(),pfos.size());


	debug.Message(2,41,"in analyseRCParticle:  begin check muon-, the FS lep size is",leps.size());
	debug.Message(2,41,"in analyseRCParticle:  begin check muon-, the FS lep is",leps);
	std::vector<ReconstructedParticle*> muonminus=checkPOParticleType(leps,13);
	leps_left=leps-muonminus;
	debug.Message(2,41,"in analyseRCParticle:  begin check muon+, the muon minus size is",muonminus.size());
	std::vector<ReconstructedParticle*> muonplus=checkPOParticleType(leps_left,-13);
	leps_left=leps_left-muonplus;
	debug.Message(2,41,"in analyseRCParticle:  begin check muon+-, the muon plus size is",muonplus.size());
	std::vector<ReconstructedParticle*> muon;
	muon=muonminus+muonplus;
	debug.Message(2,41,"in analyseRCParticle:  begin cut for muon,the muon size is",muon.size());
	info.num_muon_de.num = muon.size();
	info.num_muon_de.num_plus = muonplus.size();
	info.num_muon_de.num_minus = muonminus.size();

	if(RCCutMuon(muon,muonplus,muonminus,info.data_muon_de)){
		debug.Message(2,41,"in analyseRCParticle:  begin recoil for muon pair ");
		std::vector<ReconstructedParticle*> choosedmuon;
		bool Jrecoil=obvRecoil(muon,choosedmuon);
		if(Jrecoil){
			debug.Message(2,41,"in analyseRCParticle:  begin POCutDetail");
			bool JPOCut=POCutDetail(pfos,leps_left,choosedmuon,info.data_muon_de);

			debug.Message(2,31,"in analysePhysicalObject:  befor recovery",choosedmuon);
			bool Jrecovery=checkRCIsoLepCone(choosedmuon,pfos,choosedmuon_recovery,pfos_recovery,info);
			debug.Message(2,31,"in analysePhysicalObject:  after recovery",choosedmuon_recovery);
			bool JPOCut_recovery=POCutDetail(pfos_recovery,leps_left,choosedmuon_recovery,info.data_muon_py_im);
		}
		for(int i=0;i<choosedmuon_recovery.size();i++){
			delete choosedmuon_recovery[i];
		}
	}




	return(true);
}

