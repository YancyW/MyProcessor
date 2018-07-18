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

void MyProcessor::checkRCIsoLeptons(std::vector<ReconstructedParticle*> AllPOs, std::vector<ReconstructedParticle*> &IsoLep, std::vector<ReconstructedParticle*> &PFOs_WO_IsoLep, Infomation &info )
{
	int npfo = AllPOs.size();
	for( int i = 0; i < npfo; i++ )
	{
		if ( IsIsoLep( AllPOs[i],info)) 
		{
			IsoLep.push_back(AllPOs[i]);
		}
		else 
		{
			PFOs_WO_IsoLep.push_back(AllPOs[i]);
		}
	}
	return;
}



bool MyProcessor::IsIsoLep( ReconstructedParticle* pfo , Infomation &info ) {
	debug.Message(2,42,"in analyseRCParticle:IsIsoLep pfo type",pfo->getType());
	if ( !IsCharged(pfo) )
	{
		debug.Message(2,42,"in analyseRCParticle:IsIsoLep  not pass IsCharged");
		return false;
	}

	int _islep = IsLepton(pfo,info);

	if ( _islep ==0 )
	{
		debug.Message(2,42,"in analyseRCParticle:IsIsoLep  not pass IsLepton");
		return false;
	}
	else if ( _islep ==11 ){
		if ( !PassesElectronImpactParameterCuts(pfo,info) )
		{
			debug.Message(2,42,"in analyseRCParticle:IsIsoLep  not pass PassesImpactParameterCuts");
			return false;
		}

		if ( !PassesElectronImpactParameterSignificanceCuts(pfo,info) )
		{
			debug.Message(2,42,"in analyseRCParticle:IsIsoLep  not pass PassesImpactParameterSignificanceCuts");
			return false;
		}
	}
	else if ( _islep  ==13 ){
		if ( !PassesMuonImpactParameterCuts(pfo,info) )
		{
			debug.Message(2,42,"in analyseRCParticle:IsIsoLep  not pass PassesImpactParameterCuts");
			return false;
		}

		if ( !PassesMuonImpactParameterSignificanceCuts(pfo,info) )
		{
			debug.Message(2,42,"in analyseRCParticle:IsIsoLep  not pass PassesImpactParameterSignificanceCuts");
			return false;
		}
	}

	return true;

}



bool MyProcessor::IsCharged( ReconstructedParticle* pfo ) {
	if ( pfo->getCharge() == 0 ) return false;
	return true;
}



