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

////registerInputCollection( LCIO::LCRELATION,
////		"InputMyRecoMCTruthLinkName",
////		"Relation between MC and PFO particles",
////		_mcpfoRelation,
////		std::string("RecoMCTruthLinkNew"));

////registerInputCollection( LCIO::LCRELATION,
////		"InputMyMCTruthRecoLinkName",
////		"Relation between PFO particles and MC",
////		_pfomcRelation,
////		std::string("MCTruthRecoLinkNew"));

    registerInputCollection( LCIO::LCRELATION,
    		"InputMyRecoMCTruthLinkName",
    		"Relation between MC and PFO particles",
    		_mcpfoRelation,
    		std::string("RecoMCTruthLink"));

//   registerInputCollection( LCIO::LCRELATION,
//   		"InputMyMCTruthRecoLinkName",
//   		"Relation between PFO particles and MC",
//   		_pfomcRelation,
//   		std::string("MCTruthRecoLink"));
//
////registerInputCollection( LCIO::MCPARTICLE,
////		"MCParticleCollection", 
////		"Name of the MC particle collection",
////		_colMC,
////		std::string("MCParticle") );

    registerInputCollection( LCIO::MCPARTICLE,
    		"MCParticleCollection", 
    		"Name of the MC particle collection",
    		_colMC,
    		std::string("MCParticlesSkimmed") );

	registerProcessorParameter( "hmass",
			"For counting energetic charged particles in the collection after isolated lepton tagging",
			_hmass,
			float(125) );

	registerProcessorParameter( "cmenergy",
			"center of mass energy",
			_cmenergy,
			float(250.006126) );

	registerProcessorParameter( "ProcessListFile",
			"Name of process list file",
			_processList,
			std::string("procid500.txt") ) ;

	registerProcessorParameter( "RootFileName",
			"Name of Root file (default: output.root)",
			_rootfilename, 
			std::string("../result/output.root") );


	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			"InputPandoraIsoPhotonCollection",
			"Name of the Pandora isolated photon collection",
			_colPandoraIsoPhoton,
			std::string("PandoraIsoPhoton") );

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			"InputPandoraIsoForwardPhotonCollection",
			"Name of the pandora isolated photon collection in the forward region",
			_colPandoraIsoForwardPhoton,
			std::string("PandoraIsoForwardPhoton") );

	registerInputCollection( LCIO::RECONSTRUCTEDPARTICLE,
			"InputPandoraPFOsWithoutIsoPhotonCollection", 
			"Name of the PFOs after Pandora isolated Photon tagging collection",
			_colPandoraPFOsWithoutIsoPhoton,
			std::string("PandoraPFOsWithoutIsoPhoton") );

	registerInputCollection( LCIO::MCPARTICLE,
			"InputMCIsoPhotonCollection",
			"Name of the MC isolated photon collection",
			_colMCsPhoton,
			std::string("MCsPhoton") );

	registerInputCollection( LCIO::MCPARTICLE,
			"InputMCIsoForwardPhotonCollection",
			"Name of the MC isolated photon collection in the forward region",
			_colMCsForwardPhoton,
			std::string("MCsForwardPhoton") );

	registerInputCollection( LCIO::MCPARTICLE,
			"InputMCPFOsWithoutIsoPhotonCollection", 
			"Name of the PFOs after MC isolated Photon tagging collection",
			_colMCsWithoutIsoPhoton,
			std::string("MCsWithoutIsoPhoton") );
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

	counter.pass_MC  = 0;
	counter.pass_RC  = 0;
	counter.pass_PO  = 0;
	counter.pass_all = 0;

	counter.pass_mc_num_false = 0;
	counter.pass_rc_num_false = 0;
	counter.pass_po_num_false = 0;


	counter.pass_mc_energy_false= 0;
	counter.pass_rc_energy_false= 0;
	counter.pass_po_energy_false= 0;


    counter.pass_mc_npfo  = 0;
    counter.pass_mc_nisl  = 0;
    counter.pass_pfo_nmc  = 0;
    counter.pass_pfo_nisl = 0;
    counter.pass_isl_nmc  = 0;
    counter.pass_isl_npfo = 0;


	debug.Read_Debug();
	pass_something = 0;

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
	if( _nEvt % 50 == 0 ) std::cout << "processing event "<< _nEvt << std::endl;

	// this gets called for every event 
	// usually the working horse ...

	// Clear memory
    memset( &info_po,    0, sizeof(info_po) );
    memset( &info_mc,    0, sizeof(info_mc) );
    memset( &info_rc,    0, sizeof(info_rc) );
    memset( &info_track, 0, sizeof(info_track) );
    memset( &counter,    0, sizeof(counter) );

    info_po   .init();
    info_mc   .init();
    info_rc   .init();
    info_track.init();
    counter.init();
    _photon_chain.Init();
    _muon_chain  .Init();
    _FS_muon_chain.Init();
    _lepton_chain.Init();
    _hadron_chain.Init();

    // fill histogram from LCIO data :
    LCCollection* AllPFOs = evt->getCollection( _colAllPFOs ) ;
    LCCollection* AllMC = evt->getCollection( _colMC ) ;
    _navpfo = new LCRelationNavigator( evt->getCollection( _mcpfoRelation ) );
    //_navmc  = new LCRelationNavigator( evt->getCollection( _pfomcRelation ) );

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


    LCCollection* PandoraIsoPhoton = 0;
    try{
    	PandoraIsoPhoton= evt->getCollection( _colPandoraIsoPhoton);
    }
    catch(...){
    	cout << "no Pandora isolated photon in this event" << endl;
    }
    LCCollection* PandoraIsoForwardPhoton= 0;
    try{
    	PandoraIsoForwardPhoton= evt->getCollection( _colPandoraIsoForwardPhoton);
    }
    catch(...){
    	cout << "no Pandora isolated photon in the forward region in this event" << endl;
    }
    LCCollection* PandoraPFOsWithoutIsoPhoton = 0;
    try{
    	PandoraPFOsWithoutIsoPhoton= evt->getCollection( _colPandoraPFOsWithoutIsoPhoton);
    }
    catch(...){
    	cout << "no pfos after Pandora isolated photon tagging in this event" << endl;
    }


    LCCollection* MCsPhoton = 0;
    try{
    	MCsPhoton= evt->getCollection( _colMCsPhoton);
    }
    catch(...){
    	cout << "no MC photon in this event" << endl;
    }
    LCCollection* MCsForwardPhoton= 0;
    try{
    	MCsForwardPhoton= evt->getCollection( _colMCsForwardPhoton);
    }
    catch(...){
    	cout << "no MC photon in the forward region in this event" << endl;
    }
    LCCollection* MCsWithoutIsoPhoton = 0;
    try{
    	MCsWithoutIsoPhoton= evt->getCollection( _colMCsWithoutIsoPhoton);
    }
    catch(...){
    	cout << "no MCs after isolated photon tagging in this event" << endl;
    }

    debug.Message(1,1,"in processEvent:  begin analyseMCParticle ");

    bool JMC,JRC,JPFO,JTrack;
      if(debug.debug[900]==true){
      	JMC=analyseMCParticle(AllMC,MCsWithoutIsoPhoton, MCsPhoton, MCsForwardPhoton, info_mc);
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
    	//JPFO=analysePhysicalObject(AllPFOs,PFOsWithoutIsoleps,Isoleps,info_po);
    	JPFO=analysePhysicalObject(AllPFOs,PFOsWithoutIsoleps,Isoleps,PandoraPFOsWithoutIsoPhoton, PandoraIsoPhoton, PandoraIsoForwardPhoton, info_po);
    	if(JPFO){
    		counter.pass_PO++;
    	}
    	else{
    		debug.Message(1,1,"in processEvent: not pass analysePhysicalObject");
    	}
    }

