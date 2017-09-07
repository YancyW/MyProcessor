#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"

#include "TMVA/Factory.h"
#include "TMVA/Tools.h"
#include "TMVA/DataLoader.h"
#include "TMVA/TMVAMultiClassGui.h"

Bool_t ReadDataFromAsciiIFormat = kFALSE;

using namespace TMVA;

//void testtraining(std::vector<std::string> sig_name,std::vector<std::string> bkg_name,std::vector<std::string> variable_name,std::string mass)
void testtraining(std::string mass)
{
	TMVA::Tools::Instance();
	std::cout << mass << std::endl;
	std::map<std::string,int> Use;

	// --- Boosted Decision Trees
	Use["BDTG"]             = 1; // uses Adaptive Boost

	std::cout << std::endl << "==> Start TMVAClassification" << std::endl;

	bool batchMode(false);
	bool useDefaultMethods(true);

	std::string out_root="lctuple_"+mass+".root";
	TString outfileName( out_root );
	TFile* outputFile = TFile::Open( outfileName, "RECREATE" );

	TMVA::Factory *factory = new TMVA::Factory( "TMVAClassification", outputFile,
			"!V:!Silent:Color:!DrawProgressBar:Transformations=I;D;P;G,D:AnalysisType=Classification" );

	TMVA::DataLoader *dataloader=new TMVA::DataLoader("dataset");

	dataloader->AddVariable( "po_muon_kcut_zmass", 'F' );
	dataloader->AddVariable( "po_muon_lep_pair_costheta", 'F' );
	dataloader->AddVariable( "po_muon_lep_pair_costheta_pair", 'F' );
	dataloader->AddVariable( "po_muon_lep_pair_costheta_track1", 'F' );
	dataloader->AddVariable( "po_muon_lep_pair_costheta_track2", 'F' );


	TString fileS1= "../../../Analyse_Root/results_"+mass+"_MVA/nh_"+mass+"_"+mass+"_"+mass+".root";
	std::vector<TString> fileB;
	fileB.push_back("../../../Analyse_Root/results_"+mass+"_MVA/4f_ww_l_"+mass+".root"  );

	//TFile *inputS = TFile::Open( fileS );
	TChain chainsigtrain("datatrain");
	chainsigtrain.Add(fileS1);
	TChain chainsigtest("datatest");
	chainsigtest.Add(fileS1);
	TChain chainbgtrain("datatrain");
	for(int i=0;i<fileB.size();i++){
		chainbgtrain.Add(fileB[i]);
	}
	TChain chainbgtest("datatest");
	for(int i=0;i<fileB.size();i++){
		chainbgtest.Add(fileB[i]);
	}

	////for(int i=0;i<=variable_name.size();i++){
	////	factory->AddVariable( variable_name[i], 'F' );
	////}
	////TChain chainbg("datatrain");
	////for(int i=0;i<fileB.size();i++){
	////	chainbg.Add(fileB[i]);
	////}
	////TChain chainbgtest("datatest");
	////for(int i=0;i<fileB.size();i++){
	////	chainbgtest.Add(fileB[i]);
	////}
	////TChain chainsigtrain("datatrain");
	////TChain chainsigtest ("datatest");
	////for(int i=0;i<=sig_name.size();i++){
	////	chainsigtrain.Add(sig_name[i]);
	////	chainsigtest .Add(sig_name[i]);
	////}
	////TChain chainbgtrain("datatrain");
	////TChain chainbgtest ("datatest");
	////for(int i=0;i<=bkg_name.size();i++){
	////	chainbgtrain.Add(bkg_name[i]);
	////	chainbgtest .Add(bkg_name[i]);
	////}

	TChain *signalTrain     = &chainsigtrain;
	TChain *signalTest      = &chainsigtest ;
	TChain *backgroundTrain = &chainbgtrain ;
	TChain *backgroundTest  = &chainbgtest  ;

	// global event weights per tree (see below for setting event-wise weights)
	Double_t signalWeight     = 1.0;
	Double_t backgroundWeight = 1.0;

	dataloader->SetSignalWeightExpression("weight");
	dataloader->SetBackgroundWeightExpression("weight");

	dataloader->AddSignalTree    ( signalTrain    , signalWeight    , "Training" );
	dataloader->AddSignalTree    ( signalTest     , signalWeight    , "Test"     );
	dataloader->AddBackgroundTree( backgroundTrain, backgroundWeight, "Training" );
	dataloader->AddBackgroundTree( backgroundTest , backgroundWeight, "Test"     );


	// Apply additional cuts on the signal and background samples (can be different)
	TCut mycuts = "po_muon_kcut_zmass>0&&po_muon_lep_pair_costheta_pair>-1&&po_muon_lep_pair_costheta>-1&&po_muon_lep_pair_costheta_track1>-1&&po_muon_lep_pair_costheta_track2>-1";
	TCut mycutb = "!(po_muon_kcut_zmass>0&&po_muon_lep_pair_costheta_pair>-1&&po_muon_lep_pair_costheta>-1&&po_muon_lep_pair_costheta_track1>-1&&po_muon_lep_pair_costheta_track2>-1)";

	dataloader->PrepareTrainingAndTestTree( mycuts, "nTrain_Signal=0:nTrain_Background=0:SplitMode=Random:NormMode=NumEvents:!V" );

	cout << "Booking Start******" << endl;
	if (Use["BDTG"]) {
		factory->BookMethod(dataloader,  TMVA::Types::kBDT, "BDTG",
            "!H:!V:NTrees=1000:MinNodeSize=2.5%:BoostType=Grad:Shrinkage=0.10:UseBaggedBoost:BaggedSampleFraction=0.5:nCuts=20:MaxDepth=2" );
	} // Adaptive Boost

	cout << "Training Start*****" << endl;
	// Train MVAs using the set of training events
	factory->TrainAllMethods();
	cout << "Training End ******" << endl;

	// ---- Evaluate all MVAs using the set of test events
	cout << "Testing Start*****" << endl;
	factory->TestAllMethods();
	cout << "Testing End ******" << endl;

	// ----- Evaluate and compare performance of all configured MVAs
	cout << "Evaluation Start*****" << endl;
	factory->EvaluateAllMethods();
	cout << "Evaluation End ******" << endl;

	// --------------------------------------------------------------

	// Save the output
	outputFile->Close();

	std::cout << "==> Wrote root file: " << outputFile->GetName() << std::endl
		<< "==> TMVAClassification is done!" << std::endl
		<< std::endl
		<< "==> To view the results, launch the GUI: \"root -l ./TMVAGui.C\"" << std::endl
		<< std::endl;

	// Clean up
	delete factory;
	delete dataloader;
	if(!gROOT->IsBatch()) TMVAMultiClassGui(outfileName);
}