int MyProcessor::IsLepton( ReconstructedParticle* pfo, Infomation &info ) {

	float CalE[3];
	getCalEnergy( pfo , CalE );
	float ecale  = CalE[0];
	float hcale  = CalE[1];
	float p      = TVector3( pfo->getMomentum() ).Mag();
	float E     = pfo->getEnergy() ;
	float calByP = p>0 ? (ecale + hcale)/p : 0;
	float calSum = ecale+hcale;
	float ecalFrac = calSum>0 ? ecale / calSum : 0;
	float Eyoke  = CalE[2]; 


	bool elec_kmin_calByP  = false;
	bool elec_kmax_calByP  = false;
	bool elec_kmin_ecalFrac= false;
	bool elec_kmax_ecalFrac= false;
	bool elec_kmin_E       = false;
	bool elec_kmax_E       = false;



	bool muon_kmin_calByP  = false;
	bool muon_kmax_calByP  = false;
	bool muon_kmin_ecalFrac= false;
	bool muon_kmax_ecalFrac= false;
	bool muon_kmin_E       = false;
	bool muon_kmax_E       = false;
	bool muon_kmin_Eyoke   = false;
	bool muon_kmax_Eyoke   = false;


	
	
	// electron
	if (       calByP   > info.cut.electronMinEnergyDepositByMomentum){
		elec_kmin_calByP = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass electronMinEnergyDepositByMomentum",calByP);
	}
	if (       calByP   < info.cut.electronMaxEnergyDepositByMomentum){
		elec_kmax_calByP = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass electronMaxEnergyDepositByMomentum",calByP);
	}
	if (       ecalFrac > info.cut.electronMinEcalToHcalFraction ){
		elec_kmin_ecalFrac= true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass electronMinEcalToHcalFraction",ecalFrac);
	}
	if (       ecalFrac < info.cut.electronMaxEcalToHcalFraction ){
		elec_kmax_ecalFrac= true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass electronMaxEcalToHcalFraction",ecalFrac);
	}
	if (       E        > info.cut.electronMinTrackEnergy ){
		elec_kmin_E       = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass electronMinTrackEnergy",E);
	}
	if (       E        < info.cut.electronMaxTrackEnergy ){
		elec_kmax_E       = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass electronMaxTrackEnergy",E);
	}


	
	if (       calByP   > info.cut.muonMinEnergyDepositByMomentum){
		muon_kmin_calByP = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMinEnergyDepositByMomentum",calByP);
	}
	if (       calByP   < info.cut.muonMaxEnergyDepositByMomentum){
		muon_kmax_calByP = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMaxEnergyDepositByMomentum",calByP);
	}
	if (       ecalFrac > info.cut.muonMinEcalToHcalFraction ){
		muon_kmin_ecalFrac= true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMinEcalToHcalFraction",ecalFrac);
	}
	if (       ecalFrac < info.cut.muonMaxEcalToHcalFraction ){
		muon_kmax_ecalFrac= true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMaxEcalToHcalFraction",ecalFrac);
	}
	if (       E        > info.cut.muonMinTrackEnergy ){
		muon_kmin_E       = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMinTrackEnergy",E);
	}
	if (       E        < info.cut.muonMaxTrackEnergy ){
		muon_kmax_E       = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMaxTrackEnergy",E);
	}
	if (       Eyoke    > info.cut.muonMinEyoke){
		muon_kmin_Eyoke   = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMinEyoke",Eyoke);
	}
	if (       Eyoke    < info.cut.muonMaxEyoke ){
		muon_kmax_Eyoke   = true;
	}
	else{
		debug.Message(2,42,"in analyseRCParticle::IsIsoLep  not pass muonMaxEyoke",Eyoke);
	}


	if( elec_kmin_calByP &&  elec_kmax_calByP  &&  elec_kmin_ecalFrac &&  elec_kmax_ecalFrac &&  elec_kmin_E  &&  elec_kmax_E)
////if (       calByP   >= info.cut.electronMinEnergyDepositByMomentum 
////    	&& calByP   <= info.cut.electronMaxEnergyDepositByMomentum
////		&& ecalFrac >= info.cut.electronMinEcalToHcalFraction
////		&& ecalFrac <= info.cut.electronMaxEcalToHcalFraction 
////		&& E        >= info.cut.electronMinTrackEnergy 
////		&& E        <= info.cut.electronMaxTrackEnergy) 
			{
				return 11;
			}

	// muon
////if (       calByP   >= info.cut.muonMinEnergyDepositByMomentum
////		&& calByP   <= info.cut.muonMaxEnergyDepositByMomentum
////		&& ecalFrac >= info.cut.muonMinEcalToHcalFraction
////		&& ecalFrac <= info.cut.muonMaxEcalToHcalFraction 
////		&& Eyoke    >= info.cut.muonMinEyoke
////		&& Eyoke    <= info.cut.muonMaxEyoke 
////		&& E        >= info.cut.muonMinTrackEnergy 
////		&& E        <= info.cut.muonMaxTrackEnergy) 
	if( muon_kmin_calByP  && muon_kmax_calByP   && muon_kmin_ecalFrac  && muon_kmax_ecalFrac  && muon_kmin_E && muon_kmax_E  && muon_kmin_Eyoke    && muon_kmax_Eyoke  ) 
	{
		return 13;
	}


	return 0;
}


