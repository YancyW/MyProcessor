#include <MCOperator.h>



MCParticle* NewParticle(float px, float py, float pz,float E,MCParticle* test){
	MCParticleImpl* tmp= new MCParticleImpl;
    float Pnew[3] = {px,py,pz};
    tmp->setMomentum(Pnew);
	float mass2= std::pow(E,2)-std::pow(px,2)-std::pow(py,2)-std::pow(pz,2);
	float mass;
	if(mass2>=0){
		mass= std::sqrt(mass2);
	}
	else{
		mass=-std::sqrt(-mass2);
	}
    tmp->setMass(mass);
	MCParticle* new_particle=dynamic_cast<MCParticle*>(tmp);
	return(new_particle);
}

MCParticle&  operator +(const MCParticle &MC1, const MCParticle & MC2) {
	MCParticleImpl* MC = new MCParticleImpl ;
	TVector3  p1=MC1.getMomentum();
	TVector3  p2=MC2.getMomentum();
	TVector3  p0=p1+p2;
	float energy=(MC1.getEnergy()+MC2.getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float mass;
	if(mass2>=0){
		mass= std::sqrt(mass2);
	}
	else{
		mass=-std::sqrt(-mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	MC->setMomentum(fl0);
	MC->setMass    (mass);
	MC->setCharge  (MC1.getCharge()+MC2.getCharge());
	MC->setPDG     (10000); 
	MCParticle* MC0 = dynamic_cast<MCParticle*> (MC);
	return(*MC);
}


MCParticle*  Equal(const MCParticle* MC1) {
	MCParticleImpl* MC = new MCParticleImpl ;
	TVector3  p1=MC1->getMomentum();
	float energy=(MC1->getEnergy());
	float mass2= std::pow(energy,2)-p1.Mag2();
	float fl0[3]={p1.Px(),p1.Py(),p1.Pz()};
	MC->setMomentum(fl0);
	MC->setMass    (MC1->getMass());
	MC->setCharge  (MC1->getCharge());
	MC->setPDG     (MC1->getPDG()); 
	MCParticle* MC0 = dynamic_cast<MCParticle*> (MC);
	return(MC0);
}


MCParticle*  Add(const MCParticle* MC1, const MCParticle* MC2) {
	MCParticleImpl* MC = new MCParticleImpl ;
	TVector3  p1=MC1->getMomentum();
	TVector3  p2=MC2->getMomentum();
	TVector3  p0=p1+p2;
	float energy=(MC1->getEnergy()+MC2->getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float     mass;
	if(mass2>=0){
		mass= std::sqrt(mass2);
	}
	else{
		mass=-std::sqrt(-mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	MC->setMomentum(fl0);
	MC->setMass    (mass);
	MC->setCharge  (MC1->getCharge()+MC2->getCharge());
	MC->setPDG     (10000); 
	MCParticle* MC0 = dynamic_cast<MCParticle*> (MC);
	return(MC0);
}


MCParticle*  Minus(const MCParticle* MC1, const MCParticle* MC2) {
	MCParticleImpl* MC = new MCParticleImpl ;
	TVector3  p1=MC1->getMomentum();
	TVector3  p2=MC2->getMomentum();
	TVector3  p0=p1-p2;
	float energy=(MC1->getEnergy()-MC2->getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float     mass;
	if(mass2>=0){
		mass= std::sqrt(mass2);
	}
	else{
		mass=-std::sqrt(-mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	MC->setMomentum(fl0);
	MC->setMass    (mass);
	MC->setCharge  (MC1->getCharge());
	MC->setPDG     (10000); 
	MCParticle* MC0 = dynamic_cast<MCParticle*> (MC);
	return(MC0);
}



ReconstructedParticle*  Add(const ReconstructedParticle* RC1, const ReconstructedParticle* RC2) {
	ReconstructedParticleImpl* RC = new ReconstructedParticleImpl;
	TVector3  p1=RC1->getMomentum();
	TVector3  p2=RC2->getMomentum();
	TVector3  p0=p1+p2;
	float energy=(RC1->getEnergy()+RC2->getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float mass;
	if(mass2<0){
		mass=0;
	}
	else{
		mass= std::sqrt(mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	RC->setMomentum(fl0);
	RC->setMass    (mass);
	RC->setEnergy(energy);
	RC->setCharge  (RC1->getCharge()+RC2->getCharge());
	RC->setType    (10000); 
	ReconstructedParticle* RC0 = dynamic_cast<ReconstructedParticle*> (RC);
	return(RC0);
}



ReconstructedParticle*  Minus(const ReconstructedParticle* RC1, const ReconstructedParticle* RC2) {
	ReconstructedParticleImpl* RC = new ReconstructedParticleImpl;
	TVector3  p1=RC1->getMomentum();
	TVector3  p2=RC2->getMomentum();
	TVector3  p0=p1-p2;
	float energy=(RC1->getEnergy()-RC2->getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float mass;
	if(mass2<0){
		mass=0;
	}
	else{
		mass= std::sqrt(mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	RC->setMomentum(fl0);
	RC->setMass    (mass);
	RC->setEnergy(energy);
	RC->setCharge  (RC1->getCharge());
	RC->setType    (10000); 
	ReconstructedParticle* RC0 = dynamic_cast<ReconstructedParticle*> (RC);
	return(RC0);
}


ReconstructedParticle*  Equal(const ReconstructedParticle* MC1) {
	ReconstructedParticleImpl* MC = new ReconstructedParticleImpl;
	TVector3  p1=MC1->getMomentum();
	float energy=(MC1->getEnergy());
	float mass2= std::pow(energy,2)-p1.Mag2();
	float fl0[3]={p1.Px(),p1.Py(),p1.Pz()};
	MC->setMomentum(fl0);
	MC->setMass    (MC1->getMass());
	MC->setEnergy  (MC1->getEnergy());
	MC->setCharge  (MC1->getCharge());
	MC->setType    (MC1->getType()); 
	ReconstructedParticle* MC0 = dynamic_cast<ReconstructedParticle*> (MC);
	return(MC0);
}


ReconstructedParticle* NewParticle(float px, float py, float pz,float E, ReconstructedParticle* test){
	ReconstructedParticleImpl* tmp= new ReconstructedParticleImpl;
    float Pnew[3] = {px,py,pz};
    tmp->setMomentum(Pnew);
    tmp->setEnergy(E);
	ReconstructedParticle* new_particle=dynamic_cast<ReconstructedParticle*>(tmp);
	return(new_particle);
}

ReconstructedParticle&  operator +(const ReconstructedParticle &RC1, const ReconstructedParticle &RC2) {
	ReconstructedParticleImpl* RC = new ReconstructedParticleImpl;
	TVector3  p1=RC1.getMomentum();
	TVector3  p2=RC2.getMomentum();
	TVector3  p0=p1+p2;
	float energy=(RC1.getEnergy()+RC2.getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float mass;
	if(mass2<0){
		mass=0;
	}
	else{
		mass= std::sqrt(mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	RC->setMomentum(fl0);
	RC->setEnergy(energy);
	RC->setMass    (mass);
	RC->setCharge  (RC1.getCharge()+RC2.getCharge());
	RC->setType    (10000); 
	ReconstructedParticle* RC0 = dynamic_cast<ReconstructedParticle*> (RC);
	return(*RC0);
}

ReconstructedParticle&  operator -(const ReconstructedParticle &RC1, const ReconstructedParticle &RC2) {
	ReconstructedParticleImpl* RC = new ReconstructedParticleImpl;
	TVector3  p1=RC1.getMomentum();
	TVector3  p2=RC2.getMomentum();
	TVector3  p0=p1-p2;
	float energy=(RC1.getEnergy()-RC2.getEnergy());
	float mass2= std::pow(energy,2)-p0.Mag2();
	float mass;
	if(mass2<0){
		mass=0;
	}
	else{
		mass= std::sqrt(mass2);
	}
	float fl0[3]={p0.Px(),p0.Py(),p0.Pz()};
	RC->setMomentum(fl0);
	RC->setEnergy(energy);
	RC->setMass    (mass);
	RC->setCharge  (RC1.getCharge()+RC2.getCharge());
	RC->setType    (10000); 
	ReconstructedParticle* RC0 = dynamic_cast<ReconstructedParticle*> (RC);
	return(*RC0);
}





float calMass(MCParticle* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.M());
}
float calEnergy(MCParticle* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.E());
}
float calMag(MCParticle* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.Mag());
}
TVector3 calVect(MCParticle* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.Vect());
}


float calMass(MCParticleImpl* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.M());
}
float calEnergy(MCParticleImpl* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.E());
}
float calMag(MCParticleImpl* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.Mag());
}
TVector3 calVect(MCParticleImpl* MC){
	TLorentzVector vec =TLorentzVector(MC->getMomentum(),MC->getEnergy());
	return(vec.Vect());
}

float calEnergy(ReconstructedParticle* RC){
	TLorentzVector vec =TLorentzVector(RC->getMomentum(),RC->getEnergy());
	return(vec.E());
}
