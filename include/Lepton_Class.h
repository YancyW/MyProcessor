#ifndef LEPTON_CLASS_H
#define LEPTON_CLASS_H  
#include "TTree.h"
#include "MCOperator.h"
#include "libformat.h"
class CLEPTON_CUT{
	public:
		float electronMinEnergyDepositByMomentum;
		float electronMaxEnergyDepositByMomentum;
		float electronMinEcalToHcalFraction;
		float electronMaxEcalToHcalFraction;
		float electronMinTrackEnergy;
		float electronMaxTrackEnergy;

		float electronMinD0;
		float electronMaxD0;
		float electronMinZ0;
		float electronMaxZ0;
		float electronMinR0;
		float electronMaxR0;
		float electronMinD0Sig;
		float electronMaxD0Sig;
		float electronMinZ0Sig;
		float electronMaxZ0Sig;
		float electronMinR0Sig;
		float electronMaxR0Sig;

		float electroncosConeAngle;


		float muonMinEnergyDepositByMomentum;
		float muonMaxEnergyDepositByMomentum;
		float muonMinEcalToHcalFraction;
		float muonMaxEcalToHcalFraction;
		float muonMinTrackEnergy;
		float muonMaxTrackEnergy;
		float muonMinEyoke;
		float muonMaxEyoke;

		float muonMinD0;
		float muonMaxD0;
		float muonMinZ0;
		float muonMaxZ0;
		float muonMinR0;
		float muonMaxR0;
		float muonMinD0Sig;
		float muonMaxD0Sig;
		float muonMinZ0Sig;
		float muonMaxZ0Sig;
		float muonMinR0Sig;
		float muonMaxR0Sig;

		float cosConeAngle;

		void Read_Cut(){
			this->electronMinTrackEnergy=5.0;
			this->electronMaxTrackEnergy=100000.0;
			this->electronMinEnergyDepositByMomentum=0.5;
			this->electronMaxEnergyDepositByMomentum=1.3;
			this->electronMinEcalToHcalFraction=0.9;
			this->electronMaxEcalToHcalFraction=100000.0;

			this->muonMinTrackEnergy=5.0;
			this->muonMaxTrackEnergy=100000.0;   
			this->muonMinEnergyDepositByMomentum=0.0;
			this->muonMaxEnergyDepositByMomentum=0.3;
			this->muonMinEcalToHcalFraction=0.0;
			this->muonMaxEcalToHcalFraction=100000.0;
			this->muonMinEyoke=1.2;
			this->muonMaxEyoke=100000.0;

			this->electronMinD0=0.0;
			this->electronMaxD0=100000.0;
			this->electronMinZ0=0.0;
			this->electronMaxZ0=100000.0;
			this->electronMinR0=0.0;
			this->electronMaxR0=100000.0;
			this->electronMinD0Sig=0.0;
			this->electronMaxD0Sig=50.0;
			this->electronMinZ0Sig=0.0;
			this->electronMaxZ0Sig=5.0;
			this->electronMinR0Sig=0.0;
			this->electronMaxR0Sig=100000.0;

			this->muonMinD0=0.0;
			this->muonMaxD0=100000.0;
			this->muonMinZ0=0.0;
			this->muonMaxZ0=100000.0;
			this->muonMinR0=0.0;
			this->muonMaxR0=100000.0;
			this->muonMinD0Sig=0.0;
			this->muonMaxD0Sig=5.0;
			this->muonMinZ0Sig=0.0;
			this->muonMaxZ0Sig=5.0;
			this->muonMinR0Sig=0.0;
			this->muonMaxR0Sig=100000.0;

			this->cosConeAngle=0.98;
		}



		CLEPTON_CUT(){
			Read_Cut();
		}
};



template <typename TPAIR>
class PAIR_EVENT{
	private:
		int num_plus;
		int num_minus;
		int num_pair;
		int num_ds_pair;
		int num_ds_combine;
	public:
		std::vector<TPAIR>  pair;
		std::vector<TPAIR>  plus;
		std::vector<TPAIR>  minus;
		std::vector< std::pair<TPAIR,TPAIR> > ds_pair;
		std::vector<TPAIR> ds_combine;

