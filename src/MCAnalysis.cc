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
#include "Debug.h"

using namespace lcio;
using namespace marlin;

bool MyProcessor::analyseMCParticle( LCCollection* Allmc,Infomation &info) {
	debug.Message(2,21,"in analyseMCParticle:  begin check FS_GenLevel ");
	std::vector<MCParticle*> FS_GenLevel=checkMCFinalState_GenLevel(Allmc);

	std::vector<MCParticle*> gen_pfos_recovery,gen_choosedmuon_recovery;
	std::vector<MCParticle*> gen_leps_left;

	debug.Message(2,21,"in analyseMCParticle:  already put Allmc data into vector, totally has n particles",FS_GenLevel);

	debug.Message(2,21,"in analyseMCParticle:  the FS_GenLevel size is",FS_GenLevel.size());
	std::vector<MCParticle*> gen_muonminus=checkMCParticleType(FS_GenLevel,13);
	debug.Message(2,21,"in analyseMCParticle:  the gen_muon- size is",gen_muonminus.size());
	std::vector<MCParticle*> gen_muonplus=checkMCParticleType(FS_GenLevel,-13);
	debug.Message(2,21,"in analyseMCParticle:  the gen muon+ size is",gen_muonplus.size());
	std::vector<MCParticle*> gen_muon;
	gen_muon=gen_muonminus+gen_muonplus;
	debug.Message(2,21,"in analyseMCParticle:  the gen muon is",gen_muon);
	info.num_muon_gen.num      = gen_muon.size();
	info.num_muon_gen.num_plus = gen_muonplus.size();
	info.num_muon_gen.num_minus= gen_muonminus.size();

	if(MCCutMuon(gen_muon)){
		debug.Message(2,21,"in analyseMCParticle:  begin recoil for muon pair");
		std::vector<MCParticle*> gen_choosedmuon;
		bool Jrecoil_gen=obvRecoil(gen_muon,gen_choosedmuon);
		std::vector<MCParticle*> gen_pfos=FS_GenLevel-gen_choosedmuon;
		if( Jrecoil_gen){
////		debug.Message(2,21,"in analyseMCParticle:  before recovery",gen_choosedmuon);
////		bool Jrecovery_gen=checkMCIsoLepCone(gen_choosedmuon,gen_pfos,gen_choosedmuon_recovery,gen_pfos_recovery,info);
////		debug.Message(2,21,"in analyseMCParticle:  after recovery",gen_choosedmuon_recovery);
////		bool JMCCut_gen=MCCutDetail(gen_pfos_recovery,gen_leps_left,gen_choosedmuon_recovery,info.data_muon_gen);
		}
		for(int i=0;i<gen_choosedmuon_recovery.size();i++){
			delete gen_choosedmuon_recovery[i];
		}
	}



    std::vector<MCParticle*> pfos_recovery,choosedmuon_recovery;
    std::vector<MCParticle*> leps_left;

    debug.Message(2,25,"in analyseMCParticle:  begin check FS_PythiaLevel ");
    std::vector<MCParticle*> FS_PythiaLevel=checkMCFinalState_PythiaLevel(Allmc);
    debug.Message(2,25,"in analyseMCParticle:  begin check muon-, the FS_PythiaLevel size is",FS_PythiaLevel);
    std::vector<MCParticle*> mc_muonminus=checkMCParticleType(FS_PythiaLevel,13);
    debug.Message(2,25,"in analyseMCParticle:  begin check muon+, the muon- size is",mc_muonminus.size());
    std::vector<MCParticle*> mc_muonplus=checkMCParticleType(FS_PythiaLevel,-13);
    debug.Message(2,25,"in analyseMCParticle:  begin check muon+-, the muon+ size is",mc_muonplus.size());
    std::vector<MCParticle*> mc_muon;
    mc_muon=mc_muonminus+mc_muonplus;
    debug.Message(2,25,"in analyseMCParticle:  begin cut for muon, the muon size is", mc_muon);
    info.num_muon.num       = mc_muon.size();
    info.num_muon.num_plus  = mc_muonplus.size();
    info.num_muon.num_minus = mc_muonminus.size();

    if(MCCutMuon(mc_muon)){
    	debug.Message(2,25,"in analyseMCParticle:  begin recoil for muon pair");
    	std::vector<MCParticle*> choosedmuon;
    	bool Jrecoil=obvRecoil(mc_muon,choosedmuon);
    	std::vector<MCParticle*> pfos=FS_PythiaLevel-choosedmuon;
    	if(Jrecoil){
    //		bool JMCCut=MCCutDetail(pfos,leps_left,choosedmuon,info.data_muon);


    		debug.Message(2,25,"in analyseMCParticle:  before recovery",choosedmuon);
    		bool Jrecovery=checkMCIsoLepCone(choosedmuon,pfos,choosedmuon_recovery,pfos_recovery,info);
    		debug.Message(2,25,"in analyseMCParticle:  after recovery",choosedmuon_recovery);
    		bool JMCCut_recovery=MCCutDetail(pfos_recovery,leps_left,choosedmuon_recovery,info.data_muon_recovery);
    	}
    	for(int i=0;i<choosedmuon_recovery.size();i++){
    		delete choosedmuon_recovery[i];
    	}
    }
    debug.Message(2,25,"");
	return(true);
}






