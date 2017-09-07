#include "Print.h"

std::ostream & operator << (std::ostream & ostr, const LCEvent *in){
	LCCollection* BCALClusters=in->getCollection("BCALClusters");
	LCCollection* BCALParticles=in->getCollection("BCALParticles");
	LCCollection* BuildUpVertex=in->getCollection("BuildUpVertex");
	LCCollection* BuildUpVertex_RP=in->getCollection("BuildUpVertex_RP");
	LCCollection* BuildUpVertex_V0=in->getCollection("BuildUpVertex_V0");
	LCCollection* BuildUpVertex_V0_RP=in->getCollection("BuildUpVertex_V0_RP");
	LCCollection* MCParticlesSkimmed=in->getCollection("MCParticlesSkimmed");
	LCCollection* MCTruthMarlinTrkTracksLink=in->getCollection("MCTruthMarlinTrkTracksLink");
	LCCollection* PandoraClusters=in->getCollection("PandoraClusters");
	LCCollection* PandoraPFANewReclusterMonitoring=in->getCollection("PandoraPFANewReclusterMonitoring");
	LCCollection* PandoraPFOs=in->getCollection("PandoraPFOs");
	LCCollection* PrimaryVertex=in->getCollection("PrimaryVertex");
	LCCollection* PrimaryVertex_RP=in->getCollection("PrimaryVertex_RP");
	LCCollection* RecoMCTruthLink=in->getCollection("RecoMCTruthLink");
	LCCollection* V0RecoParticles=in->getCollection("V0RecoParticles");
	LCCollection* V0Vertices=in->getCollection("V0Vertices");
	LCCollection* Isoleps=in->getCollection("Isoleps");
	LCCollection* PFOsWithoutIsoleps=in->getCollection("PFOsWithoutIsoleps");
	printf("\n");
	printf("%30s %20i \n","BCALClusters number :                    " , BCALClusters->getNumberOfElements()                     ); 
	printf("%30s %20i \n","BCALParticles number :                   " , BCALParticles->getNumberOfElements()                    ); 
	printf("%30s %20i \n","BuildUpVertex number :                   " , BuildUpVertex->getNumberOfElements()                    ); 
	printf("%30s %20i \n","BuildUpVertex_RP number :                " , BuildUpVertex_RP ->getNumberOfElements()                );
	printf("%30s %20i \n","BuildUpVertex_V0 number :                " , BuildUpVertex_V0->getNumberOfElements()                 ); 
	printf("%30s %20i \n","BuildUpVertex_V0_RP number :             " , BuildUpVertex_V0_RP ->getNumberOfElements()             ); 
	printf("%30s %20i \n","MCParticlesSkimmed number :              " , MCParticlesSkimmed->getNumberOfElements()               ); 
	printf("%30s %20i \n","MCTruthMarlinTrkTracksLink number :      " , MCTruthMarlinTrkTracksLink ->getNumberOfElements()      ); 
	printf("%30s %20i \n","PandoraClusters number :                 " , PandoraClusters->getNumberOfElements()                  ); 
	printf("%30s %20i \n","PandoraPFANewReclusterMonitoring number :" , PandoraPFANewReclusterMonitoring ->getNumberOfElements()); 
	printf("%30s %20i \n","PandoraPFOs number :                     " , PandoraPFOs->getNumberOfElements()                      ); 
	printf("%30s %20i \n","PrimaryVertex number :                   " , PrimaryVertex->getNumberOfElements()                    ); 
	printf("%30s %20i \n","PrimaryVertex_RP number :                " , PrimaryVertex_RP->getNumberOfElements()                 ); 
	printf("%30s %20i \n","RecoMCTruthLink number :                 " , RecoMCTruthLink->getNumberOfElements()                  ); 
	printf("%30s %20i \n","V0RecoParticles number :                 " , V0RecoParticles->getNumberOfElements()                  ); 
	printf("%30s %20i \n","V0Vertices number :                      " , V0Vertices->getNumberOfElements()                       ); 
	printf("%30s %20i \n","Isoleps number :                         " , Isoleps->getNumberOfElements()                       ); 
	printf("%30s %20i \n","PFOsWithoutIsoleps number :              " , PFOsWithoutIsoleps->getNumberOfElements()                       ); 
	return ostr;
}

std::ostream & operator << (std::ostream & ostr, const MCParticle *in){
	int parent;
	int daughter;
	if(in->getParents().size()==0){
		parent = 0;
	}
	else{
		parent = 1;
	}
	if(in->getDaughters().size()==0){
		daughter= 0;
	}
	else{
		daughter= 1;
	}
	printf("\n");
	printf("%5s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s \n",      "PDG","genStatus", "energy","px","py","pz", "mass","charge","spin", "parents","daughter");
	printf("%5i %10i %10f %10f %10f %10f %10f %10f %10f %10i %10i \n",  in->getPDG(),in->getGeneratorStatus(),in->getEnergy(),in->getMomentum()[0],in->getMomentum()[1],in->getMomentum()[2],in->getMass(),in->getCharge(),in->getSpin()[0],parent, daughter);
	return ostr;
}