//  if(debug.debug[903]==true){
//  	JTrack=analyseTrack(_nEvt,AllMC,AllPFOs,_navpfo,_navmc, PFOsWithoutIsoleps, Isoleps, info_mc, info_rc, info_po);
//  	if(JTrack){
//  		counter.pass_Track++;
//  	}
//  	else{
//  		debug.Message(1,1,"in processEvent: not pass analyseTrack");
//  	}
//  }


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
	std::cout << _rootfilename << std::endl;
	_otfile = new TFile( _rootfilename.c_str() , "RECREATE" );

	//Define root tree
	_datatrain= new TTree( "datatrain" , "events" );
	//event number
	_datatrain->Branch( "evt"     , &counter.evt, "evt" );
	_datatrain->Branch( "pass_MC" , &counter.pass_MC , "pass_MC" );
	_datatrain->Branch( "pass_RC" , &counter.pass_RC , "pass_RC" );
	_datatrain->Branch( "pass_PO" , &counter.pass_PO , "pass_PO" );
	_datatrain->Branch( "pass_all", &counter.pass_all, "pass_all" );

	_datatrain->Branch( "pass_mc_num_false" , &counter.pass_mc_num_false, "pass_mc_num_false"  );
	_datatrain->Branch( "pass_rc_num_false" , &counter.pass_rc_num_false, "pass_rc_num_false"  );
	_datatrain->Branch( "pass_po_num_false" , &counter.pass_po_num_false, "pass_po_num_false"  );

	_datatrain->Branch( "pass_mc_energy_false" , &counter.pass_mc_energy_false, "pass_mc_energy_false"  );
	_datatrain->Branch( "pass_rc_energy_false" , &counter.pass_rc_energy_false, "pass_rc_energy_false"  );
	_datatrain->Branch( "pass_po_energy_false" , &counter.pass_po_energy_false, "pass_po_energy_false"  );



	_datatrain->Branch( "pass_mc_npfo" , &counter.pass_mc_npfo , "pass_mc_npfo"  );
	_datatrain->Branch( "pass_mc_nisl" , &counter.pass_mc_nisl , "pass_mc_nisl"  );
	_datatrain->Branch( "pass_pfo_nmc" , &counter.pass_pfo_nmc , "pass_pfo_nmc"  );
	_datatrain->Branch( "pass_pfo_nisl", &counter.pass_pfo_nisl, "pass_pfo_nisl" );
	_datatrain->Branch( "pass_isl_nmc" , &counter.pass_isl_nmc , "pass_isl_nmc"  );
	_datatrain->Branch( "pass_isl_npfo", &counter.pass_isl_npfo, "pass_isl_npfo" );


	//partraine number
////_datatrain->Branch( "mc_muon_num_gen"                           , &info_mc.num_muon_gen.num                              , "mc_muon_num_gen"                               );
////_datatrain->Branch( "mc_muon_num_gen_plus"                      , &info_mc.num_muon_gen.num_plus                         , "mc_muon_num_gen_plus"                          );
////_datatrain->Branch( "mc_muon_num_gen_minus"                     , &info_mc.num_muon_gen.num_minus                        , "mc_muon_num_gen_minus"                         );
////_datatrain->Branch( "mc_muon_num_recovery"                      , &info_mc.num_muon_recovery.num                         , "mc_muon_num_recovery"                          );
////_datatrain->Branch( "mc_muon_num_recovery_plus"                 , &info_mc.num_muon_recovery.num_plus                    , "mc_muon_num_recovery_plus"                     );
////_datatrain->Branch( "mc_muon_num_recovery_minus"                , &info_mc.num_muon_recovery.num_minus                   , "mc_muon_num_recovery_minus"                    );
////_datatrain->Branch( "mc_muon_num"                               , &info_mc.num_muon.num                                  , "mc_muon_num"                                   );
////_datatrain->Branch( "mc_muon_num_plus"                          , &info_mc.num_muon.num_plus                             , "mc_muon_num_plus"                              );
////_datatrain->Branch( "mc_muon_num_minus"                         , &info_mc.num_muon.num_minus                            , "mc_muon_num_minus"                             );




	info_mc.data_muon_hs.Fill_Data(_datatrain,"mc_muon_gen_");
	info_mc.num_muon_hs .Fill_Data(_datatrain,"mc_muon_gen_");
