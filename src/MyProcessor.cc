#include "MyProcessor.h"

//STL
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


using namespace lcio;
using namespace marlin;
using namespace std;

MyProcessor aMyProcessor;

MyProcessor::MyProcessor() : Processor( "MyProcessor" ) {

	//modify processor description
	_description = "MyProcessor does whatever it does ...";

	//register steering parameters: name, description, class-variable, default value
	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			"InputAllPFOsCollection", 
			"Name of the PFOs collection",
			_colAllPFOs,
			std::string("PandoraPFOs") );

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			"InputIsolepsCollection",
			"Name of the isolated lepton collection",
			_colIsoleps,
			std::string("Isoleps") );

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			"InputPFOsWithoutIsolepsCollection", 
			"Name of the PFOs after isolated lepton tagging collection",
			_colPFOsWithoutIsoleps,
			std::string("PFOsWithoutIsoleps") );

    registerInputCollection( LCIO::LCRELATION,
    		"MCPFORelation",
    		"Relation between MC and PFO particles",
    		_mcpfoRelation,
    		std::string("RecoMCTruthLink"));

    registerInputCollection( LCIO::MCPARTICLE,
    		"MCParticleCollection", 
    		"Name of the MC particle collection",
    		_colMC,
    		std::string("MCParticles") );

//  registerInputCollection( LCIO::MCPARTICLE,
//  		"MCParticleCollection", 
//  		"Name of the MC particle collection",
//  		_colMC,
//  		std::string("MCParticlesSkimmed") );

	registerProcessorParameter( "hmass",
			"For counting energetic charged particles in the collection after isolated lepton tagging",
			_hmass,
			float(125) );

	registerProcessorParameter( "ProcessListFile",
			"Name of process list file",
			_processList,
			std::string("procid500.txt") ) ;

	registerProcessorParameter( "RootFileName",
			"Name of Root file (default: output.root)",
			_rootfilename, 
			std::string("../result/output.root") );
}


void MyProcessor::init() { 

	//streamlog_out(DEBUG) << "   init called  " << std::endl;
	std::cout << "init called" << std::endl;
	std::cout << "the output root file is " << _rootfilename << std::endl; 

	// usually a good idea to
	printParameters();

	// make Ntuple
	makeNTuple();

	_nRun = 0;
	_nEvt = 0;
	counter.pass_MC = 0;
	counter.pass_RC = 0;
	counter.pass_PO = 0;
	counter.pass_all = 0;
	debug.Read_Debug();
	pass_something = 0;


	//TMVA::Reader* reader = new TMVA::Reader( "!Color:!Silent" );    
	//  // add input variables
	//   reader->AddVariable( "lepton_pair_invariant_mass",          &_lep_pair_mass); 
	//   reader->AddVariable( "lepton_pair_costheta",                &_lep_pair_constheta); 
	//   reader->AddVariable( "lepton_pair_costheta_for_pair",       &_lep_pair_constheta_for_pair); 
	//   reader->AddVariable( "lepton_pair_track1",                  &_lep_pair_track1); 
	//   reader->AddVariable( "lepton_pair_track2",                  &_lep_pair_track2);     
	//  
	//  
	//  
	//  // book the reader (method, weights)
	//  TString methodName = "MLP method";
	//  TString weightfile = "./MVA_weight/MVA_Z_weights.xml";
	//  reader->BookMVA( methodName, weightfile ); 
	//  _readers.push_back(reader);


}

void MyProcessor::processRunHeader( LCRunHeader* run ) { 
	_nRun++;
} 