std::ostream & operator << (std::ostream & ostr, const ReconstructedParticle *in){
	printf("\n");
	printf("%5s %10s %10s %10s %10s %10s %10s \n",      "PDG", "energy","px","py","pz", "mass","charge");
	printf("%5i %10f %10f %10f %10f %10f %10f \n",  in->getType(),in->getEnergy(),in->getMomentum()[0],in->getMomentum()[1],in->getMomentum()[2],in->getMass(),in->getCharge());
	return ostr;
}


std::ostream & operator << (std::ostream & ostr, const Track *in){
	printf("\n");
	printf("%5s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s \n",      "PDG", "d0","phi","omega","z0", "TanLambda","reference point","dEdx","dEdxErr","chi2","ndf");
	printf("%5i %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f \n",  in->getType(),in->getD0(),in->getPhi(),in->getOmega(),in->getZ0(),in->getReferencePoint(),in->getdEdx(),in->getdEdxError(),in->getChi2(),in->getNdf());
	return ostr;
}




std::ostream & operator << (std::ostream & ostr, const std::vector<MCParticle*> &invec){
	if (invec.size() == 0) return ostr;
	int parent;
	int daughter;
	printf("\n");
	for (unsigned int i = 0; i < invec.size(); i++) {
		MCParticle* in=invec[i];
		if(in->getParents().size()==0){
			parent = 0;
		}
		else{
			parent = 1;
		}
		if(in->getDaughters().size()==0){
			daughter= 0;
		}
		else{
			daughter= 1;
		}
		printf("%5s %5s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s \n",  "#",    "PDG","genStatus", "energy","px","py","pz", "mass","charge","spin", "parents","daughter");
		printf("%5i %5i %10i %10f %10f %10f %10f %10f %10f %10f %10i %10i \n",    i,   in->getPDG(),in->getGeneratorStatus(),in->getEnergy(),in->getMomentum()[0],in->getMomentum()[1],in->getMomentum()[2],in->getMass(),in->getCharge(),in->getSpin()[0],parent, daughter);
	}
	return ostr;
}


std::ostream & operator << (std::ostream & ostr, const std::vector<ReconstructedParticle*> &invec){
	if (invec.size() == 0) return ostr;
	printf("\n");
	for (unsigned int i = 0; i < invec.size(); i++) {
		ReconstructedParticle* in=invec[i];
		printf("%5s %5s %10s %10s %10s %10s %10s %10s \n",    "#",    "PDG", "energy","px","py","pz", "mass","charge");
		printf("%5i %5i %10f %10f %10f %10f %10f %10f \n",  i, in->getType(),in->getEnergy(),in->getMomentum()[0],in->getMomentum()[1],in->getMomentum()[2],in->getMass(),in->getCharge());
	}
	return ostr;
}


std::ostream & operator << (std::ostream & ostr, const std::vector<Track*> &invec){
	if (invec.size() == 0) return ostr;
	printf("\n");
	for (unsigned int i = 0; i < invec.size(); i++) {
		Track* in=invec[i];
		printf("%5s %5s %10s %10s %10s %10s %10s %10s %10s %10s %10s %10s \n",  "#",    "PDG", "d0","phi","omega","z0", "TanLambda","reference point","dEdx","dEdxErr","chi2","ndf");
		printf("%5i %5i %10f %10f %10f %10f %10f %10f %10f %10f %10f %10f \n",  i, in->getType(),in->getD0(),in->getPhi(),in->getOmega(),in->getZ0(),in->getReferencePoint(),in->getdEdx(),in->getdEdxError(),in->getChi2(),in->getNdf());
	}
	return ostr;
}


std::ostream & operator << (std::ostream & ostr, const TLorentzVector &invec){
	printf("\n");
	printf("%10s %10s %10s %10s \n",    "px", "py","pz","e");
	printf("%10f %10f %10f %10f \n",  invec[0],invec[1],invec[2],invec[3]);
	return ostr;
}


std::ostream & operator << (std::ostream & ostr, const std::vector<TLorentzVector> &invec){
	if (invec.size() == 0) return ostr;
	printf("\n");
	for (unsigned int i = 0; i < invec.size(); i++) {
		TLorentzVector in=invec[i];
		printf("%5s %10s %10s %10s %10s \n", "#",   "px", "py","pz","e");
		printf("%5i %10f %10f %10f %10f \n", i, in[0],in[1],in[2],in[3]);
	}
	return ostr;
}
