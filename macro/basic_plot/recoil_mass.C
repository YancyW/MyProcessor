#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TMath.h>
#include <TLatex.h>
#include <string>
#include <vector>


void recoil_mass(std::vector<std::string> mhmass) {

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

	int colornum=0;
	for(int i=0;i<filename.size();i++){
		TTree *MyLCTuple=(TTree*)in_file[i]->Get("dataTree");

		int nevent = MyLCTuple->GetEntries();
		float variable;
		MyLCTuple->SetBranchAddress("mc_recoil_mass", &variable);
//		MyLCTuple->SetBranchStatus("mc_recoil_mass",1);

		hvariable[i] = new TH1F("mc_recoil_mass>>h1","recoil mass",100,1,250);
		hvariable[i]->SetTitle(";Recoil Mass(GeV/c^{2});Events");


		for(int event=0;event<nevent;event++){
			MyLCTuple->GetEntry(event);
			hvariable[i]->Fill(variable);
		}

		colornum++;
		if(colornum==3||colornum==5||colornum==8||colornum==10||colornum==11){
			colornum++;
		}
		hvariable[i]->SetLineColor(colornum);
		std::string legname=filename[i];
		leg->AddEntry(hvariable[i],legname.c_str(),"l");
	}



	for(int i=0;i<filename.size();i++){
		if(i==0){
			hvariable[i]->Draw(); 
		}
		else
		{
			hvariable[i]->Draw("Same"); 
		}
	}

////THStack *s = new THStack();
////for(int i=0;i<filename.size();i++){
////	s->Add(hvariable[i]);
////}
////s->Draw("nostack");
	leg->Draw();

	c1->Update();
	TImage *img = TImage::Create();
	img->FromPad(c1);
	std::string pngname="recoil_mass.png";
	img->WriteImage(pngname.c_str());


	return;
}
