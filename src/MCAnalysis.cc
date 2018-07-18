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
#include "CMC.h"

using namespace lcio;
using namespace marlin;
using ToolSet::operator<<;

bool MyProcessor::analyseMCParticle( LCCollection* Allmc,LCCollection* MCsWithoutIsoPhoton_Collection,LCCollection* MCsPhoton_Collection,LCCollection* MCsForwardPhoton_Collection,   Infomation &info) {
	debug.Message(2,21,"in analyseMCParticle:  begin check FS_GenLevel ");

	//generate all basic sorts 
	std::vector<MCParticle*> MCs_TFS                =ToolSet::CMC::Get_MCParticle(Allmc);
	//std::vector<MCParticle*> MCs                   =ToolSet::CMC::Pass_DetectorAngle(MCs_TFS);
	std::vector<MCParticle*> MCs = MCs_TFS;
	std::vector<MCParticle*> hs_FS                 =ToolSet::CMC::Get_MC_HardScattering_FS(MCs);
	std::vector<MCParticle*> py_FS                 =ToolSet::CMC::Get_MC_PythiaShowering_FS(MCs);
	std::vector<MCParticle*> de_FS                 =ToolSet::CMC::Get_MC_DetectorSimulating_FS(MCs);

	// photon related

	std::vector<MCParticle*> MCsWithoutIsoPhoton   =ToolSet::CMC::Get_MCParticle(MCsWithoutIsoPhoton_Collection);
	std::vector<MCParticle*> MCsCenterPhoton       =ToolSet::CMC::Get_MCParticle(MCsPhoton_Collection);
	std::vector<MCParticle*> MCsForwardPhoton      =ToolSet::CMC::Get_MCParticle(MCsForwardPhoton_Collection);


	std::vector<MCParticle*> MCsPhoton             =MCsCenterPhoton + MCsForwardPhoton;
	std::vector<MCParticle*> hs_MCsPhoton          =ToolSet::CMC::Get_MC_HardScattering_FS    (MCsPhoton       );
	std::vector<MCParticle*> py_MCsPhoton          =ToolSet::CMC::Get_MC_PythiaShowering_FS   (MCsPhoton       );
	std::vector<MCParticle*> de_MCsPhoton          =ToolSet::CMC::Get_MC_DetectorSimulating_FS(MCsPhoton       );




	// for hard scattering

	std::vector<MCParticle*> hs_muon, hs_muon_choosed,  hs_womuon, hs_muon_combined, hs_womuon_combined, hs_muon_left;


	std::vector<MCParticle*> hs_higgs     =ToolSet::CMC::Get_MCParticleType(hs_FS,25);
	std::vector<MCParticle*> hs_W         =ToolSet::CMC::Get_MCParticleType_Abs(hs_FS,24);
	std::vector<MCParticle*> hs_Z         =ToolSet::CMC::Get_MCParticleType(hs_FS,23);
	std::vector<MCParticle*> hs_gamma     =ToolSet::CMC::Get_MCParticleType(hs_FS,22);

	std::vector<MCParticle*> hs_muonminus =ToolSet::CMC::Get_MCParticleType(hs_FS,13);
	std::vector<MCParticle*> hs_muonplus  =ToolSet::CMC::Get_MCParticleType(hs_FS,-13);
	std::vector<MCParticle*> hs_tauminus  =ToolSet::CMC::Get_MCParticleType_Abs(hs_FS,15);
	std::vector<MCParticle*> hs_elecminus =ToolSet::CMC::Get_MCParticleType_Abs(hs_FS,11);
	std::vector<MCParticle*> hs_quark     =ToolSet::CMC::Get_MCParticleType(hs_FS,"quark");


	if(hs_higgs.size()>0){
		info.data_muon_hs.higgs_channel = ToolSet::CMC::Get_DecayChannel(hs_higgs[0]);
	}

	hs_muon=hs_muonminus+hs_muonplus;
	info.num_muon_hs.num      = hs_muon.size();
	info.num_muon_hs.num_plus = hs_muonplus.size();
	info.num_muon_hs.num_minus= hs_muonminus.size();

	if(MCCutMuon(hs_muon)){
		debug.Message(2,21,"in analyseMCParticle:  begin recoil for muon pair");
		bool Jrecoil=obvRecoil(hs_muon,hs_muon_choosed);
		hs_womuon    = hs_FS   - hs_muon_choosed;
		hs_muon_left = hs_muon - hs_muon_choosed;
		if( Jrecoil){
			bool Jcombinephoton = checkMCIsoLepCone(hs_muon_choosed, hs_womuon, hs_muon_combined, hs_womuon_combined, info);
			bool JMCCut         = MCCutDetail      (hs_womuon_combined, hs_muon_left, hs_muon_combined, info.data_muon_hs);
			MCCutPhoton(hs_MCsPhoton, info.data_muon_hs);
		}
		for(int i=0;i<hs_muon_combined.size();i++){
			delete hs_muon_combined[i];
		}
	}



	//use Pythia FS muon, but it should come from Z
	// get FS particle
//	std::cout << "pythia FS muon" << std::endl;
	_muon_chain.Get_ChainVec(hs_muon,_navpfo);
    std::vector<std::vector<MCParticle*> >                           isr_end_muon      = _muon_chain.Get_MCEnd();

	_photon_chain.Get_ChainVec(hs_MCsPhoton,_navpfo);
    std::vector<std::vector<MCParticle*> >                           isr_end_photon    = _photon_chain.Get_MCEnd();

	std::vector<MCParticle*> hs_end_muon, py_ex_MCsPhoton, py_ex_FS;
	std::vector<MCParticle*> py_ex_muon, py_ex_muon_choosed, py_ex_womuon, py_ex_muon_combined, py_ex_womuon_combined, py_ex_muon_left;
    for(int i=0;i<isr_end_muon.size();i++){
		std::vector<MCParticle*> hs_end_muon_tmp =ToolSet::CMC::Get_MCParticleType_Abs(isr_end_muon[i],13);
		hs_end_muon = hs_end_muon + hs_end_muon_tmp;
		py_ex_FS   = py_ex_FS   + isr_end_muon[i];
    }
	ToolSet::CMC::Remove_Duplication(hs_end_muon);
    for(int i=0;i<isr_end_photon.size();i++){
		std::vector<MCParticle*> py_ex_MCsPhoton_tmp =ToolSet::CMC::Get_MCParticleType_Abs(isr_end_photon[i],22);
		py_ex_MCsPhoton= py_ex_MCsPhoton+ py_ex_MCsPhoton_tmp;
		py_ex_FS   = py_ex_FS   + isr_end_photon[i];
    }
	ToolSet::CMC::Remove_Duplication(py_ex_MCsPhoton);
	ToolSet::CMC::Remove_Duplication(py_ex_FS);
	py_ex_womuon = py_ex_FS - hs_end_muon;




	std::vector<MCParticle*> py_ex_muonminus   =ToolSet::CMC::Get_MCParticleType(hs_end_muon,13);
	std::vector<MCParticle*> py_ex_muonplus    =ToolSet::CMC::Get_MCParticleType(hs_end_muon,-13);

	py_ex_muon=py_ex_muonminus+py_ex_muonplus;
	info.num_muon_py_ex.num       =py_ex_muon.size();
	info.num_muon_py_ex.num_plus  =py_ex_muonplus.size();
	info.num_muon_py_ex.num_minus =py_ex_muonminus.size();

	if(MCCutMuon(py_ex_muon)){
		debug.Message(2,25,"in analyseMCParticle:  begin recoil for muon pair");
		bool Jrecoil=obvRecoil(py_ex_muon,py_ex_muon_choosed);
		py_ex_womuon    = py_ex_FS   - py_ex_muon_choosed;
		py_ex_muon_left = py_ex_muon - py_ex_muon_choosed;
		if(Jrecoil){
			bool Jcombinephoton = checkMCIsoLepCone(py_ex_muon_choosed, py_ex_womuon, py_ex_muon_combined, py_ex_womuon_combined, info);
			bool JMCCut         = MCCutDetail      (py_ex_womuon_combined, py_ex_muon_left, py_ex_muon_combined, info.data_muon_py_ex);
			MCCutPhoton(py_ex_MCsPhoton,info.data_muon_py_ex);
		}
		for(int i=0;i<py_ex_muon_combined.size();i++){
			delete py_ex_muon_combined[i];
		}
	}
	debug.Message(2,25,"");






	// for all pythia final state
//	std::cout << "pythia all FS" << std::endl;
	std::vector<MCParticle*> py_im_muon, py_im_muon_choosed, py_im_womuon, py_im_muon_combined, py_im_womuon_combined, py_im_muon_left;

	std::vector<MCParticle*> py_im_muonminus   =ToolSet::CMC::Get_MCParticleType(py_FS,13);
	std::vector<MCParticle*> py_im_muonplus    =ToolSet::CMC::Get_MCParticleType(py_FS,-13);

	py_im_muon=py_im_muonminus+py_im_muonplus;
	info.num_muon_py_im.num       =py_im_muon.size();
	info.num_muon_py_im.num_plus  =py_im_muonplus.size();
	info.num_muon_py_im.num_minus =py_im_muonminus.size();

	if(MCCutMuon(py_im_muon)){
		debug.Message(2,25,"in analyseMCParticle:  begin recoil for muon pair");
		bool Jrecoil=obvRecoil(py_im_muon,py_im_muon_choosed);
		py_im_womuon    = py_FS      - py_im_muon_choosed;
		py_im_muon_left = py_im_muon - py_im_muon_choosed;
		if(Jrecoil){
			bool Jcombinephoton = checkMCIsoLepCone(py_im_muon_choosed, py_im_womuon, py_im_muon_combined, py_im_womuon_combined, info);
			bool JMCCut         = MCCutDetail      (py_im_womuon_combined, py_im_muon_left, py_im_muon_combined, info.data_muon_py_im);
			MCCutPhoton(py_MCsPhoton,info.data_muon_py_im);
		}
		for(int i=0;i<py_im_muon_combined.size();i++){
			delete py_im_muon_combined[i];
		}
	}
	debug.Message(2,25,"");




	// for all detecter final state
//	std::cout << "detecter all FS" << std::endl;
	std::vector<MCParticle*> de_muon, de_muon_choosed, de_womuon, de_muon_combined, de_womuon_combined, de_muon_left;


	std::vector<MCParticle*> de_muonminus   =ToolSet::CMC::Get_MCParticleType(de_FS,13);
	std::vector<MCParticle*> de_muonplus    =ToolSet::CMC::Get_MCParticleType(de_FS,-13);

	de_muon=de_muonminus+de_muonplus;
	info.num_muon_de.num       =de_muon.size();
	info.num_muon_de.num_plus  =de_muonplus.size();
	info.num_muon_de.num_minus =de_muonminus.size();

	if(MCCutMuon(de_muon)){
		debug.Message(2,25,"in analyseMCParticle:  begin recoil for muon pair");
		bool Jrecoil=obvRecoil(de_muon,de_muon_choosed);
		de_womuon    = de_FS   - de_muon_choosed;
		de_muon_left = de_muon - de_muon_choosed;
		if(Jrecoil){
			bool Jcombinephoton = checkMCIsoLepCone(de_muon_choosed, de_womuon, de_muon_combined, de_womuon_combined, info);
			bool JMCCut         = MCCutDetail      (de_womuon_combined, de_muon_left, de_muon_combined, info.data_muon_de);
			MCCutPhoton(de_MCsPhoton, info.data_muon_de);
		}
		for(int i=0;i<de_muon_combined.size();i++){
			delete de_muon_combined[i];
		}
	}
	debug.Message(2,25,"");
	return(true);
}






