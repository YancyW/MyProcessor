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

bool MyProcessor::analysePhysicalObject(LCCollection* PFOs_col, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &info) {

	std::vector<ReconstructedParticle*> pfos_recovery,choosedmuon_recovery;
	std::vector<ReconstructedParticle*> leps_left;




	std::vector<ReconstructedParticle*> leps=checkPOFinalState(Isoleps);
	std::vector<ReconstructedParticle*> pfos=checkPOFinalState(PFOsWithoutIsoleps);
	std::vector<ReconstructedParticle*> PFOs=checkPOFinalState(PFOs_col);

	info.data_muon.pfo_num = PFOs.size();

	debug.Message(2,31,"in analysePOParticle:  get the ISO,  WO_IsoLep and PFOs_size are ",leps.size(),pfos.size(),PFOs.size());
	debug.Message(2,31,"in analysePOParticle:  get the PFOs are",pfos);
	info.num_lep.num = leps.size();




	std::vector<ReconstructedParticle*> muonminus=checkPOParticleType(leps,13);
	debug.Message(2,31,"in analysePhysicalObject:  begin check muon+, the muon minus size is",muonminus.size());
	std::vector<ReconstructedParticle*> muonplus=checkPOParticleType(leps,-13);
	debug.Message(2,31,"in analysePhysicalObject:  begin check muon+-, the muon plus size is",muonplus.size());
	std::vector<ReconstructedParticle*> muon;
	muon=muonminus+muonplus;
	debug.Message(2,31,"in analysePhysicalObject:  begin cut for muon,the muon size is",muon.size());
	info.num_muon.num = muon.size();
	info.num_muon.num_plus = muonplus.size();
	info.num_muon.num_minus = muonminus.size();

	if(POCutMuon(muon,muonplus,muonminus,info.data_muon)){
		debug.Message(2,31,"in analysePhysicalObject:  begin recoil for muon pair ");
		std::vector<ReconstructedParticle*> choosedmuon;
		bool Jrecoil=obvRecoil(muon,choosedmuon);
		leps_left=leps-choosedmuon;
		debug.Message(2,31,"in analysePhysicalObject:  left leps are",leps_left);
		if(Jrecoil){
			bool JPOCut=POCutDetail(pfos,leps_left,choosedmuon,info.data_muon);


			debug.Message(2,31,"in analysePhysicalObject:  befor recovery",choosedmuon);
			bool Jrecovery=checkRCIsoLepCone(choosedmuon,pfos,choosedmuon_recovery,pfos_recovery,info);
			debug.Message(2,31,"in analysePhysicalObject:  after recovery",choosedmuon_recovery);
			bool JPOCut_recovery=POCutDetail(pfos_recovery,leps_left,choosedmuon_recovery,info.data_muon_recovery);
		}
		for(int i=0;i<choosedmuon_recovery.size();i++){
			delete choosedmuon_recovery[i];
		}
	}




	return(true);
}

