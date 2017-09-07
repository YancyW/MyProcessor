#include "TCanvas.h"
#include "TDirectory.h"
#include "TF1.h"
#include "TFile.h"
#include "TFitter.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2F.h"
#include "TH3.h"
#include "THStack.h"
#include "TImage.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TLine.h"
#include "TMath.h"
#include "TMatrix.h"
#include "TMatrixT.h"
#include "TMinuit.h"
#include "TProfile.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TTree.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <math.h>
#include <sstream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <time.h> 
#include <vector>


#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

void kcut_zpt(std::vector<std::string> mhmass) {

	std::vector<std::string> filename;

	int num=mhmass.size();
	std::cout << num <<std::endl;
	for(int i=0;i<num;i++){
		filename.push_back(mhmass[i] +".root");
	}
	std::vector<TFile*> in_file;
	std::vector<TH1F*> hvariable;
	hvariable.resize(filename.size());

	for(int i=0;i<filename.size();i++){
		TFile* in_file_tmp=new TFile(filename[i].c_str());
		in_file.push_back(in_file_tmp);
	}

	TCanvas *c1=new TCanvas("c1"," ",1000,700);
	auto leg = new TLegend(0.7,0.7,0.9,0.9);
	leg->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 
	gPad->SetLogy();

	string title_name="po_muon_kcut_zpt";
	string x_name=";"+title_name+ "(GeV/c^{2})";
	string y_name=";Events";
	string axis_name=x_name+y_name;
	int colornum=0;
	for(int i=0;i<filename.size();i++){
		TTree *MyLCTuple=(TTree*)in_file[i]->Get("datatrain");

		int nevent = MyLCTuple->GetEntries();
		float variable;
		MyLCTuple->SetBranchAddress(title_name.c_str(), &variable);
		//		MyLCTuple->SetBranchStatus("mc_ISR_elec_mass",1);

		hvariable[i] = new TH1F(title_name.c_str(),title_name.c_str(), 150,0,150);
		hvariable[i]->SetTitle(axis_name.c_str());


		for(int event=0;event<nevent;event++){
			MyLCTuple->GetEntry(event);
			hvariable[i]->Fill(variable);
		}

  	////  Double_t norm=hvariable[i]->GetEntries();
  	////  if(norm>0){hvariable[i]->Scale(1/norm);}
		  
		colornum++;
		if(colornum==3||colornum==5||colornum==8||colornum==10||colornum==11){
			colornum++;
		}
		hvariable[i]->SetLineColor(colornum);
		std::string legname=filename[i];
		leg->AddEntry(hvariable[i],legname.c_str(),"l");
	}



    THStack *s = new THStack();
    for(int i=0;i<filename.size();i++){
    	s->Add(hvariable[i]);
    }
    s->Draw("nostack");
	s->GetYaxis()->SetTitle("Events");
	s->GetXaxis()->SetTitle("P_{T}^{Z} GeV/c^{2}");
	leg->Draw();

	c1->Update();
	TImage *img = TImage::Create();
	img->FromPad(c1);
	std::string pngname=title_name+".png";
	img->WriteImage(pngname.c_str());


	return;
}
