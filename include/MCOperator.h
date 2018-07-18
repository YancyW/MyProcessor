#ifndef MCOPERATOR_H
#define MCOPERATOR_H  
#include <iostream>
#include <fstream>
#include <set>

#include <vector>
#include <stack>

//LCIO
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Vertex.h>
#include <IMPL/MCParticleImpl.h>
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
#include "libformat.h"

using namespace lcio;
using namespace marlin;

MCParticle* NewParticle(float px, float py, float pz,float E,MCParticle* test);
ReconstructedParticle* NewParticle(float px, float py, float pz,float E,ReconstructedParticle* test);

MCParticle* Add(const MCParticle* MC1,const MCParticle* MC2);
MCParticle* Minus(const MCParticle* MC1,const MCParticle* MC2);
MCParticle* Equal(const MCParticle* MC1) ;
ReconstructedParticle* Add(const ReconstructedParticle* MC1,const ReconstructedParticle* MC2);
ReconstructedParticle* Minus(const ReconstructedParticle* MC1,const ReconstructedParticle* MC2);
ReconstructedParticle* Equal(const ReconstructedParticle* MC1) ;

MCParticle& operator+(const MCParticle& MC1,const MCParticle& MC2);
ReconstructedParticle& operator+(const ReconstructedParticle& MC1,const ReconstructedParticle& MC2);
ReconstructedParticle& operator-(const ReconstructedParticle& MC1,const ReconstructedParticle& MC2);

float calMass  (MCParticle* MC);
float calEnergy(MCParticle* MC);
float calMag   (MCParticle* MC);
TVector3 calVect(MCParticle* MC);

template<typename TPT>
float calPT(TPT MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum() ,MC->getEnergy());
	return(vec.Perp());
}

template<typename TPT>
float calPT    (TPT MC    , TPT Ref    ){
	TLorentzVector vec =TLorentzVector(MC->getMomentum() ,MC->getEnergy());
	TLorentzVector ref =TLorentzVector(Ref->getMomentum(),Ref->getEnergy());
	return(vec.Perp(ref.Vect()));
}

float calMass  (MCParticleImpl* MC);
float calEnergy(MCParticleImpl* MC);
float calMag   (MCParticleImpl* MC);
TVector3 calVect(MCParticleImpl* MC);




template<typename TTheta>
float calCosTheta(TTheta  MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.CosTheta());
}

template<typename TTheta>
float calCosTheta(TTheta  MC, TTheta  Ref){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	TLorentzVector ref =TLorentzVector(Ref->getMomentum(),Ref->getEnergy());
	float angle=vec.Angle(ref.Vect());
	return(TMath::Cos(angle));
}

template<typename TPhi>
float calAzimuth(TPhi MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.Phi());
}


template<typename TPhi>
float calAzimuth(TPhi MC, TPhi Ref){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	TLorentzVector ref =TLorentzVector(Ref->getMomentum(),Ref->getEnergy());
	return(vec.DeltaPhi(ref));
}

float calEnergy(ReconstructedParticle* RC);
#endif