////_datatrain->Branch( "mc_muon_gen_higgs_channle"                 , &info_mc.data_muon_gen.higgs_channel                   , "mc_muon_gen_higgs_channel"                     );
////_datatrain->Branch( "mc_muon_gen_kcut_zmass"                    , &info_mc.data_muon_gen.kcut_zmass                      , "mc_muon_gen_kcut_zmass"                        );
////_datatrain->Branch( "mc_muon_gen_kcut_zpt"                      , &info_mc.data_muon_gen.kcut_zpt                        , "mc_muon_gen_kcut_zpt"                          );
////_datatrain->Branch( "mc_muon_gen_kcut_invis_costheta"           , &info_mc.data_muon_gen.kcut_invis_costheta             , "mc_muon_gen_kcut_invis_costheta"               );
////_datatrain->Branch( "mc_muon_gen_kcut_mva"                      , &info_mc.data_muon_gen.kcut_mva                        , "mc_muon_gen_kcut_mva"                          );
////_datatrain->Branch( "mc_muon_gen_kcut_recoil_mass"              , &info_mc.data_muon_gen.kcut_recoil_mass                , "mc_muon_gen_kcut_recoil_mass"                  );
////_datatrain->Branch( "mc_muon_gen_kcut_vis_e"                    , &info_mc.data_muon_gen.kcut_vis_e                      , "mc_muon_gen_kcut_vis_e"                        );
////_datatrain->Branch( "mc_muon_gen_lep_pair_costheta"             , &info_mc.data_muon_gen.lep_pair_costheta               , "mc_muon_gen_lep_pair_costheta"                 );
////_datatrain->Branch( "mc_muon_gen_lep_pair_costheta_pair"        , &info_mc.data_muon_gen.lep_pair_costheta_pair          , "mc_muon_gen_lep_pair_costheta_pair"            );
////_datatrain->Branch( "mc_muon_gen_lep_pair_costheta_track1"      , &info_mc.data_muon_gen.lep_pair_costheta_track1        , "mc_muon_gen_lep_pair_costheta_track1"          );        
////_datatrain->Branch( "mc_muon_gen_lep_pair_costheta_track2"      , &info_mc.data_muon_gen.lep_pair_costheta_track2        , "mc_muon_gen_lep_pair_costheta_track2"          );        
////_datatrain->Branch( "mc_muon_gen_lep_pair_azimuth"              , &info_mc.data_muon_gen.lep_pair_azimuth                , "mc_muon_gen_lep_pair_azimuth"                  );
////_datatrain->Branch( "mc_muon_gen_lep_pair_azimuth_pair"         , &info_mc.data_muon_gen.lep_pair_azimuth_pair           , "mc_muon_gen_lep_pair_azimuth_pair"             );
////_datatrain->Branch( "mc_muon_gen_lep_pair_azimuth_track1"       , &info_mc.data_muon_gen.lep_pair_azimuth_track1         , "mc_muon_gen_lep_pair_azimuth_track1"           );        
////_datatrain->Branch( "mc_muon_gen_lep_pair_azimuth_track2"       , &info_mc.data_muon_gen.lep_pair_azimuth_track2         , "mc_muon_gen_lep_pair_azimuth_track2"           );        

////_datatrain->Branch( "mc_muon_gen_track1_pt"                     , &info_mc.data_muon_gen.track1_pt                       , "mc_muon_gen_track1_pt"                         );        
////_datatrain->Branch( "mc_muon_gen_track1_e"                      , &info_mc.data_muon_gen.track1_e                        , "mc_muon_gen_track1_e"                          );        
////_datatrain->Branch( "mc_muon_gen_track2_pt"                     , &info_mc.data_muon_gen.track2_pt                       , "mc_muon_gen_track2_pt"                         );        
////_datatrain->Branch( "mc_muon_gen_track2_e"                      , &info_mc.data_muon_gen.track2_e                        , "mc_muon_gen_track2_e"                          );        
////_datatrain->Branch( "mc_muon_gen_total_photon_num"              , &info_mc.data_muon_gen.total_photon_number             , "mc_muon_gen_total_photon_num"                  );
////_datatrain->Branch( "mc_muon_gen_photon_num"                    , &info_mc.data_muon_gen.photon_number                   , "mc_muon_gen_photon_num"                        );
////_datatrain->Branch( "mc_muon_gen_photon_energy1"                , &info_mc.data_muon_gen.photon_energy1                  , "mc_muon_gen_photon_energy1"                    );
////_datatrain->Branch( "mc_muon_gen_photon_energy2"                , &info_mc.data_muon_gen.photon_energy2                  , "mc_muon_gen_photon_energy2"                    );
////_datatrain->Branch( "mc_muon_gen_photon_costheta1"              , &info_mc.data_muon_gen.photon_costheta1                , "mc_muon_gen_photon_costheta1"                  );
////_datatrain->Branch( "mc_muon_gen_photon_costheta2"              , &info_mc.data_muon_gen.photon_costheta2                , "mc_muon_gen_photon_costheta2"                  );
////_datatrain->Branch( "mc_muon_gen_forward_photon_num"            , &info_mc.data_muon_gen.forward_photon_number           , "mc_muon_gen_forward_photon_num"                );
////_datatrain->Branch( "mc_muon_gen_forward_photon_energy1"        , &info_mc.data_muon_gen.forward_photon_energy1          , "mc_muon_gen_forward_photon_energy1"            );
////_datatrain->Branch( "mc_muon_gen_forward_photon_energy2"        , &info_mc.data_muon_gen.forward_photon_energy2          , "mc_muon_gen_forward_photon_energy2"            );
////_datatrain->Branch( "mc_muon_gen_forward_photon_costheta1"      , &info_mc.data_muon_gen.forward_photon_costheta1        , "mc_muon_gen_forward_photon_costheta1"          );
////_datatrain->Branch( "mc_muon_gen_forward_photon_costheta2"      , &info_mc.data_muon_gen.forward_photon_costheta2        , "mc_muon_gen_forward_photon_costheta2"          );

	info_mc.data_muon_py_ex.Fill_Data(_datatrain,"mc_muon_py_ex_");
	info_mc.num_muon_py_ex .Fill_Data(_datatrain,"mc_muon_py_ex_");
	info_mc.data_muon_py_im.Fill_Data(_datatrain,"mc_muon_py_im_");
	info_mc.num_muon_py_im .Fill_Data(_datatrain,"mc_muon_py_im_");
