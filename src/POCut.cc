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

bool MyProcessor::POCutMuon( std::vector<ReconstructedParticle*> &muon,std::vector<ReconstructedParticle*> &muon_plus,std::vector<ReconstructedParticle*> &muon_minus ,Variable& data) {
	if(muon.size()<2 || muon_plus.size()<1 || muon_minus.size()<1) {
		return(false);
	}
////for( int i = 0; i < num; i++ ){
////	ReconstructedParticle * PO = POs[i];
////	TLorentzVector Tpo=TLorentzVector(PO->getMomentum(),PO->getEnergy());
////	float pt=TMath::Sqrt(Tpo[0]*Tpo[0]+Tpo[1]*Tpo[1])<5.0;
////	if(pt<5.0){
////		return(false);
////	}
////	const Track* trk=PO->getTracks()[0];
////	if(std::abs(trk->getD0()/(TMath::Sqrt(trk->getCovMatrix()[0])))<5.0){
////		return(false);
////	}
////	if(std::abs(trk->getZ0()/(TMath::Sqrt(trk->getCovMatrix()[9])))<5.0){
////		return(false);
////	}

////	const EVENT::ClusterVec & clusvec = PO->getClusters();
////	float mu_ecal;
////	float mu_hcal;
////	float mu_yoke;
////	float mu_lcal;
////	float mu_lhcal;
////	float mu_bcal;
////	float mu_ecalfrac;
////	float mu_EbyP;
////	if( clusvec.size() > 0 ){//clsuter parameters of mu+
////		for( ClusterVec::const_iterator iClus = clusvec.begin();
////				iClus != clusvec.end();
////				++iClus ){
////			mu_ecal  += ( *iClus )->getSubdetectorEnergies()[0];
////			mu_hcal  += ( *iClus )->getSubdetectorEnergies()[1];
////			mu_yoke  += ( *iClus )->getSubdetectorEnergies()[2];
////			mu_lcal  += ( *iClus )->getSubdetectorEnergies()[3];
////			mu_lhcal += ( *iClus )->getSubdetectorEnergies()[4];
////			mu_bcal  += ( *iClus )->getSubdetectorEnergies()[5];
////			mu_ecalfrac = mu_ecal / (mu_ecal + mu_hcal );
////			mu_EbyP     = ( mu_ecal + mu_hcal ) / ( Tpo.Vect().Mag() );
////		}
////	}
////	if(mu_ecal/pt<0.3){
////		return(false);
////	}
////	if(mu_yoke<1.2){
////		return(false);
////	}
////}
	return(true);
}


bool MyProcessor::POCutElec( std::vector<ReconstructedParticle*> POs) {
	int num = POs.size();
	if(num==0||num==1) {
		return(false);
	}
////for( int i = 0; i < num; i++ ){
////	ReconstructedParticle * PO = POs[i];
////	TLorentzVector Tpo=TLorentzVector(PO->getMomentum(),PO->getEnergy());
////	float pt=TMath::Sqrt(Tpo[0]*Tpo[0]+Tpo[1]*Tpo[1])<5.0;
////	if(pt<5.0){
////		return(false);
////	}
////	const Track* trk=PO->getTracks()[0];
////	if(std::abs(trk->getD0()/(TMath::Sqrt(trk->getCovMatrix()[0])))<5.0){
////		return(false);
////	}
////	if(std::abs(trk->getZ0()/(TMath::Sqrt(trk->getCovMatrix()[9])))<5.0){
////		return(false);
////	}

////	const EVENT::ClusterVec & clusvec = PO->getClusters();
////	float mu_ecal;
////	float mu_hcal;
////	float mu_yoke;
////	float mu_lcal;
////	float mu_lhcal;
////	float mu_bcal;
////	float mu_ecalfrac;
////	float mu_EbyP;
////	if( clusvec.size() > 0 ){//clsuter parameters of mu+
////		for( ClusterVec::const_iterator iClus = clusvec.begin();
////				iClus != clusvec.end();
////				++iClus ){
////			mu_ecal  += ( *iClus )->getSubdetectorEnergies()[0];
////			mu_hcal  += ( *iClus )->getSubdetectorEnergies()[1];
////			mu_yoke  += ( *iClus )->getSubdetectorEnergies()[2];
////			mu_lcal  += ( *iClus )->getSubdetectorEnergies()[3];
////			mu_lhcal += ( *iClus )->getSubdetectorEnergies()[4];
////			mu_bcal  += ( *iClus )->getSubdetectorEnergies()[5];
////			mu_ecalfrac = mu_ecal / (mu_ecal + mu_hcal );
////			mu_EbyP     = ( mu_ecal + mu_hcal ) / ( Tpo.Vect().Mag() );
////		}
////	}
////	if(mu_ecal/pt<0.3){
////		return(false);
////	}
////	if(mu_yoke<1.2){
////		return(false);
////	}
////}
	return(true);
}
