#ifndef MyProcessor_h
#define MyProcessor_h  

#include "marlin/Processor.h"
#include "lcio.h"
#include "EVENT/MCParticle.h"
#include "UTIL/LCRelationNavigator.h"
#include <string>
//#include "Print.h"
#include "CPrint.h"
#include "Fvector.h"
#include "TRandom.h"
#include "Debug.h"
#include "Element_DataType.h"
#include "CMC.h"
#include "CChain.h"

using namespace lcio;
using namespace marlin;
using namespace IMPL;
using ToolSet::operator<<;

class TFile;
class TTree;
class CDebug;

namespace EVENT{
	class ReconstructedParticle;
	class Track;
	class Cluster;
}

typedef std::map<std::string,float>                                    ACounterType;
typedef std::map<std::string,std::vector<MCParticle*> >                MCSpeciesType;
typedef std::map<std::string,std::vector<ReconstructedParticle*> >     RCSpeciesType;
typedef std::map<std::string,int >                                     ASpeciesNumType;

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



		bool analyseMCParticle( LCCollection* Allmc,LCCollection* MCsWithoutIsoPhoton,LCCollection* MCsPhoton,LCCollection* MCsForwardPhoton,   Infomation &info) ;
		std::vector<MCParticle*> checkMCFinalState_GenLevel(LCCollection* AllMC) ;
		std::vector<MCParticle*> checkMCFinalState_PythiaLevel(LCCollection* AllMC) ;
		std::vector<MCParticle*> checkMCParticleType(std::vector<MCParticle*> MCs,signed int PDG) ;
		bool MCCutElec( std::vector<MCParticle*> MCs) ;
		bool MCCutMuon( std::vector<MCParticle*> MCs) ;
		bool checkMCIsoLepCone(std::vector<MCParticle*> IsoLep, std::vector<MCParticle*> PFOs_WO_IsoLep, std::vector<MCParticle*> &NewIsoLep, std::vector<MCParticle*> &NewPFOs_WO_IsoLep, Infomation &info);
		bool IsInLepCone( MCParticle* lep, MCParticle* pfo , Infomation &info) ;
		bool MCCutDetail(std::vector<MCParticle*> &PFOsWithoutIsoleps, std::vector<MCParticle*> &Isoleps,std::vector<MCParticle*> &choosed_lep,Variable &vari);
        void MCCutPhoton(std::vector<MCParticle*> &MCsPhoton,  Variable &vari);
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
		bool analysePhysicalObject(LCCollection* PFOs_col, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,LCCollection* PandoraPFOsWithoutIsoPhoton,LCCollection* PandoraIsoPhoton,LCCollection* PandoraIsoForwardPhoton,Infomation &info) ;
		std::vector<ReconstructedParticle*> checkPOFinalState(LCCollection* AllPOs) ;
		std::vector<ReconstructedParticle*> checkPOParticleType(std::vector<ReconstructedParticle*> POs,int PDG) ;
		std::vector<ReconstructedParticle*> CheckPOParticlePairType( std::vector<ReconstructedParticle*> PFOs,int PDG, int counter_minus, int counter_plus, int counter_pair) ;
		bool POCutElec( std::vector<ReconstructedParticle*> POs) ;
		bool POCutMuon( std::vector<ReconstructedParticle*> &muon,std::vector<ReconstructedParticle*> &muon_plus,std::vector<ReconstructedParticle*> &muon_minus ,Variable& data) ;

		bool POCutDetail(std::vector<ReconstructedParticle*> &PFOsWithoutIsoleps, std::vector<ReconstructedParticle*> &Isoleps,std::vector<ReconstructedParticle*> &choosed_lep,  Variable &vari);
        void POCutPhoton(std::vector<ReconstructedParticle*> &IsoPhoton, std::vector<ReconstructedParticle*> &IsoForwardPhoton,  Variable &vari);
		ReconstructedParticle* getVisible(std::vector<ReconstructedParticle*> in);
		ReconstructedParticle* getInVisible(std::vector<ReconstructedParticle*> in);





		bool analyseTrack(int event_num,  LCCollection* MC_col, LCCollection* PFOs_col, LCRelationNavigator* navpfo, LCRelationNavigator* navmc, LCCollection* PFOsWithoutIsoleps, LCCollection* Isoleps,Infomation &info_mc,Infomation &info_rc, Infomation &info_po) ;
		void TrackGetSource(std::vector<ReconstructedParticle*> &source, std::vector<std::vector<MCParticle*> >  &to, LCRelationNavigator* &relation);
		void TrackGetObject(std::vector<MCParticle*> &source, std::vector<std::vector<ReconstructedParticle*> >  &to, LCRelationNavigator* &relation);
		int TrackAnaMC(int event_num,  std::vector<MCParticle*> MCs, LCRelationNavigator* navmc, Infomation &mcinfo) ;
		void TrackPayBackMC(int event_num,  std::vector<MCParticle*> MCs, LCRelationNavigator* navmc, Infomation &mcinfo) ;
		int TrackAnaRC(int event_num,  std::vector<ReconstructedParticle*> PFOs, LCRelationNavigator* navpfo, Infomation &rcinfo) ;
		bool TrackAnaISL(int event_num,  LCRelationNavigator* navpfo, std::vector<ReconstructedParticle*> pfoWOleps, std::vector<ReconstructedParticle*> leps, Infomation &poinfo) ;
		//		template <class TPAIREVENT>
		//			void getPOParticlePairEvent( std::vector<TPAIREVENT> PFOs,int PID, PAIR_EVENT<TPAIREVENT> &pair) ;
		//void getPOParticlePairEvent( std::vector<ReconstructedParticle*> PFOs,int PID, PAIR_EVENT<ReconstructedParticle*> &pair) ;
		//Input collection name.
		std::string _colAllPFOs;
		std::string _colIsoleps;
		std::string _colPFOsWithoutIsoleps;

		std::string _colPandoraIsoPhoton;
		std::string _colPandoraIsoForwardPhoton;
		std::string _colPandoraPFOsWithoutIsoPhoton;

		std::string _colMCsPhoton;
		std::string _colMCsForwardPhoton;
		std::string _colMCsWithoutIsoPhoton;

		std::string _colMC;
		std::string _mcpfoRelation;
		std::string _pfomcRelation;
		LCCollection* _mcpCol;
		LCRelationNavigator* _navpfo;
		LCRelationNavigator* _navmc;
		// output root file name
		std::string _rootfilename;
		std::string _processList;

		float _hmass;
		float _cmenergy;
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


		ToolSet::CChain_Vec         _photon_chain ;
		ToolSet::CChain_Vec         _muon_chain ;
		ToolSet::CChain_Vec         _FS_muon_chain ;
		ToolSet::CChain_Vec         _lepton_chain ;
		ToolSet::CChain_Vec         _hadron_chain ;

	public:
		class ASpecies{
			private:
				bool IsUsed ;

			public:
				MCSpeciesType         _mcspecies;
				RCSpeciesType         _rcspecies;
				ASpeciesNumType       _mcnum;
				ASpeciesNumType       _rcnum;

				ASpecies(){
					Clear();
				}

				~ASpecies(){
					Clear();
				}


				// /****************************************************************************
				/// @Name: Num 
				///
				/// @Returns: 
				// ****************************************************************************/
				ASpeciesNumType MCNum(){
					return(_mcnum);
				}

				ASpeciesNumType RCNum(){
					return(_rcnum);
				}

				// /****************************************************************************
				/// @Name: setNum 
				// ****************************************************************************/
				void setMCNum(){
					for(MCSpeciesType::iterator it = _mcspecies.begin(); it!=_mcspecies.end(); ++it ){
						_mcnum[it->first]=it->second.size();
					}
				}

				void setRCNum(){
					for(RCSpeciesType::iterator it = _rcspecies.begin(); it!=_rcspecies.end(); ++it ){
						_rcnum[it->first]=it->second.size();
					}
				}

				MCSpeciesType MCSpecies(){
					return(_mcspecies);
				}

				RCSpeciesType RCSpecies(){
					return(_rcspecies);
				}

				void get_mcparticle_species(std::vector<MCParticle*> &allparticle);
				void get_rcparticle_species(std::vector<ReconstructedParticle*> &allparticle);

				// /****************************************************************************
				/// @Name: Clear 
				// ****************************************************************************/
				void Clear();
				void Init ();


		};


		class ACounter{ 
			private:
				float                _weight;
				ACounterType         _counter;
			public:
				ACounter(){
					Clear();
				}

				~ACounter(){
					Clear();
				}


				void Clear();



				void setWeight(float value);

				void setCounterSingle(std::string event_name,float num);

				void setMCCounter(ASpecies& spe);

				void setRCCounter(ASpecies& spe);

				void showCounter();
		};


	protected:
		ACounter           _counter      ;
		ASpecies           _species;

};

std::ostream & operator<<(std::ostream & ostr, MyProcessor::ASpecies spe);
std::ostream & operator<<(std::ostream & ostr, MyProcessor::ASpecies* spe);
std::ostream & operator<<(std::ostream & ostr, MCSpeciesType spe);
std::ostream & operator<<(std::ostream & ostr, RCSpeciesType spe);
std::ostream & operator<<(std::ostream & ostr, ASpeciesNumType spe);
#endif