////_datatrain->Branch( "mc_muon_recovery_kcut_zmass"               , &info_mc.data_muon_recovery.kcut_zmass                 , "mc_muon_recovery_kcut_zmass"                   );
////_datatrain->Branch( "mc_muon_recovery_kcut_zpt"                 , &info_mc.data_muon_recovery.kcut_zpt                   , "mc_muon_recovery_kcut_zpt"                     );
////_datatrain->Branch( "mc_muon_recovery_kcut_invis_costheta"      , &info_mc.data_muon_recovery.kcut_invis_costheta        , "mc_muon_recovery_kcut_invis_costheta"          );
////_datatrain->Branch( "mc_muon_recovery_kcut_mva"                 , &info_mc.data_muon_recovery.kcut_mva                   , "mc_muon_recovery_kcut_mva"                     );
////_datatrain->Branch( "mc_muon_recovery_kcut_recoil_mass"         , &info_mc.data_muon_recovery.kcut_recoil_mass           , "mc_muon_recovery_kcut_recoil_mass"             );
////_datatrain->Branch( "mc_muon_recovery_kcut_vis_e"               , &info_mc.data_muon_recovery.kcut_vis_e                 , "mc_muon_recovery_kcut_vis_e"                   );
////_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta"        , &info_mc.data_muon_recovery.lep_pair_costheta          , "mc_muon_recovery_lep_pair_costheta"            );
////_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta_pair"   , &info_mc.data_muon_recovery.lep_pair_costheta_pair     , "mc_muon_recovery_lep_pair_costheta_pair"       );
////_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta_track1" , &info_mc.data_muon_recovery.lep_pair_costheta_track1   , "mc_muon_recovery_lep_pair_costheta_track1"     );        
////_datatrain->Branch( "mc_muon_recovery_lep_pair_costheta_track2" , &info_mc.data_muon_recovery.lep_pair_costheta_track2   , "mc_muon_recovery_lep_pair_costheta_track2"     );        
////_datatrain->Branch( "mc_muon_recovery_lep_pair_azimuth"         , &info_mc.data_muon_recovery.lep_pair_azimuth           , "mc_muon_recovery_lep_pair_azimuth"             );
////_datatrain->Branch( "mc_muon_recovery_lep_pair_azimuth_pair"    , &info_mc.data_muon_recovery.lep_pair_azimuth_pair      , "mc_muon_recovery_lep_pair_azimuth_pair"        );
////_datatrain->Branch( "mc_muon_recovery_lep_pair_azimuth_track1"  , &info_mc.data_muon_recovery.lep_pair_azimuth_track1    , "mc_muon_recovery_lep_pair_azimuth_track1"      );        
////_datatrain->Branch( "mc_muon_recovery_lep_pair_azimuth_track2"  , &info_mc.data_muon_recovery.lep_pair_azimuth_track2    , "mc_muon_recovery_lep_pair_azimuth_track2"      );        

////_datatrain->Branch( "mc_muon_recovery_track1_pt"                , &info_mc.data_muon_recovery.track1_pt                  , "mc_muon_recovery_track1_pt"                    );        
////_datatrain->Branch( "mc_muon_recovery_track1_e"                 , &info_mc.data_muon_recovery.track1_e                   , "mc_muon_recovery_track1_e"                     );        
////_datatrain->Branch( "mc_muon_recovery_track2_pt"                , &info_mc.data_muon_recovery.track2_pt                  , "mc_muon_recovery_track2_pt"                    );        
////_datatrain->Branch( "mc_muon_recovery_track2_e"                 , &info_mc.data_muon_recovery.track2_e                   , "mc_muon_recovery_track2_e"                     );        
////_datatrain->Branch( "mc_muon_recovery_total_photon_num"         , &info_mc.data_muon_recovery.total_photon_number        , "mc_muon_recovery_total_photon_num"             );
////_datatrain->Branch( "mc_muon_recovery_photon_num"               , &info_mc.data_muon_recovery.photon_number              , "mc_muon_recovery_photon_num"                   );
////_datatrain->Branch( "mc_muon_recovery_photon_energy1"           , &info_mc.data_muon_recovery.photon_energy1             , "mc_muon_recovery_photon_energy1"               );
////_datatrain->Branch( "mc_muon_recovery_photon_energy2"           , &info_mc.data_muon_recovery.photon_energy2             , "mc_muon_recovery_photon_energy2"               );
////_datatrain->Branch( "mc_muon_recovery_photon_costheta1"         , &info_mc.data_muon_recovery.photon_costheta1           , "mc_muon_recovery_photon_costheta1"             );
////_datatrain->Branch( "mc_muon_recovery_photon_costheta2"         , &info_mc.data_muon_recovery.photon_costheta2           , "mc_muon_recovery_photon_costheta2"             );
////_datatrain->Branch( "mc_muon_recovery_forward_photon_num"       , &info_mc.data_muon_recovery.forward_photon_number      , "mc_muon_recovery_forward_photon_num"           );
////_datatrain->Branch( "mc_muon_recovery_forward_photon_energy1"   , &info_mc.data_muon_recovery.forward_photon_energy1     , "mc_muon_recovery_forward_photon_energy1"       );
////_datatrain->Branch( "mc_muon_recovery_forward_photon_energy2"   , &info_mc.data_muon_recovery.forward_photon_energy2     , "mc_muon_recovery_forward_photon_energy2"       );
////_datatrain->Branch( "mc_muon_recovery_forward_photon_costheta1" , &info_mc.data_muon_recovery.forward_photon_costheta1   , "mc_muon_recovery_forward_photon_costheta1"     );
////_datatrain->Branch( "mc_muon_recovery_forward_photon_costheta2" , &info_mc.data_muon_recovery.forward_photon_costheta2   , "mc_muon_recovery_forward_photon_costheta2"     );


	info_mc.data_muon_de.Fill_Data(_datatrain,"mc_muon_");
	info_mc.num_muon_de .Fill_Data(_datatrain,"mc_muon_");
////_datatrain->Branch( "mc_muon_kcut_zmass"                        , &info_mc.data_muon.kcut_zmass                          , "mc_muon_kcut_zmass"                            );
////_datatrain->Branch( "mc_muon_kcut_zpt"                          , &info_mc.data_muon.kcut_zpt                            , "mc_muon_kcut_zpt"                              );
////_datatrain->Branch( "mc_muon_kcut_invis_costheta"               , &info_mc.data_muon.kcut_invis_costheta                 , "mc_muon_kcut_invis_costheta"                   );
////_datatrain->Branch( "mc_muon_kcut_mva"                          , &info_mc.data_muon.kcut_mva                            , "mc_muon_kcut_mva"                              );
////_datatrain->Branch( "mc_muon_kcut_recoil_mass"                  , &info_mc.data_muon.kcut_recoil_mass                    , "mc_muon_kcut_recoil_mass"                      );
////_datatrain->Branch( "mc_muon_kcut_vis_e"                        , &info_mc.data_muon.kcut_vis_e                          , "mc_muon_kcut_vis_e"                            );
////_datatrain->Branch( "mc_muon_lep_pair_costheta"                 , &info_mc.data_muon.lep_pair_costheta                   , "mc_muon_lep_pair_costheta"                     );
////_datatrain->Branch( "mc_muon_lep_pair_costheta_pair"            , &info_mc.data_muon.lep_pair_costheta_pair              , "mc_muon_lep_pair_costheta_pair"                );
////_datatrain->Branch( "mc_muon_lep_pair_costheta_track1"          , &info_mc.data_muon.lep_pair_costheta_track1            , "mc_muon_lep_pair_costheta_track1"              );        
////_datatrain->Branch( "mc_muon_lep_pair_costheta_track2"          , &info_mc.data_muon.lep_pair_costheta_track2            , "mc_muon_lep_pair_costheta_track2"              );        
////_datatrain->Branch( "mc_muon_lep_pair_azimuth"                  , &info_mc.data_muon.lep_pair_azimuth                    , "mc_muon_lep_pair_azimuth"                      );
////_datatrain->Branch( "mc_muon_lep_pair_azimuth_pair"             , &info_mc.data_muon.lep_pair_azimuth_pair               , "mc_muon_lep_pair_azimuth_pair"                 );
////_datatrain->Branch( "mc_muon_lep_pair_azimuth_track1"           , &info_mc.data_muon.lep_pair_azimuth_track1             , "mc_muon_lep_pair_azimuth_track1"               );        
////_datatrain->Branch( "mc_muon_lep_pair_azimuth_track2"           , &info_mc.data_muon.lep_pair_azimuth_track2             , "mc_muon_lep_pair_azimuth_track2"               );        