		PAIR_EVENT(){
			num_plus = 0;
			num_minus= 0;
			num_pair = 0;
		}

		~PAIR_EVENT(){
			plus.clear();
			minus.clear();
			pair.clear();
		}


		void getPairNum(){
			return(num_pair);
		}

		void getPlusNum(){
			return(num_plus);
		}

		void getMinusNum(){
			return(num_minus);
		}

		void getDSPairNum(){
			return(num_ds_pair);
		}

		void getDSCombineNum(){
			return(num_ds_combine);
		}

		void setPlusNum(){
			num_plus=plus.size();
		}

		void setMinusNum(){
			num_minus=minus.size();
		}

		void setPairNum(){
			num_pair=pair.size();
		}

		void setDSPairNum(){
			num_ds_pair=ds_pair.size();
		}

		void setDSCombine_Num(){
			num_ds_combine=ds_combine.size();
		}
		void setNum(){
			setPlusNum();
			setMinusNum();
			setPairNum();
		}

		void getDifferentSignPair(){
			if(getPlusNum()<=0 || getMinusNum()<=0){
				return;
			}
			ds_pair.clean();

			for(int i=0;i<getPlusNum;i++){
				for(int j=0;j<getMinusNum();j++){
					std::pair<TPAIR,TPAIR> ds_pair_aux(plus[i],minus[j]);
					ds_pair.push_back(ds_pair_aux);
				}
			}
			setDSPairNum();
		}

		void getDSCombine(){
			if(getDSPairNum()<=0){
				return;
			}

			for(int i=0;i<getDSPairNum();i++){
				ds_combine.push_back(ds_pair[i].first+ds_pair[i].second);
			}
		}



		void getPlusVec(std::vector<TPAIR> input){
			plus = plus + input;
			pair.clear();
			pair = plus + minus;
			setNum();
		}

		void getMinusVec(std::vector<TPAIR> input){
			minus = minus + input;
			pair.clear();
			pair = plus + minus;
			setNum();
		}
};

template<typename TLEPTON>
class CLEPTON{
	public:
		TLEPTON elec_pair;
		TLEPTON muon_pair;
		TLEPTON lep_pair;
		CLEPTON_CUT          elec_cut;
		CLEPTON_CUT          muon_cut;
		CLEPTON_CUT          lep_cut;

		CLEPTON(){
		}

		std::vector<TLEPTON> checkParticleType(std::vector<TLEPTON> input,int PDG) {
			int num = input.size();
			std::vector<TLEPTON> output;
			for( int i = 0; i < num; i++ ){
				if(input[i]->getType()==PDG){
					output.push_back(input[i]);
				}
			}
			return(output);
		}



		void getPair(std::vector<TLEPTON> in_events,int PID,PAIR_EVENT<TLEPTON> &pair){
			std::vector<TLEPTON> input_plus;
			std::vector<TLEPTON> input_minus;
			input_plus  = checkParticleType(in_events,PID);
			pair.getPlusVec(input_plus);

			input_minus = checkParticleType(in_events,-1*PID);
			pair.getMinusVec(input_minus);
			return;
		}


		void getElecPair(std::vector<TLEPTON> in_events){
			getPair(in_events,11,elec_pair);
		}

		void getMuonPair(std::vector<TLEPTON> in_events){
			getPair(in_events,13,muon_pair);
		}

		void getLepPair(std::vector<TLEPTON> in_events){
			getElecPair(in_events);
			getMuonPair(in_events);
			lep_pair = elec_pair + muon_pair;
		}

		bool getElecCut(){
			int num = elec_pair.getPairNum();
			if(num==0||num==1){
				return(false);
			}
		}

		bool getMuonCut(){
			int num = muon_pair.getPairNum();
			if(num==0||num==1){
				return(false);
			}
		}

		bool getLepCut(){
			int num = lep_pair.getPairNum();
			if(num==0||num==1){
				return(false);
			}
		}
};



#endif