void MyProcessor::processEvent( LCEvent * evt ) 
{ 
	debug.Message(1,1,"");
	debug.Message(1,1,"");
	debug.Message(1,1,"");
	debug.Message(1,1,"*************in processEvent: begin analysis *************************");
	debug.Message(1,1,"in processEvent: read collections ");
	_nEvt++;
	if( _nEvt % 1000 == 0 ) std::cout << "processing event "<< _nEvt << std::endl;

	// this gets called for every event 
	// usually the working horse ...

	// Clear memory
	memset( &info_po, 0, sizeof(info_po) );
	memset( &info_mc, 0, sizeof(info_mc) );
	memset( &info_rc, 0, sizeof(info_rc) );

	info_po.init();
	info_mc.init();
	info_rc.init();

	// fill histogram from LCIO data :
	LCCollection* AllPFOs = evt->getCollection( _colAllPFOs ) ;
	LCCollection* AllMC = evt->getCollection( _colMC ) ;
   _navpfo = new LCRelationNavigator( evt->getCollection( _mcpfoRelation ) );

	LCCollection* Isoleps = 0;
	try{
		Isoleps = evt->getCollection( _colIsoleps );
	}
	catch(...){
		cout << "no isolated leptons in this event" << endl;
	}
	LCCollection* PFOsWithoutIsoleps = 0;
	try{
		PFOsWithoutIsoleps = evt->getCollection( _colPFOsWithoutIsoleps );
	}
	catch(...){
		cout << "no pfos after isolated lepton tagging in this event" << endl;
	}

	debug.Message(1,1,"in processEvent:  begin analyseMCParticle ");

	bool JMC,JRC,JPFO,JTrack;
	if(debug.debug[900]==true){
		JMC=analyseMCParticle(AllMC,info_mc);
		if(JMC){
			counter.pass_MC++;
		}
		else{
			debug.Message(1,1,"in processEvent: not pass analyseMCParticle ");
		}

	}




	if(debug.debug[901]==true){
		debug.Message(1,1,"in processEvent:  begin analyseReconstructedParticle");
		JRC=analyseRCParticle(AllPFOs,info_rc);
		if(JRC){
			counter.pass_RC++;
		}
		else{
			debug.Message(1,1,"in processEvent: not pass analyseReconstructedParticle");
		}
	}


	debug.Message(1,1,"in processEvent:  begin analysePhysicalObject ");
	if(debug.debug[902]==true){
		JPFO=analysePhysicalObject(AllPFOs,PFOsWithoutIsoleps,Isoleps,info_po);
		if(JPFO){
			counter.pass_PO++;
		}
		else{
			debug.Message(1,1,"in processEvent: not pass analysePhysicalObject");
		}
	}

	if(debug.debug[903]==true){
		JTrack=analyseTrack(_nEvt,AllPFOs,PFOsWithoutIsoleps, Isoleps, info_track);
		if(JTrack){
			counter.pass_Track++;
		}
		else{
			debug.Message(1,1,"in processEvent: not pass analyseTrack");
		}
	}


	debug.Message(1,1,"in processEvent:  begin fill data ");
	// Fill the event data 
	counter.evt = _nEvt;

	if(JMC && JRC && JPFO) {
		counter.pass_all++;
	}
	else{
	}

	_datatrain->Fill();

	debug.Message(1,1,"in processEvent:  begin delete variables in processEvent");
	delete _navpfo;
	//-- note: this will not be printed if compiled w/o MARLINDEBUG=1 !
	streamlog_out(DEBUG) << "   processing event: " << evt->getEventNumber() 
		<< "   in run:  " << evt->getRunNumber()   
		<< std::endl ;

}


void MyProcessor::check( LCEvent * evt ) { 
	// nothing to check here - could be used to fill checkplots in reconstruction processor
}


void MyProcessor::end(){ 

	//   std::cout << "MyProcessor::end()  " << name() 
	// 	    << " processed " << _nEvt << " events in " << _nRun << " runs "
	// 	    << std::endl ;

	_otfile->cd();
	_datatrain->Write();
	_otfile->Close();


}