////_datatrain->Branch( "mc_muon_track1_pt"                         , &info_mc.data_muon.track1_pt                           , "mc_muon_track1_pt"                             );        
////_datatrain->Branch( "mc_muon_track1_e"                          , &info_mc.data_muon.track1_e                            , "mc_muon_track1_e"                              );        
////_datatrain->Branch( "mc_muon_track2_pt"                         , &info_mc.data_muon.track2_pt                           , "mc_muon_track2_pt"                             );        
////_datatrain->Branch( "mc_muon_track2_e"                          , &info_mc.data_muon.track2_e                            , "mc_muon_track2_e"                              );        
////_datatrain->Branch( "mc_muon_total_photon_num"                  , &info_mc.data_muon.total_photon_number                 , "mc_muon_total_photon_num"                      );
////_datatrain->Branch( "mc_muon_photon_num"                        , &info_mc.data_muon.photon_number                       , "mc_muon_photon_num"                            );
////_datatrain->Branch( "mc_muon_photon_energy1"                    , &info_mc.data_muon.photon_energy1                      , "mc_muon_photon_energy1"                        );
////_datatrain->Branch( "mc_muon_photon_energy2"                    , &info_mc.data_muon.photon_energy2                      , "mc_muon_photon_energy2"                        );
////_datatrain->Branch( "mc_muon_photon_costheta1"                  , &info_mc.data_muon.photon_costheta1                    , "mc_muon_photon_costheta1"                      );
////_datatrain->Branch( "mc_muon_photon_costheta2"                  , &info_mc.data_muon.photon_costheta2                    , "mc_muon_photon_costheta2"                      );
////_datatrain->Branch( "mc_muon_forward_photon_num"                , &info_mc.data_muon.forward_photon_number               , "mc_muon_forward_photon_num"                    );
////_datatrain->Branch( "mc_muon_forward_photon_energy1"            , &info_mc.data_muon.forward_photon_energy1              , "mc_muon_forward_photon_energy1"                );
////_datatrain->Branch( "mc_muon_forward_photon_energy2"            , &info_mc.data_muon.forward_photon_energy2              , "mc_muon_forward_photon_energy2"                );
////_datatrain->Branch( "mc_muon_forward_photon_costheta1"          , &info_mc.data_muon.forward_photon_costheta1            , "mc_muon_forward_photon_costheta1"              );
////_datatrain->Branch( "mc_muon_forward_photon_costheta2"          , &info_mc.data_muon.forward_photon_costheta2            , "mc_muon_forward_photon_costheta2"              );





	//partraine number
////_datatrain->Branch( "rc_lep_num_gen"                            , &info_rc.num_lep_gen.num                               , "rc_lep_num_gen"                                );
////_datatrain->Branch( "rc_muon_num_gen"                           , &info_rc.num_muon_gen.num                              , "rc_muon_num_gen"                               );
////_datatrain->Branch( "rc_muon_num_gen_plus"                      , &info_rc.num_muon_gen.num_plus                         , "rc_muon_num_gen_plus"                          );
////_datatrain->Branch( "rc_muon_num_gen_minus"                     , &info_rc.num_muon_gen.num_minus                        , "rc_muon_num_gen_minus"                         );
////_datatrain->Branch( "rc_lep_num_recovery"                       , &info_rc.num_lep_recovery.num                          , "rc_lep_num_recovery"                           );
////_datatrain->Branch( "rc_muon_num_recovery"                      , &info_rc.num_muon_recovery.num                         , "rc_muon_num_recovery"                          );
////_datatrain->Branch( "rc_muon_num_recovery_plus"                 , &info_rc.num_muon_recovery.num_plus                    , "rc_muon_num_recovery_plus"                     );
////_datatrain->Branch( "rc_muon_num_recovery_minus"                , &info_rc.num_muon_recovery.num_minus                   , "rc_muon_num_recovery_minus"                    );
////_datatrain->Branch( "rc_lep_num"                                , &info_rc.num_lep.num                                   , "rc_lep_num"                                    );
////_datatrain->Branch( "rc_muon_num"                               , &info_rc.num_muon.num                                  , "rc_muon_num"                                   );
////_datatrain->Branch( "rc_muon_num_plus"                          , &info_rc.num_muon.num_plus                             , "rc_muon_num_plus"                              );
////_datatrain->Branch( "rc_muon_num_minus"                         , &info_rc.num_muon.num_minus                            , "rc_muon_num_minus"                             );




	info_rc.data_muon_hs.Fill_Data(_datatrain,"rc_muon_gen_");
	info_rc.num_muon_hs .Fill_Data(_datatrain,"rc_muon_gen_");
