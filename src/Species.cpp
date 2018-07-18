#include "MyProcessor.h"


void  MyProcessor::ASpecies::get_mcparticle_species(std::vector<MCParticle*> &allparticle){
	for( int i = 0; i < allparticle.size(); i++ ){
		int    PID    = allparticle[i]->getPDG();
		int    aPID   = std::abs(PID); 
		if(aPID==12 || aPID==14 || aPID==16 ){   	// neutrino
			_mcspecies["inv"].push_back(allparticle[i]);
		}
		else if(PID == 11 ){  	// e-
			_mcspecies["e-"].push_back(allparticle[i]);
		}
		else if( PID == 13 ){  	// mu-
			_mcspecies["m-"].push_back(allparticle[i]);
		}
		else if( PID == 15 ){  	// tau-
			_mcspecies["tau-"].push_back(allparticle[i]);
		}
		else if( PID == -11 ){ 	// e+
			_mcspecies["e+"].push_back(allparticle[i]);
		}
		else if( PID == -13 ){ 	// mu+
			_mcspecies["m+"].push_back(allparticle[i]);
		}
		else if( PID == -15 ){ 	// tau+
			_mcspecies["tau+"].push_back(allparticle[i]);
		}
		else if(abs(PID)==22){ 	// photon
			_mcspecies["photon"].push_back(allparticle[i]);
		}
		else if(aPID==23){  // Z 
			_mcspecies["Z"].push_back(allparticle[i]);
		}
		else if(aPID==24){  // W 
			_mcspecies["W"].push_back(allparticle[i]);
		}
		else if(aPID==25){  // higgs 
			_mcspecies["Higgs"].push_back(allparticle[i]);
		}
		else if((aPID%1000>110&&aPID%1000<120)){  	// higgs 
			_mcspecies["Light I=1 Mesons, pi0"].push_back(allparticle[i]);
		}
		else if(aPID%1000>210&&aPID%1000<220){  	// higgs 
			_mcspecies["Light I=1 Mesons, pi+"].push_back(allparticle[i]);
		}
		else if((aPID%1000>220&&aPID%1000<230)||(aPID%1000>331&&aPID%1000<340)){  	// higgs 
			_mcspecies["Light I=0 Mesons, eta"].push_back(allparticle[i]);
		}
		else if((aPID%1000>310&&aPID%1000<320)){  	// higgs 
			_mcspecies["Strange Mesons, K0   "].push_back(allparticle[i]);
		}
		else if((aPID%1000>320&&aPID%1000<330)){  	// higgs 
			_mcspecies["Strange Mesons, K+   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>410&&aPID%10000<420)){  	// higgs 
			_mcspecies["Charmed Mesons, D0   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>420&&aPID%10000<430)){  	// higgs 
			_mcspecies["Charmed Mesons, D+   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>510&&aPID%10000<520)||(aPID%10000>530&&aPID%10000<540)){  	// higgs 
			_mcspecies["Bottom Mesons, B0   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>520&&aPID%10000<530)||(aPID%10000>540&&aPID%10000<550)){  	// higgs 
			_mcspecies["Bottom Mesons, B+   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>441&&aPID%10000<450)){  	// higgs 
			_mcspecies["ccbar Mesons        "].push_back(allparticle[i]);
		}
		else if((aPID%10000>551&&aPID%10000<560)){  	// higgs 
			_mcspecies["ccbar Mesons        "].push_back(allparticle[i]);
		}
		else if((aPID%10000>1000&&aPID%10000<4000)){  	// higgs 
			_mcspecies["Light Baryons       "].push_back(allparticle[i]);
		}
		else if((aPID%10000>4000&&aPID%10000<5000)){  	// higgs 
			_mcspecies["Charmed Baryons     "].push_back(allparticle[i]);
		}
		else if((aPID%10000>5000&&aPID%10000<6000)){  	// higgs 
			_mcspecies["Bottom Baryons      "].push_back(allparticle[i]);
		}
		else{
			_mcspecies["other hadron"].push_back(allparticle[i]);
		}
	}

	setMCNum();
}


void MyProcessor::ASpecies::get_rcparticle_species(std::vector<ReconstructedParticle*> &allparticle){
	for( int i = 0; i < allparticle.size(); i++ ){
		int    PID    = allparticle[i]->getType();
		int    aPID   = std::abs(PID); 
		if(aPID==12 || aPID==14 || aPID==16 ){   	// neutrino
			_rcspecies["inv"].push_back(allparticle[i]);
		}
		else if(PID == 11 ){  	// e-
			_rcspecies["e-"].push_back(allparticle[i]);
		}
		else if( PID == 13 ){  	// mu-
			_rcspecies["m-"].push_back(allparticle[i]);
		}
		else if( PID == 15 ){  	// tau-
			_rcspecies["tau-"].push_back(allparticle[i]);
		}
		else if( PID == -11 ){  	// e+
			_rcspecies["e+"].push_back(allparticle[i]);
		}
		else if( PID == -13 ){  	// mu+
			_rcspecies["m+"].push_back(allparticle[i]);
		}
		else if( PID == -15 ){  	// tau+
			_rcspecies["tau+"].push_back(allparticle[i]);
		}
		else if(abs(PID)==22){ 	// photon
			_rcspecies["photon"].push_back(allparticle[i]);
		}
		else if(aPID==23){  	// Z 
			_rcspecies["Z"].push_back(allparticle[i]);
		}
		else if(aPID==24){  	// W 
			_rcspecies["W"].push_back(allparticle[i]);
		}
		else if(aPID==25){  	// higgs 
			_rcspecies["Higgs"].push_back(allparticle[i]);
		}
		else if((aPID%1000>110&&aPID%1000<120)){  	// higgs 
			_rcspecies["Light I=1 Mesons, pi0"].push_back(allparticle[i]);
		}
		else if(aPID%1000>210&&aPID%1000<220){  	// higgs 
			_rcspecies["Light I=1 Mesons, pi+"].push_back(allparticle[i]);
		}
		else if((aPID%1000>220&&aPID%1000<230)||(aPID%1000>331&&aPID%1000<340)){  	// higgs 
			_rcspecies["Light I=0 Mesons, eta"].push_back(allparticle[i]);
		}
		else if((aPID%1000>310&&aPID%1000<320)){  	// higgs 
			_rcspecies["Strange Mesons, K0   "].push_back(allparticle[i]);
		}
		else if((aPID%1000>320&&aPID%1000<330)){  	// higgs 
			_rcspecies["Strange Mesons, K+   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>410&&aPID%10000<420)){  	// higgs 
			_rcspecies["Charmed Mesons, D0   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>420&&aPID%10000<430)){  	// higgs 
			_rcspecies["Charmed Mesons, D+   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>510&&aPID%10000<520)||(aPID%10000>530&&aPID%10000<540)){  	// higgs 
			_rcspecies["Bottom Mesons, B0   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>520&&aPID%10000<530)||(aPID%10000>540&&aPID%10000<550)){  	// higgs 
			_rcspecies["Bottom Mesons, B+   "].push_back(allparticle[i]);
		}
		else if((aPID%10000>441&&aPID%10000<450)){  	// higgs 
			_rcspecies["ccbar Mesons        "].push_back(allparticle[i]);
		}
		else if((aPID%10000>551&&aPID%10000<560)){  	// higgs 
			_rcspecies["ccbar Mesons        "].push_back(allparticle[i]);
		}
		else if((aPID%10000>1000&&aPID%10000<4000)){  	// higgs 
			_rcspecies["Light Baryons       "].push_back(allparticle[i]);
		}
		else if((aPID%10000>4000&&aPID%10000<5000)){  	// higgs 
			_rcspecies["Charmed Baryons     "].push_back(allparticle[i]);
		}
		else if((aPID%10000>5000&&aPID%10000<6000)){  	// higgs 
			_rcspecies["Bottom Baryons      "].push_back(allparticle[i]);
		}
		else{
			_rcspecies["other hadron"].push_back(allparticle[i]);
		}
	}

	setRCNum();
}


void MyProcessor::ASpecies::Init(){
	_mcspecies.clear();
	_rcspecies.clear();
	_mcnum    .clear();
	_rcnum    .clear();
}

void MyProcessor::ASpecies::Clear(){
	_mcspecies.clear();
	_rcspecies.clear();
	_mcnum    .clear();
	_rcnum    .clear();
}





std::ostream & operator<<(std::ostream & ostr, MyProcessor::ASpecies spe){
	ostr<<"\n";
	for(MCSpeciesType::iterator it = spe.MCSpecies() .begin(); it!=spe.MCSpecies() .end(); ++it ){
		ostr<< it->first << " is:  " ;
		ostr<< it->second;
	}
	return ostr;
}

std::ostream & operator<<(std::ostream & ostr, MyProcessor::ASpecies* spe){
	ostr<<"\n";
	for(RCSpeciesType::iterator it = spe->RCSpecies().begin(); it!=spe->RCSpecies().end(); ++it ){
		ostr<< it->first << " is:  " << it->second;
	}
	return ostr;
}

std::ostream & operator<<(std::ostream & ostr, MCSpeciesType spe){
	ostr<<"\n";
	for(MCSpeciesType::iterator it = spe.begin(); it!=spe.end(); ++it ){
		ostr<< it->first << " is:  " << it->second;
	}
	return ostr;
}

std::ostream & operator<<(std::ostream & ostr, RCSpeciesType spe){
	ostr<<"\n";
	for(RCSpeciesType::iterator it = spe.begin(); it!=spe.end(); ++it ){
		ostr<< it->first << " is:  " << it->second;
	}
	return ostr;
}

std::ostream & operator<<(std::ostream & ostr, ASpeciesNumType spe){
	ostr<<"\n";
	for(ASpeciesNumType::iterator it = spe.begin(); it!=spe.end(); ++it ){
		ostr<< it->first << " num is:  " << it->second << std::endl;
	}
	return ostr;
}