bool MyProcessor::PassesElectronImpactParameterCuts( ReconstructedParticle* pfo , Infomation &info ) {
	const EVENT::TrackVec & trkvec = pfo->getTracks();

	if (trkvec.size()==0) return false;

	// TODO: more sophisticated pfo/track matching
	float d0 = fabs(trkvec[0]->getD0());
	float z0 = fabs(trkvec[0]->getZ0());
	float r0 = sqrt( d0*d0 + z0*z0 );

	if ( d0 < info.cut.electronMinD0 ) return false;
	if ( d0 > info.cut.electronMaxD0 ) return false;
	if ( z0 < info.cut.electronMinZ0 ) return false;
	if ( z0 > info.cut.electronMaxZ0 ) return false;
	if ( r0 < info.cut.electronMinR0 ) return false;
	if ( r0 > info.cut.electronMaxR0 ) return false;

	return true;
}

bool MyProcessor::PassesElectronImpactParameterSignificanceCuts( ReconstructedParticle* pfo , Infomation &info ) {
	const EVENT::TrackVec & trkvec = pfo->getTracks();

	if (trkvec.size()==0) return false;

	float d0 = fabs(trkvec[0]->getD0());
	float z0 = fabs(trkvec[0]->getZ0());
	float d0err = sqrt(trkvec[0]->getCovMatrix()[0]);
	float z0err = sqrt(trkvec[0]->getCovMatrix()[9]);

	float d0sig = d0err != 0 ? d0/d0err : 0;
	float z0sig = z0err != 0 ? z0/z0err : 0;
	float r0sig = sqrt( d0sig*d0sig + z0sig*z0sig );

	if ( d0sig < info.cut.electronMinD0Sig ) return false;
	if ( d0sig > info.cut.electronMaxD0Sig ) return false;
	if ( z0sig < info.cut.electronMinZ0Sig ) return false;
	if ( z0sig > info.cut.electronMaxZ0Sig ) return false;
	if ( r0sig < info.cut.electronMinR0Sig ) return false;
	if ( r0sig > info.cut.electronMaxR0Sig ) return false;

	return true;
}

bool MyProcessor::PassesMuonImpactParameterCuts( ReconstructedParticle* pfo , Infomation &info ) {
	const EVENT::TrackVec & trkvec = pfo->getTracks();

	if (trkvec.size()==0) return false;

	// TODO: more sophisticated pfo/track matching
	float d0 = fabs(trkvec[0]->getD0());
	float z0 = fabs(trkvec[0]->getZ0());
	float r0 = sqrt( d0*d0 + z0*z0 );

	if ( d0 < info.cut.muonMinD0 ) return false;
	if ( d0 > info.cut.muonMaxD0 ) return false;
	if ( z0 < info.cut.muonMinZ0 ) return false;
	if ( z0 > info.cut.muonMaxZ0 ) return false;
	if ( r0 < info.cut.muonMinR0 ) return false;
	if ( r0 > info.cut.muonMaxR0 ) return false;

	return true;
}

bool MyProcessor::PassesMuonImpactParameterSignificanceCuts( ReconstructedParticle* pfo, Infomation &info ) {
	const EVENT::TrackVec & trkvec = pfo->getTracks();

	if (trkvec.size()==0) return false;

	float d0 = fabs(trkvec[0]->getD0());
	float z0 = fabs(trkvec[0]->getZ0());
	float d0err = sqrt(trkvec[0]->getCovMatrix()[0]);
	float z0err = sqrt(trkvec[0]->getCovMatrix()[9]);

	float d0sig = d0err != 0 ? d0/d0err : 0;
	float z0sig = z0err != 0 ? z0/z0err : 0;
	float r0sig = sqrt( d0sig*d0sig + z0sig*z0sig );

	if ( d0sig < info.cut.muonMinD0Sig ) return false;
	if ( d0sig > info.cut.muonMaxD0Sig ) return false;
	if ( z0sig < info.cut.muonMinZ0Sig ) return false;
	if ( z0sig > info.cut.muonMaxZ0Sig ) return false;
	if ( r0sig < info.cut.muonMinR0Sig ) return false;
	if ( r0sig > info.cut.muonMaxR0Sig ) return false;

	return true;
}