////_datatrain->Branch( "rc_muon_gen_kcut_zmass"                    , &info_rc.data_muon_gen.kcut_zmass                      , "rc_muon_gen_kcut_zmass"                        );
////_datatrain->Branch( "rc_muon_gen_kcut_zpt"                      , &info_rc.data_muon_gen.kcut_zpt                        , "rc_muon_gen_kcut_zpt"                          );
////_datatrain->Branch( "rc_muon_gen_kcut_invis_costheta"           , &info_rc.data_muon_gen.kcut_invis_costheta             , "rc_muon_gen_kcut_invis_costheta"               );
////_datatrain->Branch( "rc_muon_gen_kcut_mva"                      , &info_rc.data_muon_gen.kcut_mva                        , "rc_muon_gen_kcut_mva"                          );
////_datatrain->Branch( "rc_muon_gen_kcut_recoil_mass"              , &info_rc.data_muon_gen.kcut_recoil_mass                , "rc_muon_gen_kcut_recoil_mass"                  );
////_datatrain->Branch( "rc_muon_gen_kcut_vis_e"                    , &info_rc.data_muon_gen.kcut_vis_e                      , "rc_muon_gen_kcut_vis_e"                        );
////_datatrain->Branch( "rc_muon_gen_lep_pair_costheta"             , &info_rc.data_muon_gen.lep_pair_costheta               , "rc_muon_gen_lep_pair_costheta"                 );
////_datatrain->Branch( "rc_muon_gen_lep_pair_costheta_pair"        , &info_rc.data_muon_gen.lep_pair_costheta_pair          , "rc_muon_gen_lep_pair_costheta_pair"            );
////_datatrain->Branch( "rc_muon_gen_lep_pair_costheta_track1"      , &info_rc.data_muon_gen.lep_pair_costheta_track1        , "rc_muon_gen_lep_pair_costheta_track1"          );        
////_datatrain->Branch( "rc_muon_gen_lep_pair_costheta_track2"      , &info_rc.data_muon_gen.lep_pair_costheta_track2        , "rc_muon_gen_lep_pair_costheta_track2"          );        
////_datatrain->Branch( "rc_muon_gen_lep_pair_azimuth"              , &info_rc.data_muon_gen.lep_pair_azimuth                , "rc_muon_gen_lep_pair_azimuth"                  );
////_datatrain->Branch( "rc_muon_gen_lep_pair_azimuth_pair"         , &info_rc.data_muon_gen.lep_pair_azimuth_pair           , "rc_muon_gen_lep_pair_azimuth_pair"             );
////_datatrain->Branch( "rc_muon_gen_lep_pair_azimuth_track1"       , &info_rc.data_muon_gen.lep_pair_azimuth_track1         , "rc_muon_gen_lep_pair_azimuth_track1"           );        
////_datatrain->Branch( "rc_muon_gen_lep_pair_azimuth_track2"       , &info_rc.data_muon_gen.lep_pair_azimuth_track2         , "rc_muon_gen_lep_pair_azimuth_track2"           );        

////_datatrain->Branch( "rc_muon_recovery_kcut_zmass"               , &info_rc.data_muon_recovery.kcut_zmass                 , "rc_muon_recovery_kcut_zmass"                   );
////_datatrain->Branch( "rc_muon_recovery_kcut_zpt"                 , &info_rc.data_muon_recovery.kcut_zpt                   , "rc_muon_recovery_kcut_zpt"                     );
////_datatrain->Branch( "rc_muon_recovery_kcut_invis_costheta"      , &info_rc.data_muon_recovery.kcut_invis_costheta        , "rc_muon_recovery_kcut_invis_costheta"          );
////_datatrain->Branch( "rc_muon_recovery_kcut_mva"                 , &info_rc.data_muon_recovery.kcut_mva                   , "rc_muon_recovery_kcut_mva"                     );
////_datatrain->Branch( "rc_muon_recovery_kcut_recoil_mass"         , &info_rc.data_muon_recovery.kcut_recoil_mass           , "rc_muon_recovery_kcut_recoil_mass"             );
////_datatrain->Branch( "rc_muon_recovery_kcut_vis_e"               , &info_rc.data_muon_recovery.kcut_vis_e                 , "rc_muon_recovery_kcut_vis_e"                   );
////_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta"        , &info_rc.data_muon_recovery.lep_pair_costheta          , "rc_muon_recovery_lep_pair_costheta"            );
////_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta_pair"   , &info_rc.data_muon_recovery.lep_pair_costheta_pair     , "rc_muon_recovery_lep_pair_costheta_pair"       );
////_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta_track1" , &info_rc.data_muon_recovery.lep_pair_costheta_track1   , "rc_muon_recovery_lep_pair_costheta_track1"     );        
////_datatrain->Branch( "rc_muon_recovery_lep_pair_costheta_track2" , &info_rc.data_muon_recovery.lep_pair_costheta_track2   , "rc_muon_recovery_lep_pair_costheta_track2"     );        
////_datatrain->Branch( "rc_muon_recovery_lep_pair_azimuth"         , &info_rc.data_muon_recovery.lep_pair_azimuth           , "rc_muon_recovery_lep_pair_azimuth"             );
////_datatrain->Branch( "rc_muon_recovery_lep_pair_azimuth_pair"    , &info_rc.data_muon_recovery.lep_pair_azimuth_pair      , "rc_muon_recovery_lep_pair_azimuth_pair"        );
////_datatrain->Branch( "rc_muon_recovery_lep_pair_azimuth_track1"  , &info_rc.data_muon_recovery.lep_pair_azimuth_track1    , "rc_muon_recovery_lep_pair_azimuth_track1"      );        
////_datatrain->Branch( "rc_muon_recovery_lep_pair_azimuth_track2"  , &info_rc.data_muon_recovery.lep_pair_azimuth_track2    , "rc_muon_recovery_lep_pair_azimuth_track2"      );        

	info_rc.data_muon_de.Fill_Data(_datatrain,"rc_muon_");
	info_rc.num_muon_de .Fill_Data(_datatrain,"rc_muon_");
////_datatrain->Branch( "rc_muon_kcut_zmass"                        , &info_rc.data_muon.kcut_zmass                          , "rc_muon_kcut_zmass"                            );
////_datatrain->Branch( "rc_muon_kcut_zpt"                          , &info_rc.data_muon.kcut_zpt                            , "rc_muon_kcut_zpt"                              );
////_datatrain->Branch( "rc_muon_kcut_invis_costheta"               , &info_rc.data_muon.kcut_invis_costheta                 , "rc_muon_kcut_invis_costheta"                   );
////_datatrain->Branch( "rc_muon_kcut_mva"                          , &info_rc.data_muon.kcut_mva                            , "rc_muon_kcut_mva"                              );
////_datatrain->Branch( "rc_muon_kcut_recoil_mass"                  , &info_rc.data_muon.kcut_recoil_mass                    , "rc_muon_kcut_recoil_mass"                      );
////_datatrain->Branch( "rc_muon_kcut_vis_e"                        , &info_rc.data_muon.kcut_vis_e                          , "rc_muon_kcut_vis_e"                            );
////_datatrain->Branch( "rc_muon_lep_pair_costheta"                 , &info_rc.data_muon.lep_pair_costheta                   , "rc_muon_lep_pair_costheta"                     );
////_datatrain->Branch( "rc_muon_lep_pair_costheta_pair"            , &info_rc.data_muon.lep_pair_costheta_pair              , "rc_muon_lep_pair_costheta_pair"                );
////_datatrain->Branch( "rc_muon_lep_pair_costheta_track1"          , &info_rc.data_muon.lep_pair_costheta_track1            , "rc_muon_lep_pair_costheta_track1"              );        
////_datatrain->Branch( "rc_muon_lep_pair_costheta_track2"          , &info_rc.data_muon.lep_pair_costheta_track2            , "rc_muon_lep_pair_costheta_track2"              );        
////_datatrain->Branch( "rc_muon_lep_pair_azimuth"                  , &info_rc.data_muon.lep_pair_azimuth                    , "rc_muon_lep_pair_azimuth"                      );
////_datatrain->Branch( "rc_muon_lep_pair_azimuth_pair"             , &info_rc.data_muon.lep_pair_azimuth_pair               , "rc_muon_lep_pair_azimuth_pair"                 );
////_datatrain->Branch( "rc_muon_lep_pair_azimuth_track1"           , &info_rc.data_muon.lep_pair_azimuth_track1             , "rc_muon_lep_pair_azimuth_track1"               );        
////_datatrain->Branch( "rc_muon_lep_pair_azimuth_track2"           , &info_rc.data_muon.lep_pair_azimuth_track2             , "rc_muon_lep_pair_azimuth_track2"               );        


