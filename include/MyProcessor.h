#ifndef MyProcessor_h
#define MyProcessor_h  

#include "marlin/Processor.h"
#include "lcio.h"
#include "EVENT/MCParticle.h"
#include "UTIL/LCRelationNavigator.h"
#include <string>
#include "Print.h"
#include "Fvector.h"
#include "TRandom.h"
#include "Debug.h"
#include "Print.h"
#include "Element_DataType.h"

using namespace lcio;
using namespace marlin;
using namespace IMPL;

class TFile;
class TTree;
class CDebug;

namespace EVENT{
	class ReconstructedParticle;
	class Track;
	class Cluster;
}

class MyProcessor : public Processor{

	public:

		virtual Processor* newProcessor() { return new MyProcessor; }

		MyProcessor();

		//Called at the begin of the job before anything is read.
		//Use to initialize the processor, e.g. book histograms.
		virtual void init();

		//Called for every run.
		virtual void processRunHeader( LCRunHeader* run );

		//Called for every event - the working horse.
		virtual void processEvent( LCEvent* evt ); 

		virtual void check( LCEvent* evt ); 

		//Called after data processing for clean up.
		virtual void end();

	protected:

		//Prepare NTuple to check the data
		void makeNTuple();
		template<typename TOBVRECOIL>
			bool obvRecoil(std::vector<TOBVRECOIL> in, std::vector<TOBVRECOIL> &out) ;



		bool analyseMCParticle( LCCollection* Allmc,Infomation &info) ;
		std::vector<MCParticle*> checkMCFinalState_GenLevel(LCCollection* AllMC) ;
		std::vector<MCParticle*> checkMCFinalState_PythiaLevel(LCCollection* AllMC) ;
		std::vector<MCParticle*> checkMCParticleType(std::vector<MCParticle*> MCs,signed int PDG) ;
		bool MCCutElec( std::vector<MCParticle*> MCs) ;
		bool MCCutMuon( std::vector<MCParticle*> MCs) ;
		bool checkMCIsoLepCone(std::vector<MCParticle*> IsoLep, std::vector<MCParticle*> PFOs_WO_IsoLep, std::vector<MCParticle*> &NewIsoLep, std::vector<MCParticle*> &NewPFOs_WO_IsoLep, Infomation &info);
		bool IsInLepCone( MCParticle* lep, MCParticle* pfo , Infomation &info) ;
		bool MCCutDetail(std::vector<MCParticle*> &PFOsWithoutIsoleps, std::vector<MCParticle*> &Isoleps,std::vector<MCParticle*> &choosed_lep,Variable &vari);
		MCParticle* getInVisible(std::vector<MCParticle*> in);
		MCParticle* getVisible(std::vector<MCParticle*> in);



		bool analyseRCParticle( LCCollection* AllPFO,Infomation &info) ;
		std::vector<ReconstructedParticle*> checkRCIsoLeptons(std::vector<ReconstructedParticle*> AllPOs) ;
		void checkRCIsoLeptons(std::vector<ReconstructedParticle*> AllPOs, std::vector<ReconstructedParticle*> &IsoLep, std::vector<ReconstructedParticle*> &PFOs_WO_IsoLep,Infomation &info);
		bool IsIsoLep( ReconstructedParticle* pfo ,Infomation &info) ;
		bool IsCharged( ReconstructedParticle* pfo ) ;
		int  IsLepton( ReconstructedParticle* pfo ,Infomation &info) ;
		bool PassesElectronImpactParameterCuts( ReconstructedParticle* pfo ,Infomation &info) ;
		bool PassesElectronImpactParameterSignificanceCuts( ReconstructedParticle* pfo ,Infomation &info) ;
		bool PassesMuonImpactParameterCuts( ReconstructedParticle* pfo ,Infomation &info) ;
		bool PassesMuonImpactParameterSignificanceCuts( ReconstructedParticle* pfo ,Infomation &info) ;
		void getCalEnergy( ReconstructedParticle* pfo , float* cale) ;
		bool checkRCIsoLepCone(std::vector<ReconstructedParticle*> IsoLep, std::vector<ReconstructedParticle*> PFOs_WO_IsoLep, std::vector<ReconstructedParticle*> &NewIsoLep, std::vector<ReconstructedParticle*> &NewPFOs_WO_IsoLep,Infomation &info);
		bool IsInLepCone( ReconstructedParticle* lep, ReconstructedParticle* pfo ,Infomation &info) ;

		bool RCCutElec( std::vector<ReconstructedParticle*> RCs) ;
		bool RCCutMuon( std::vector<ReconstructedParticle*> &muon,std::vector<ReconstructedParticle*> &muon_plus,std::vector<ReconstructedParticle*> &muon_minus ,Variable& data) ;






		//		bool analysePhysicalObject( LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &info) ;
		bool analysePhysicalObject(LCCollection* PFOs_col, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &info);
		std::vector<ReconstructedParticle*> checkPOFinalState(LCCollection* AllPOs) ;
		std::vector<ReconstructedParticle*> checkPOParticleType(std::vector<ReconstructedParticle*> POs,int PDG) ;
		std::vector<ReconstructedParticle*> CheckPOParticlePairType( std::vector<ReconstructedParticle*> PFOs,int PDG, int counter_minus, int counter_plus, int counter_pair) ;
		bool POCutElec( std::vector<ReconstructedParticle*> POs) ;
		bool POCutMuon( std::vector<ReconstructedParticle*> &muon,std::vector<ReconstructedParticle*> &muon_plus,std::vector<ReconstructedParticle*> &muon_minus ,Variable& data) ;

		bool POCutDetail(std::vector<ReconstructedParticle*> &PFOsWithoutIsoleps, std::vector<ReconstructedParticle*> &Isoleps,std::vector<ReconstructedParticle*> &choosed_lep,  Variable &vari);
		ReconstructedParticle* getVisible(std::vector<ReconstructedParticle*> in);
		ReconstructedParticle* getInVisible(std::vector<ReconstructedParticle*> in);





		bool analyseTrack(int event_num,  LCCollection* PFOs_col, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &info) ;
		//		template <class TPAIREVENT>
		//			void getPOParticlePairEvent( std::vector<TPAIREVENT> PFOs,int PID, PAIR_EVENT<TPAIREVENT> &pair) ;
		//void getPOParticlePairEvent( std::vector<ReconstructedParticle*> PFOs,int PID, PAIR_EVENT<ReconstructedParticle*> &pair) ;
		//Input collection name.
		std::string _colAllPFOs;
		std::string _colIsoleps;
		std::string _colPFOsWithoutIsoleps;
		std::string _colMC;
		std::string _mcpfoRelation;
		LCCollection* _mcpCol;
		LCRelationNavigator* _navpfo;
		// output root file name
		std::string _rootfilename;
		std::string _processList;

		float _hmass;
		int _nRun;
		int _nEvt;

		int pass_something;

		Infomation info_po;
		Infomation info_mc;
		Infomation info_rc;
		Infomation info_track;

		Counter   counter;
		CDebug    debug;

		//ROOT
		TFile* _otfile;
		TTree* _datatrain;
};

#endif



