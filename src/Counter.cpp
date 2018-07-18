#include "MyProcessor.h"

void MyProcessor::ACounter::setWeight(float value){
	_weight = value;
}

void MyProcessor::ACounter::setCounterSingle(std::string event_name,float num){
	_counter[event_name]=_counter[event_name]+num;
}

void MyProcessor::ACounter::setMCCounter(ASpecies& spe){
	for(MCSpeciesType::iterator it = spe._mcspecies.begin(); it!=spe._mcspecies.end(); ++it ){
		setCounterSingle(it->first,it->second.size()*_weight);	
	}
}

void MyProcessor::ACounter::setRCCounter(ASpecies& spe){
	for(RCSpeciesType::iterator it = spe._rcspecies.begin(); it!=spe._rcspecies.end(); ++it ){
		setCounterSingle(it->first,it->second.size()*_weight);	
	}
}

void MyProcessor::ACounter::showCounter(){
	ShowMessage(2," OutPut --- Species for All Particle" );
	for(ACounterType::iterator it = _counter.begin(); it!= _counter.end(); ++it ){
		ShowMessage(2,"number  of "+it->first+" is" , it->second);
	}
}


void MyProcessor::ACounter::Clear(){
	_counter          .clear();
	_weight           = 1;
}