////_datatrain->Branch( "rc_muon_track1_pt"                         , &info_rc.data_muon.track1_pt                           , "rc_muon_track1_pt"                             );        
////_datatrain->Branch( "rc_muon_track1_e"                          , &info_rc.data_muon.track1_e                            , "rc_muon_track1_e"                              );        
////_datatrain->Branch( "rc_muon_track2_pt"                         , &info_rc.data_muon.track2_pt                           , "rc_muon_track2_pt"                             );        
////_datatrain->Branch( "rc_muon_track2_e"                          , &info_rc.data_muon.track2_e                            , "rc_muon_track2_e"                              );        





	//partraine number
////_datatrain->Branch( "po_lep_num_gen"                            , &info_po.num_lep_gen.num                               , "po_lep_num_gen"                                );
////_datatrain->Branch( "po_muon_num_gen"                           , &info_po.num_muon_gen.num                              , "po_muon_num_gen"                               );
////_datatrain->Branch( "po_muon_num_gen_plus"                      , &info_po.num_muon_gen.num_plus                         , "po_muon_num_gen_plus"                          );
////_datatrain->Branch( "po_muon_num_gen_minus"                     , &info_po.num_muon_gen.num_minus                        , "po_muon_num_gen_minus"                         );
////_datatrain->Branch( "po_lep_num_recovery"                       , &info_po.num_lep_recovery.num                          , "po_lep_num_recovery"                           );
////_datatrain->Branch( "po_muon_num_recovery"                      , &info_po.num_muon_recovery.num                         , "po_muon_num_recovery"                          );
////_datatrain->Branch( "po_muon_num_recovery_plus"                 , &info_po.num_muon_recovery.num_plus                    , "po_muon_num_recovery_plus"                     );
////_datatrain->Branch( "po_muon_num_recovery_minus"                , &info_po.num_muon_recovery.num_minus                   , "po_muon_num_recovery_minus"                    );
////_datatrain->Branch( "po_lep_num"                                , &info_po.num_lep.num                                   , "po_lep_num"                                    );
////_datatrain->Branch( "po_muon_num"                               , &info_po.num_muon.num                                  , "po_muon_num"                                   );
////_datatrain->Branch( "po_muon_num_minus"                         , &info_po.num_muon.num_minus                            , "po_muon_num_minus"                             );




	info_po.data_muon_py_im.Fill_Data(_datatrain,"po_muon_py_im_");
	info_po.num_muon_py_im .Fill_Data(_datatrain,"po_muon_py_im_");
////_datatrain->Branch( "po_muon_gen_kcut_zmass"                    , &info_po.data_muon_gen.kcut_zmass                      , "po_muon_gen_kcut_zmass"                        );
////_datatrain->Branch( "po_muon_gen_kcut_zpt"                      , &info_po.data_muon_gen.kcut_zpt                        , "po_muon_gen_kcut_zpt"                          );
////_datatrain->Branch( "po_muon_gen_kcut_invis_costheta"           , &info_po.data_muon_gen.kcut_invis_costheta             , "po_muon_gen_kcut_invis_costheta"               );
////_datatrain->Branch( "po_muon_gen_kcut_mva"                      , &info_po.data_muon_gen.kcut_mva                        , "po_muon_gen_kcut_mva"                          );
////_datatrain->Branch( "po_muon_gen_kcut_recoil_mass"              , &info_po.data_muon_gen.kcut_recoil_mass                , "po_muon_gen_kcut_recoil_mass"                  );
////_datatrain->Branch( "po_muon_gen_kcut_vis_e"                    , &info_po.data_muon_gen.kcut_vis_e                      , "po_muon_gen_kcut_vis_e"                        );
////_datatrain->Branch( "po_muon_gen_lep_pair_costheta"             , &info_po.data_muon_gen.lep_pair_costheta               , "po_muon_gen_lep_pair_costheta"                 );
////_datatrain->Branch( "po_muon_gen_lep_pair_costheta_pair"        , &info_po.data_muon_gen.lep_pair_costheta_pair          , "po_muon_gen_lep_pair_costheta_pair"            );
////_datatrain->Branch( "po_muon_gen_lep_pair_costheta_track1"      , &info_po.data_muon_gen.lep_pair_costheta_track1        , "po_muon_gen_lep_pair_costheta_track1"          );        
////_datatrain->Branch( "po_muon_gen_lep_pair_costheta_track2"      , &info_po.data_muon_gen.lep_pair_costheta_track2        , "po_muon_gen_lep_pair_costheta_track2"          );        
////_datatrain->Branch( "po_muon_gen_lep_pair_azimuth"              , &info_po.data_muon_gen.lep_pair_azimuth                , "po_muon_gen_lep_pair_azimuth"                  );
////_datatrain->Branch( "po_muon_gen_lep_pair_azimuth_pair"         , &info_po.data_muon_gen.lep_pair_azimuth_pair           , "po_muon_gen_lep_pair_azimuth_pair"             );
////_datatrain->Branch( "po_muon_gen_lep_pair_azimuth_track1"       , &info_po.data_muon_gen.lep_pair_azimuth_track1         , "po_muon_gen_lep_pair_azimuth_track1"           );        
////_datatrain->Branch( "po_muon_gen_lep_pair_azimuth_track2"       , &info_po.data_muon_gen.lep_pair_azimuth_track2         , "po_muon_gen_lep_pair_azimuth_track2"           );        