void MyProcessor::getCalEnergy( ReconstructedParticle* pfo , float* cale) {
	float ecal = 0;
	float hcal = 0;
	float yokeEnergy = 0;
	std::vector<lcio::Cluster*> clusters = pfo->getClusters();
	for ( std::vector<lcio::Cluster*>::const_iterator iCluster=clusters.begin();
			iCluster!=clusters.end();
			++iCluster) {
		ecal       += (*iCluster)->getSubdetectorEnergies()[0];
		hcal       += (*iCluster)->getSubdetectorEnergies()[1];
		yokeEnergy += (*iCluster)->getSubdetectorEnergies()[2];
		ecal       += (*iCluster)->getSubdetectorEnergies()[3];
		hcal       += (*iCluster)->getSubdetectorEnergies()[4];
	}
	cale[0] = ecal;
	cale[1] = hcal;
	cale[2] = yokeEnergy;
}

bool MyProcessor::checkRCIsoLepCone(std::vector<ReconstructedParticle*> IsoLep, std::vector<ReconstructedParticle*> PFOs_WO_IsoLep, std::vector<ReconstructedParticle*> &NewIsoLep, std::vector<ReconstructedParticle*> &NewPFOs_WO_IsoLep, Infomation &info)
{
	float Mass;
	int nlep = IsoLep.size();
	NewPFOs_WO_IsoLep = PFOs_WO_IsoLep;
	int npfo = NewPFOs_WO_IsoLep.size();
	if(nlep<1) {
		return false;
	}
	for( int i = 0; i < nlep; i++ )
	{
		ReconstructedParticleImpl* NewLep = new ReconstructedParticleImpl; 
		float charge  = IsoLep[i]->getCharge();
		float pid     = IsoLep[i]->getType();
		float Pnew[3] = {IsoLep[i]->getMomentum()[0],IsoLep[i]->getMomentum()[1],IsoLep[i]->getMomentum()[2]} ;
		float Enew    = IsoLep[i]->getEnergy() ;
		std::vector<ReconstructedParticle*>::iterator it = NewPFOs_WO_IsoLep.begin();
		while (it!=NewPFOs_WO_IsoLep.end())
		{
			if (IsInLepCone( IsoLep[i], *it,info)) 
			{
				float Ppfo[3] = {(*it)->getMomentum()[0],(*it)->getMomentum()[1],(*it)->getMomentum()[2]};
				float Epfo = (*it)->getEnergy();
				Pnew[0] = Pnew[0] + Ppfo[0];
				Pnew[1] = Pnew[1] + Ppfo[1];
				Pnew[2] = Pnew[2] + Ppfo[2];
				Enew    = Enew    + Epfo   ;
				NewPFOs_WO_IsoLep.erase(it);
			}
			else{
				it++;
			}
		}
		float Mass2=Enew*Enew-Pnew[0]*Pnew[0]-Pnew[1]*Pnew[1]-Pnew[2]*Pnew[2];
		if(Mass2>0){
			Mass=sqrt(Mass2);
		}
		NewLep->setMomentum(Pnew);
		NewLep->setEnergy  (Enew);
		NewLep->setMass    (Mass);
		NewLep->setType    (pid);
		NewLep->setCharge  (charge);
		NewIsoLep.push_back(dynamic_cast<ReconstructedParticle*> (NewLep));
	}
	return true;
}

bool MyProcessor::IsInLepCone( ReconstructedParticle* lep, ReconstructedParticle* pfo , Infomation &info) {
	float coneE = 0;
	if(pfo->getType()!=22){
		return(false);
	}

	TVector3 P_pfo( pfo->getMomentum() );

	TVector3 P_lep( lep->getMomentum() );

	float cosTheta = P_lep.Dot( P_pfo )/(P_lep.Mag()*P_pfo.Mag());
	if ( cosTheta >= info.cut.cosConeAngle )
	{
		return true;
	}
	return false;
}
