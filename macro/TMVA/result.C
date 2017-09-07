#include <cstdlib>
#include <vector>
#include <iostream>
#include <map>
#include <string>

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TSystem.h"
#include "TROOT.h"
#include "TStopwatch.h"
#include "TTreeFormula.h"

#include "TMVA/Reader.h"
#include "TMVA/Config.h"
#include "TMVA/Tools.h"
#include "TMVA/MethodCuts.h"

using namespace std;

//void result( const char *fname )
//void result( std::vector<std::string> in_name )
void result( std::string in_name )
{

	// --- Create the Reader object
  vector<string> varsFinal;
  varsFinal.push_back("po_muon_kcut_zmass");
  varsFinal.push_back("po_muon_lep_pair_costheta");
  varsFinal.push_back("po_muon_lep_pair_costheta_pair");
  varsFinal.push_back("po_muon_lep_pair_costheta_track1");
  varsFinal.push_back("po_muon_lep_pair_costheta_track2");

	TMVA::Reader *reader= new TMVA::Reader("!Color:!Silent" );
	Float_t var1, var2,var3,var4,var5;
	reader->AddVariable("po_muon_kcut_zmass",&var1);
	reader->AddVariable("po_muon_lep_pair_costheta",&var2);
	reader->AddVariable("po_muon_lep_pair_costheta_pair",&var3);
	reader->AddVariable("po_muon_lep_pair_costheta_track1",&var4);
	reader->AddVariable("po_muon_lep_pair_costheta_track2",&var5);

	// --- Book the MVA methods
	reader->BookMVA("mymethod","../../../Analyse_Root/dataset/weights/TMVAClassification_BDTG.weights.xml");

//	for(int i=0;i<in_name.size();i++){
//		std::string fname=in_name[i];
  		std::string fname=in_name;
		TFile* input = TFile::Open( fname.c_str(),"update" );

		if (!input) {
			std::cout << "ERROR: could not open data file" << std::endl;
			exit(1);
		}
		std::cout << "--- TMVAClassificationApp    : Using input file: " << input->GetName() << std::endl;

		// --- Event loop

		TTree* tree3 = (TTree*)input->Get("datatest");

		double BDTGoutput;
		TBranch *br = tree3->Branch("BDTG",&BDTGoutput);

		std::cout << "--- Processing: " << tree3->GetEntries() << " events" << std::endl;
		TStopwatch sw;
		sw.Start();

		Int_t nEvent = tree3->GetEntries();
		for (Long64_t ievt=0; ievt<nEvent; ievt++) {
			if (ievt%1000 == 0) std::cout << "--- ... Processing event: " << ievt << std::endl;
			tree3->GetEntry(ievt);

			vector<float> f;
			f.resize(varsFinal.size());
			for(unsigned int nvar=0;nvar<varsFinal.size();nvar++){
				TTreeFormula f1("n",varsFinal[nvar].c_str(),tree3);
				f[nvar] = f1.EvalInstance(ievt);
			}
			BDTGoutput = reader->EvaluateMVA(f,"mymethod");
			br->Fill();
		}
		tree3->Write();
		input->Close();

		std::cout << "--- Created root file containing the MVA output" << std::endl;

		delete reader;

		std::cout << "==> TMVAClassificationApplication is done!" << std::endl << std::endl;

//	}
}