////_datatrain->Branch( "po_muon_recovery_kcut_zmass"               , &info_po.data_muon_recovery.kcut_zmass                 , "po_muon_recovery_kcut_zmass"                   );
////_datatrain->Branch( "po_muon_recovery_kcut_zpt"                 , &info_po.data_muon_recovery.kcut_zpt                   , "po_muon_recovery_kcut_zpt"                     );
////_datatrain->Branch( "po_muon_recovery_kcut_invis_costheta"      , &info_po.data_muon_recovery.kcut_invis_costheta        , "po_muon_recovery_kcut_invis_costheta"          );
////_datatrain->Branch( "po_muon_recovery_kcut_mva"                 , &info_po.data_muon_recovery.kcut_mva                   , "po_muon_recovery_kcut_mva"                     );
////_datatrain->Branch( "po_muon_recovery_kcut_recoil_mass"         , &info_po.data_muon_recovery.kcut_recoil_mass           , "po_muon_recovery_kcut_recoil_mass"             );
////_datatrain->Branch( "po_muon_recovery_kcut_vis_e"               , &info_po.data_muon_recovery.kcut_vis_e                 , "po_muon_recovery_kcut_vis_e"                   );
////_datatrain->Branch( "po_muon_recovery_lep_pair_azimuth"         , &info_po.data_muon_recovery.lep_pair_azimuth           , "po_muon_recovery_lep_pair_azimuth"             );
////_datatrain->Branch( "po_muon_recovery_lep_pair_azimuth_pair"    , &info_po.data_muon_recovery.lep_pair_azimuth_pair      , "po_muon_recovery_lep_pair_azimuth_pair"        );
////_datatrain->Branch( "po_muon_recovery_lep_pair_azimuth_track1"  , &info_po.data_muon_recovery.lep_pair_azimuth_track1    , "po_muon_recovery_lep_pair_azimuth_track1"      );        
////_datatrain->Branch( "po_muon_recovery_lep_pair_azimuth_track2"  , &info_po.data_muon_recovery.lep_pair_azimuth_track2    , "po_muon_recovery_lep_pair_azimuth_track2"      );        

	info_po.data_muon_de.Fill_Data(_datatrain,"po_muon_");
	info_po.num_muon_de .Fill_Data(_datatrain,"po_muon_");
////_datatrain->Branch( "po_muon_kcut_zmass"                        , &info_po.data_muon.kcut_zmass                          , "po_muon_kcut_zmass"                            );
////_datatrain->Branch( "po_muon_kcut_zpt"                          , &info_po.data_muon.kcut_zpt                            , "po_muon_kcut_zpt"                              );
////_datatrain->Branch( "po_muon_kcut_invis_costheta"               , &info_po.data_muon.kcut_invis_costheta                 , "po_muon_kcut_invis_costheta"                   );
////_datatrain->Branch( "po_muon_kcut_mva"                          , &info_po.data_muon.kcut_mva                            , "po_muon_kcut_mva"                              );
////_datatrain->Branch( "po_muon_kcut_recoil_mass"                  , &info_po.data_muon.kcut_recoil_mass                    , "po_muon_kcut_recoil_mass"                      );
////_datatrain->Branch( "po_muon_kcut_vis_e"                        , &info_po.data_muon.kcut_vis_e                          , "po_muon_kcut_vis_e"                            );
////_datatrain->Branch( "po_muon_lep_pair_costheta"                 , &info_po.data_muon.lep_pair_costheta                   , "po_muon_lep_pair_costheta"                     );
////_datatrain->Branch( "po_muon_lep_pair_costheta_pair"            , &info_po.data_muon.lep_pair_costheta_pair              , "po_muon_lep_pair_costheta_pair"                );
////_datatrain->Branch( "po_muon_lep_pair_costheta_track1"          , &info_po.data_muon.lep_pair_costheta_track1            , "po_muon_lep_pair_costheta_track1"              );        
////_datatrain->Branch( "po_muon_lep_pair_costheta_track2"          , &info_po.data_muon.lep_pair_costheta_track2            , "po_muon_lep_pair_costheta_track2"              );        
////_datatrain->Branch( "po_muon_lep_pair_azimuth"                  , &info_po.data_muon.lep_pair_azimuth                    , "po_muon_lep_pair_azimuth"                      );
////_datatrain->Branch( "po_muon_lep_pair_azimuth_pair"             , &info_po.data_muon.lep_pair_azimuth_pair               , "po_muon_lep_pair_azimuth_pair"                 );
////_datatrain->Branch( "po_muon_lep_pair_azimuth_track1"           , &info_po.data_muon.lep_pair_azimuth_track1             , "po_muon_lep_pair_azimuth_track1"               );        
////_datatrain->Branch( "po_muon_lep_pair_azimuth_track2"           , &info_po.data_muon.lep_pair_azimuth_track2             , "po_muon_lep_pair_azimuth_track2"               );        
////_datatrain->Branch( "po_muon_track1_pt"                         , &info_po.data_muon.track1_pt                           , "po_muon_track1_pt"                             );        
////_datatrain->Branch( "po_muon_track1_e"                          , &info_po.data_muon.track1_e                            , "po_muon_track1_e"                              );        
////_datatrain->Branch( "po_muon_track2_pt"                         , &info_po.data_muon.track2_pt                           , "po_muon_track2_pt"                             );        
////_datatrain->Branch( "po_muon_track2_e"                          , &info_po.data_muon.track2_e                            , "po_muon_track2_e"                              );        
////_datatrain->Branch( "po_muon_total_photon_num"                  , &info_po.data_muon.total_photon_number                 , "po_muon_total_photon_num"                      );
////_datatrain->Branch( "po_muon_photon_num"                        , &info_po.data_muon.photon_number                       , "po_muon_photon_num"                            );
////_datatrain->Branch( "po_muon_photon_energy1"                    , &info_po.data_muon.photon_energy1                      , "po_muon_photon_energy1"                        );
////_datatrain->Branch( "po_muon_photon_energy2"                    , &info_po.data_muon.photon_energy2                      , "po_muon_photon_energy2"                        );
////_datatrain->Branch( "po_muon_photon_costheta1"                  , &info_po.data_muon.photon_costheta1                    , "po_muon_photon_costheta1"                      );
////_datatrain->Branch( "po_muon_photon_costheta2"                  , &info_po.data_muon.photon_costheta2                    , "po_muon_photon_costheta2"                      );
////_datatrain->Branch( "po_muon_forward_photon_num"                , &info_po.data_muon.forward_photon_number               , "po_muon_forward_photon_num"                    );
////_datatrain->Branch( "po_muon_forward_photon_energy1"            , &info_po.data_muon.forward_photon_energy1              , "po_muon_forward_photon_energy1"                );
////_datatrain->Branch( "po_muon_forward_photon_energy2"            , &info_po.data_muon.forward_photon_energy2              , "po_muon_forward_photon_energy2"                );
////_datatrain->Branch( "po_muon_forward_photon_costheta1"          , &info_po.data_muon.forward_photon_costheta1            , "po_muon_forward_photon_costheta1"              );
////_datatrain->Branch( "po_muon_forward_photon_costheta2"          , &info_po.data_muon.forward_photon_costheta2            , "po_muon_forward_photon_costheta2"              );









	return;
}

