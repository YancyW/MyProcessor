#ifndef PRINT_H
#define PRINT_H  
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "marlin/Processor.h"
#include "EVENT/MCParticle.h"
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Vertex.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include "UTIL/PIDHandler.h"
#include "LCIOSTLTypes.h"
#include "TLorentzVector.h"



std::ostream & operator << (std::ostream & ostr, const LCEvent *in);

std::ostream & operator << (std::ostream & ostr, const MCParticle *in);
std::ostream & operator << (std::ostream & ostr, const ReconstructedParticle *in);
std::ostream & operator << (std::ostream & ostr, const Track *in);

std::ostream & operator << (std::ostream & ostr, const std::vector<MCParticle*> &invec);
std::ostream & operator << (std::ostream & ostr, const std::vector<ReconstructedParticle*> &invec);
std::ostream & operator << (std::ostream & ostr, const std::vector<Track*> &invec);


std::ostream & operator << (std::ostream & ostr, const TLorentzVector &invec);
std::ostream & operator << (std::ostream & ostr, const std::vector<TLorentzVector> &invec);
#endif