void MyProcessor::makeNTuple() {

	//Output root file
	_otfile = new TFile( _rootfilename.c_str() , "RECREATE" );

	//Define root tree
	_datatrain= new TTree( "datatrain" , "events" );
	//event number
	_datatrain->Branch( "evt"     , &counter.evt, "evt" );
	_datatrain->Branch( "pass_MC" , &counter.pass_MC , "pass_MC" );
	_datatrain->Branch( "pass_RC" , &counter.pass_RC , "pass_RC" );
	_datatrain->Branch( "pass_PO" , &counter.pass_PO , "pass_PO" );
	_datatrain->Branch( "pass_all", &counter.pass_all, "pass_all" );



	//partraine number
	_datatrain->Branch( "mc_muon_num_gen"                           , &info_mc.num_muon_gen.num                              , "mc_muon_num_gen"                             );
	_datatrain->Branch( "mc_muon_num_gen_plus"                      , &info_mc.num_muon_gen.num_plus                         , "mc_muon_num_gen_plus"                        );
	_datatrain->Branch( "mc_muon_num_gen_minus"                     , &info_mc.num_muon_gen.num_minus                        , "mc_muon_num_gen_minus"                       );
	_datatrain->Branch( "mc_muon_num_recovery"                      , &info_mc.num_muon_recovery.num                         , "mc_muon_num_recovery"                        );
	_datatrain->Branch( "mc_muon_num_recovery_plus"                 , &info_mc.num_muon_recovery.num_plus                    , "mc_muon_num_recovery_plus"                   );
	_datatrain->Branch( "mc_muon_num_recovery_minus"                , &info_mc.num_muon_recovery.num_minus                   , "mc_muon_num_recovery_minus"                  );
	_datatrain->Branch( "mc_muon_num"                               , &info_mc.num_muon.num                                  , "mc_muon_num"                                 );
	_datatrain->Branch( "mc_muon_num_plus"                          , &info_mc.num_muon.num_plus                             , "mc_muon_num_plus"                            );
	_datatrain->Branch( "mc_muon_num_minus"                         , &info_mc.num_muon.num_minus                            , "mc_muon_num_minus"                           );



	_datatrain->Branch( "mc_muon_gen_kcut_zmass"                    , &info_mc.data_muon_gen.kcut_zmass                      , "mc_muon_kcut_zmass"                            );
	_datatrain->Branch( "mc_muon_gen_kcut_zpt"                      , &info_mc.data_muon_gen.kcut_zpt                        , "mc_muon_kcut_zpt"                              );
	_datatrain->Branch( "mc_muon_gen_kcut_invis_costheta"           , &info_mc.data_muon_gen.kcut_invis_costheta             , "mc_muon_kcut_invis_costheta"                   );
	_datatrain->Branch( "mc_muon_gen_kcut_mva"                      , &info_mc.data_muon_gen.kcut_mva                        , "mc_muon_kcut_mva"                              );
	_datatrain->Branch( "mc_muon_gen_kcut_recoil_mass"              , &info_mc.data_muon_gen.kcut_recoil_mass                , "mc_muon_kcut_recoil_mass"                      );
	_datatrain->Branch( "mc_muon_gen_kcut_vis_e"                    , &info_mc.data_muon_gen.kcut_vis_e                      , "mc_muon_kcut_vis_e"                            );
	_datatrain->Branch( "mc_muon_gen_lep_pair_costheta"             , &info_mc.data_muon_gen.lep_pair_costheta               , "mc_muon_lep_pair_costheta"                     );
	_datatrain->Branch( "mc_muon_gen_lep_pair_costheta_pair"        , &info_mc.data_muon_gen.lep_pair_costheta_pair          , "mc_muon_lep_pair_costheta_pair"                );
	_datatrain->Branch( "mc_muon_gen_lep_pair_costheta_track1"      , &info_mc.data_muon_gen.lep_pair_costheta_track1        , "mc_muon_lep_pair_costheta_track1"              );        
	_datatrain->Branch( "mc_muon_gen_lep_pair_costheta_track2"      , &info_mc.data_muon_gen.lep_pair_costheta_track2        , "mc_muon_lep_pair_costheta_track2"              );        

	_datatrain->Branch( "mc_muon_recovery_kcut_zmass"               , &info_mc.data_muon_recovery.kcut_zmass                 , "mc_muon_kcut_zmass"                            );
	_datatrain->Branch( "mc_muon_recovery_kcut_zpt"                 , &info_mc.data_muon_recovery.kcut_zpt                   , "mc_muon_kcut_zpt"                              );
	_datatrain->Branch( "mc_muon_recovery_kcut_invis_costheta"      , &info_mc.data_muon_recovery.kcut_invis_costheta        , "mc_muon_kcut_invis_costheta"                   );
	_datatrain->Branch( "mc_muon_recovery_kcut_mva"                 , &info_mc.data_muon_recovery.kcut_mva                   , "mc_muon_kcut_mva"                              );
	_datatrain->Branch( "mc_muon_recovery_kcut_recoil_mass"         , &info_mc.data_muon_recovery.kcut_recoil_mass           , "mc_muon_kcut_recoil_mass"                      );
	_datatrain->Branch( "mc_muon_recovery_kcut_vis_e"               , &info_mc.data_muon_recovery.kcut_vis_e                 , "mc_muon_kcut_vis_e"                            );
	_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta"        , &info_mc.data_muon_recovery.lep_pair_costheta          , "mc_muon_lep_pair_costheta"                     );
	_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta_pair"   , &info_mc.data_muon_recovery.lep_pair_costheta_pair     , "mc_muon_lep_pair_costheta_pair"                );
	_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta_track1" , &info_mc.data_muon_recovery.lep_pair_costheta_track1   , "mc_muon_lep_pair_costheta_track1"              );        
	_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta_track2" , &info_mc.data_muon_recovery.lep_pair_costheta_track2   , "mc_muon_lep_pair_costheta_track2"              );        


	_datatrain->Branch( "mc_muon_kcut_zmass"                        , &info_mc.data_muon.kcut_zmass                          , "mc_muon_kcut_zmass"                            );
	_datatrain->Branch( "mc_muon_kcut_zpt"                          , &info_mc.data_muon.kcut_zpt                            , "mc_muon_kcut_zpt"                              );
	_datatrain->Branch( "mc_muon_kcut_invis_costheta"               , &info_mc.data_muon.kcut_invis_costheta                 , "mc_muon_kcut_invis_costheta"                   );
	_datatrain->Branch( "mc_muon_kcut_mva"                          , &info_mc.data_muon.kcut_mva                            , "mc_muon_kcut_mva"                              );
	_datatrain->Branch( "mc_muon_kcut_recoil_mass"                  , &info_mc.data_muon.kcut_recoil_mass                    , "mc_muon_kcut_recoil_mass"                      );
	_datatrain->Branch( "mc_muon_kcut_vis_e"                        , &info_mc.data_muon.kcut_vis_e                          , "mc_muon_kcut_vis_e"                            );
	_datatrain->Branch( "mc_muon_lep_pair_costheta"                 , &info_mc.data_muon.lep_pair_costheta                   , "mc_muon_lep_pair_costheta"                     );
	_datatrain->Branch( "mc_muon_lep_pair_costheta_pair"            , &info_mc.data_muon.lep_pair_costheta_pair              , "mc_muon_lep_pair_costheta_pair"                );
	_datatrain->Branch( "mc_muon_lep_pair_costheta_track1"          , &info_mc.data_muon.lep_pair_costheta_track1            , "mc_muon_lep_pair_costheta_track1"              );        
	_datatrain->Branch( "mc_muon_lep_pair_costheta_track2"          , &info_mc.data_muon.lep_pair_costheta_track2            , "mc_muon_lep_pair_costheta_track2"              );        






	//partraine number
	_datatrain->Branch( "rc_lep_num_gen"                            , &info_rc.num_lep_gen.num                               , "rc_lep_num_gen"                              );
	_datatrain->Branch( "rc_muon_num_gen"                           , &info_rc.num_muon_gen.num                              , "rc_muon_num_gen"                             );
	_datatrain->Branch( "rc_muon_num_gen_plus"                      , &info_rc.num_muon_gen.num_plus                         , "rc_muon_num_gen_plus"                        );
	_datatrain->Branch( "rc_muon_num_gen_minus"                     , &info_rc.num_muon_gen.num_minus                        , "rc_muon_num_gen_minus"                       );
	_datatrain->Branch( "rc_lep_num_recovery"                       , &info_rc.num_lep_recovery.num                          , "rc_lep_num_recovery"                         );
	_datatrain->Branch( "rc_muon_num_recovery"                      , &info_rc.num_muon_recovery.num                         , "rc_muon_num_recovery"                        );
	_datatrain->Branch( "rc_muon_num_recovery_plus"                 , &info_rc.num_muon_recovery.num_plus                    , "rc_muon_num_recovery_plus"                   );
	_datatrain->Branch( "rc_muon_num_recovery_minus"                , &info_rc.num_muon_recovery.num_minus                   , "rc_muon_num_recovery_minus"                  );
	_datatrain->Branch( "rc_lep_num"                                , &info_rc.num_lep.num                                   , "rc_lep_num"                                  );
	_datatrain->Branch( "rc_muon_num"                               , &info_rc.num_muon.num                                  , "rc_muon_num"                                 );
	_datatrain->Branch( "rc_muon_num_plus"                          , &info_rc.num_muon.num_plus                             , "rc_muon_num_plus"                            );
	_datatrain->Branch( "rc_muon_num_minus"                         , &info_rc.num_muon.num_minus                            , "rc_muon_num_minus"                           );




	_datatrain->Branch( "rc_muon_gen_kcut_zmass"                    , &info_rc.data_muon_gen.kcut_zmass                      , "rc_muon_gen_kcut_zmass"                        );
	_datatrain->Branch( "rc_muon_gen_kcut_zpt"                      , &info_rc.data_muon_gen.kcut_zpt                        , "rc_muon_gen_kcut_zpt"                          );
	_datatrain->Branch( "rc_muon_gen_kcut_invis_costheta"           , &info_rc.data_muon_gen.kcut_invis_costheta             , "rc_muon_gen_kcut_invis_costheta"               );
	_datatrain->Branch( "rc_muon_gen_kcut_mva"                      , &info_rc.data_muon_gen.kcut_mva                        , "rc_muon_gen_kcut_mva"                          );
	_datatrain->Branch( "rc_muon_gen_kcut_recoil_mass"              , &info_rc.data_muon_gen.kcut_recoil_mass                , "rc_muon_gen_kcut_recoil_mass"                  );
	_datatrain->Branch( "rc_muon_gen_kcut_vis_e"                    , &info_rc.data_muon_gen.kcut_vis_e                      , "rc_muon_gen_kcut_vis_e"                        );
	_datatrain->Branch( "rc_muon_gen_lep_pair_costheta"             , &info_rc.data_muon_gen.lep_pair_costheta               , "rc_muon_gen_lep_pair_costheta"                 );
	_datatrain->Branch( "rc_muon_gen_lep_pair_costheta_pair"        , &info_rc.data_muon_gen.lep_pair_costheta_pair          , "rc_muon_gen_lep_pair_costheta_pair"            );
	_datatrain->Branch( "rc_muon_gen_lep_pair_costheta_track1"      , &info_rc.data_muon_gen.lep_pair_costheta_track1        , "rc_muon_gen_lep_pair_costheta_track1"          );        
	_datatrain->Branch( "rc_muon_gen_lep_pair_costheta_track2"      , &info_rc.data_muon_gen.lep_pair_costheta_track2        , "rc_muon_gen_lep_pair_costheta_track2"          );        

	_datatrain->Branch( "rc_muon_recovery_kcut_zmass"               , &info_rc.data_muon_recovery.kcut_zmass                 , "rc_muon_recovery_kcut_zmass"                   );
	_datatrain->Branch( "rc_muon_recovery_kcut_zpt"                 , &info_rc.data_muon_recovery.kcut_zpt                   , "rc_muon_recovery_kcut_zpt"                     );
	_datatrain->Branch( "rc_muon_recovery_kcut_invis_costheta"      , &info_rc.data_muon_recovery.kcut_invis_costheta        , "rc_muon_recovery_kcut_invis_costheta"          );
	_datatrain->Branch( "rc_muon_recovery_kcut_mva"                 , &info_rc.data_muon_recovery.kcut_mva                   , "rc_muon_recovery_kcut_mva"                     );
	_datatrain->Branch( "rc_muon_recovery_kcut_recoil_mass"         , &info_rc.data_muon_recovery.kcut_recoil_mass           , "rc_muon_recovery_kcut_recoil_mass"             );
	_datatrain->Branch( "rc_muon_recovery_kcut_vis_e"               , &info_rc.data_muon_recovery.kcut_vis_e                 , "rc_muon_recovery_kcut_vis_e"                   );
	_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta"        , &info_rc.data_muon_recovery.lep_pair_costheta          , "rc_muon_recovery_lep_pair_costheta"            );
	_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta_pair"   , &info_rc.data_muon_recovery.lep_pair_costheta_pair     , "rc_muon_recovery_lep_pair_costheta_pair"       );
	_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta_track1" , &info_rc.data_muon_recovery.lep_pair_costheta_track1   , "rc_muon_recovery_lep_pair_costheta_track1"     );        
	_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta_track2" , &info_rc.data_muon_recovery.lep_pair_costheta_track2   , "rc_muon_recovery_lep_pair_costheta_track2"     );        

	_datatrain->Branch( "rc_muon_kcut_zmass"                        , &info_rc.data_muon.kcut_zmass                          , "rc_muon_kcut_zmass"                            );
	_datatrain->Branch( "rc_muon_kcut_zpt"                          , &info_rc.data_muon.kcut_zpt                            , "rc_muon_kcut_zpt"                              );
	_datatrain->Branch( "rc_muon_kcut_invis_costheta"               , &info_rc.data_muon.kcut_invis_costheta                 , "rc_muon_kcut_invis_costheta"                   );
	_datatrain->Branch( "rc_muon_kcut_mva"                          , &info_rc.data_muon.kcut_mva                            , "rc_muon_kcut_mva"                              );
	_datatrain->Branch( "rc_muon_kcut_recoil_mass"                  , &info_rc.data_muon.kcut_recoil_mass                    , "rc_muon_kcut_recoil_mass"                      );
	_datatrain->Branch( "rc_muon_kcut_vis_e"                        , &info_rc.data_muon.kcut_vis_e                          , "rc_muon_kcut_vis_e"                            );
	_datatrain->Branch( "rc_muon_lep_pair_costheta"                 , &info_rc.data_muon.lep_pair_costheta                   , "rc_muon_lep_pair_costheta"                     );
	_datatrain->Branch( "rc_muon_lep_pair_costheta_pair"            , &info_rc.data_muon.lep_pair_costheta_pair              , "rc_muon_lep_pair_costheta_pair"                );
	_datatrain->Branch( "rc_muon_lep_pair_costheta_track1"          , &info_rc.data_muon.lep_pair_costheta_track1            , "rc_muon_lep_pair_costheta_track1"              );        
	_datatrain->Branch( "rc_muon_lep_pair_costheta_track2"          , &info_rc.data_muon.lep_pair_costheta_track2            , "rc_muon_lep_pair_costheta_track2"              );        







	//partraine number
	_datatrain->Branch( "po_lep_num_gen"                            , &info_po.num_lep_gen.num                               , "po_lep_num_gen"                             );
	_datatrain->Branch( "po_muon_num_gen"                           , &info_po.num_muon_gen.num                              , "po_muon_num_gen"                             );
	_datatrain->Branch( "po_muon_num_gen_plus"                      , &info_po.num_muon_gen.num_plus                         , "po_muon_num_gen_plus"                        );
	_datatrain->Branch( "po_muon_num_gen_minus"                     , &info_po.num_muon_gen.num_minus                        , "po_muon_num_gen_minus"                       );
	_datatrain->Branch( "po_lep_num_recovery"                       , &info_po.num_lep_recovery.num                          , "po_lep_num_recovery"                        );
	_datatrain->Branch( "po_muon_num_recovery"                      , &info_po.num_muon_recovery.num                         , "po_muon_num_recovery"                        );
	_datatrain->Branch( "po_muon_num_recovery_plus"                 , &info_po.num_muon_recovery.num_plus                    , "po_muon_num_recovery_plus"                   );
	_datatrain->Branch( "po_muon_num_recovery_minus"                , &info_po.num_muon_recovery.num_minus                   , "po_muon_num_recovery_minus"                  );
	_datatrain->Branch( "po_lep_num"                                , &info_po.num_lep.num                                   , "po_lep_num"                                 );
	_datatrain->Branch( "po_muon_num"                               , &info_po.num_muon.num                                  , "po_muon_num"                                 );
	_datatrain->Branch( "po_muon_num_plus"                          , &info_po.num_muon.num_plus                             , "po_muon_num_plus"                            );
	_datatrain->Branch( "po_muon_num_minus"                         , &info_po.num_muon.num_minus                            , "po_muon_num_minus"                           );




	_datatrain->Branch( "po_muon_gen_kcut_zmass"                    , &info_po.data_muon_gen.kcut_zmass                      , "po_muon_gen_kcut_zmass"                        );
	_datatrain->Branch( "po_muon_gen_kcut_zpt"                      , &info_po.data_muon_gen.kcut_zpt                        , "po_muon_gen_kcut_zpt"                          );
	_datatrain->Branch( "po_muon_gen_kcut_invis_costheta"           , &info_po.data_muon_gen.kcut_invis_costheta             , "po_muon_gen_kcut_invis_costheta"               );
	_datatrain->Branch( "po_muon_gen_kcut_mva"                      , &info_po.data_muon_gen.kcut_mva                        , "po_muon_gen_kcut_mva"                          );
	_datatrain->Branch( "po_muon_gen_kcut_recoil_mass"              , &info_po.data_muon_gen.kcut_recoil_mass                , "po_muon_gen_kcut_recoil_mass"                  );
	_datatrain->Branch( "po_muon_gen_kcut_vis_e"                    , &info_po.data_muon_gen.kcut_vis_e                      , "po_muon_gen_kcut_vis_e"                        );
	_datatrain->Branch( "po_muon_gen_lep_pair_costheta"             , &info_po.data_muon_gen.lep_pair_costheta               , "po_muon_gen_lep_pair_costheta"                 );
	_datatrain->Branch( "po_muon_gen_lep_pair_costheta_pair"        , &info_po.data_muon_gen.lep_pair_costheta_pair          , "po_muon_gen_lep_pair_costheta_pair"            );
	_datatrain->Branch( "po_muon_gen_lep_pair_costheta_track1"      , &info_po.data_muon_gen.lep_pair_costheta_track1        , "po_muon_gen_lep_pair_costheta_track1"          );        
	_datatrain->Branch( "po_muon_gen_lep_pair_costheta_track2"      , &info_po.data_muon_gen.lep_pair_costheta_track2        , "po_muon_gen_lep_pair_costheta_track2"          );        

	_datatrain->Branch( "po_muon_recovery_kcut_zmass"               , &info_po.data_muon_recovery.kcut_zmass                 , "po_muon_recovery_kcut_zmass"                   );
	_datatrain->Branch( "po_muon_recovery_kcut_zpt"                 , &info_po.data_muon_recovery.kcut_zpt                   , "po_muon_recovery_kcut_zpt"                     );
	_datatrain->Branch( "po_muon_recovery_kcut_invis_costheta"      , &info_po.data_muon_recovery.kcut_invis_costheta        , "po_muon_recovery_kcut_invis_costheta"          );
	_datatrain->Branch( "po_muon_recovery_kcut_mva"                 , &info_po.data_muon_recovery.kcut_mva                   , "po_muon_recovery_kcut_mva"                     );
	_datatrain->Branch( "po_muon_recovery_kcut_recoil_mass"         , &info_po.data_muon_recovery.kcut_recoil_mass           , "po_muon_recovery_kcut_recoil_mass"             );
	_datatrain->Branch( "po_muon_recovery_kcut_vis_e"               , &info_po.data_muon_recovery.kcut_vis_e                 , "po_muon_recovery_kcut_vis_e"                   );
	_datatrain->Branch( "po_muon_recovery_lep_pair_costheta"        , &info_po.data_muon_recovery.lep_pair_costheta          , "po_muon_recovery_lep_pair_costheta"            );
	_datatrain->Branch( "po_muon_recovery_lep_pair_costheta_pair"   , &info_po.data_muon_recovery.lep_pair_costheta_pair     , "po_muon_recovery_lep_pair_costheta_pair"       );
	_datatrain->Branch( "po_muon_recovery_lep_pair_costheta_track1" , &info_po.data_muon_recovery.lep_pair_costheta_track1   , "po_muon_recovery_lep_pair_costheta_track1"     );        
	_datatrain->Branch( "po_muon_recovery_lep_pair_costheta_track2" , &info_po.data_muon_recovery.lep_pair_costheta_track2   , "po_muon_recovery_lep_pair_costheta_track2"     );        

	_datatrain->Branch( "po_muon_kcut_zmass"                        , &info_po.data_muon.kcut_zmass                          , "po_muon_kcut_zmass"                            );
	_datatrain->Branch( "po_muon_kcut_zpt"                          , &info_po.data_muon.kcut_zpt                            , "po_muon_kcut_zpt"                              );
	_datatrain->Branch( "po_muon_kcut_invis_costheta"               , &info_po.data_muon.kcut_invis_costheta                 , "po_muon_kcut_invis_costheta"                   );
	_datatrain->Branch( "po_muon_kcut_mva"                          , &info_po.data_muon.kcut_mva                            , "po_muon_kcut_mva"                              );
	_datatrain->Branch( "po_muon_kcut_recoil_mass"                  , &info_po.data_muon.kcut_recoil_mass                    , "po_muon_kcut_recoil_mass"                      );
	_datatrain->Branch( "po_muon_kcut_vis_e"                        , &info_po.data_muon.kcut_vis_e                          , "po_muon_kcut_vis_e"                            );
	_datatrain->Branch( "po_muon_lep_pair_costheta"                 , &info_po.data_muon.lep_pair_costheta                   , "po_muon_lep_pair_costheta"                     );
	_datatrain->Branch( "po_muon_lep_pair_costheta_pair"            , &info_po.data_muon.lep_pair_costheta_pair              , "po_muon_lep_pair_costheta_pair"                );
	_datatrain->Branch( "po_muon_lep_pair_costheta_track1"          , &info_po.data_muon.lep_pair_costheta_track1            , "po_muon_lep_pair_costheta_track1"              );        
	_datatrain->Branch( "po_muon_lep_pair_costheta_track2"          , &info_po.data_muon.lep_pair_costheta_track2            , "po_muon_lep_pair_costheta_track2"              );        
	_datatrain->Branch( "po_muon_track1_pt"                         , &info_po.data_muon.track1_pt                           , "po_muon_track1_pt"                             );        
	_datatrain->Branch( "po_muon_track1_e"                          , &info_po.data_muon.track1_e                            , "po_muon_track1_e"                              );        
	_datatrain->Branch( "po_muon_track2_pt"                         , &info_po.data_muon.track2_pt                           , "po_muon_track2_pt"                             );        
	_datatrain->Branch( "po_muon_track2_e"                          , &info_po.data_muon.track2_e                            , "po_muon_track2_e"                              );        









	return;
}

