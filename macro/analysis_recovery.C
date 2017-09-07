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

ofstream myfile;

template<typename TCut1, typename TCut2>
bool GetCut(TCut1 data, TCut2 limit_down, TCut2 limit_up){
	if(data<limit_down || data>limit_up){
		return(false);
	}
	else{
		return(true);
	}
}

template<typename TCut1, typename TCut2>
bool GetCut(TCut1 data, TCut2 limit_down, TCut2 limit_up,Int_t &pass){
	if(data<limit_down || data>limit_up){
		return(false);
	}
	else{
		pass++;
		return(true);
	}
}


template <typename TMess1, typename TMess2> 
void ShowMessage(Int_t level, TMess1 mess, TMess2 ddata){
	std::cout.setf(std::ios::left);
	std::string fill1="";
	std::string fill2="     ";
	std::string fill3="           ";
	std::string fill4="               ";
	std::string fill5="                    ";
	std::string fill6="                         ";
	std::string fill;

	int  fillnum1=0;
	int  fillnum2=5;
	int  fillnum3=10;
	int  fillnum4=15;
	int  fillnum5=20;
	int  fillnum6=25;
	int  fillnum=0;

	switch(level){
		case 1:fill=fill1;fillnum=fillnum1;break;
		case 2:fill=fill2;fillnum=fillnum2;break;
		case 3:fill=fill3;fillnum=fillnum3;break;
		case 4:fill=fill4;fillnum=fillnum4;break;
		case 5:fill=fill5;fillnum=fillnum5;break;
		case 6:fill=fill6;fillnum=fillnum6;break;
		default:fill=fill1;
	}

	myfile << fill << mess  << ":    " << ddata << "\n";
}

void analysis_recovery(std::vector<std::string> arg_string1,std::vector<std::string> arg_string2,std::string out_name) {


	if(arg_string1.size()!=arg_string2.size()){
		std::cout << "Error in analysis: input arguments size not equal to each other!" << std::endl;
		return;
	}

	//controler
	bool plot_all=false;

	myfile.open(out_name.c_str());
	ShowMessage(1,"filenum", "");

	//setting
	int colornum[12]={1,2,4,6,7,9,12,13,14,15,16,17};

	//file
	std::vector<std::string> filename;
	std::vector<int>         nevent;

	int num=arg_string1.size();
	for(int i=0;i<num;i++){
		filename.push_back(arg_string1[i]);
	}
	std::vector<TFile*> in_file;

	std::vector<Int_t> pass_basic_muon ;
	std::vector<Int_t> pass_mll        ;
	std::vector<Int_t> pass_ptll       ;
	std::vector<Int_t> pass_mis_theta  ;
	std::vector<Int_t> pass_BDT        ;
	std::vector<Int_t> pass_rec        ;
	std::vector<Int_t> pass_vis        ;
	std::vector<Int_t> pass            ;
	std::vector<Int_t> pass_pre        ;

	for(unsigned int i=0;i<filename.size();i++){
		TFile* in_file_tmp=new TFile(filename[i].c_str());
		in_file.push_back(in_file_tmp);

		pass_basic_muon.push_back(0);
		pass_mll       .push_back(0);
		pass_ptll      .push_back(0);
		pass_mis_theta .push_back(0);
		pass_BDT       .push_back(0);
		pass_rec       .push_back(0);
		pass_vis       .push_back(0);
		pass           .push_back(0);
		pass_pre       .push_back(0);
	}




	// the first  vector --- for file
	// the second vector --- for plot 
	std::vector< std::vector<TH1F*> >    hvariable;
	hvariable.resize(filename.size());

	std::vector<float>    variable;
	std::vector<TCanvas*> c1;
	std::vector<string>   c1_name;
	std::vector<float>    c1_width;
	std::vector<float>    c1_height;
	std::vector<TLegend*> leg;
	std::vector<string>   title_name;
	std::vector<string>   x_name;
	std::vector<int>      x_histbin;
	std::vector<float>    x_histmin;
	std::vector<float>    x_histmax;
	std::vector<string>   y_name;
	std::vector<int>      y_histbin;
	std::vector<float>    y_histmin;
	std::vector<float>    y_histmax;
	std::vector<bool>     plot_switch;
	std::vector<bool>     log_switch;
	std::vector<bool>     norm_switch;

	std::vector<string>   aux_fname;
	std::vector<string>   aux_iname;
	std::vector<float>    aux_f;
	std::vector<Int_t>    aux_i;

	//setting for obseravables
	//
	//variables that do not want to plot
	title_name    .push_back("po_muon_recovery_kcut_zmass"                   );
	c1_name       .push_back("c1"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("M_{Z} (GeV/c^{2})"                    );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(150                                    );
	//plot_switch   .push_back(false                                  );
	plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	title_name    .push_back("po_muon_recovery_kcut_zpt"                     );
	c1_name       .push_back("c2"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.75,0.7,0.92,0.9)         );
	x_name        .push_back("P_{T}^{l^{+}l^{-}} (GeV/c^{2})"       );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(100                                    );
	//plot_switch   .push_back(false                                  );
	plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_recovery_kcut_invis_costheta"          );
	c1_name       .push_back("c3"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos #theta_{missing}"                 );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_recovery_kcut_recoil_mass"             );
	c1_name       .push_back("c4"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("Recoil Mass (GeV/c^{2})"              );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(300                                    );
	//plot_switch   .push_back(false                                  );
	plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_recovery_kcut_vis_e"                   );
	c1_name       .push_back("c5"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("E_{vis} (GeV/c^{2})"                  );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(250                                    );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_recovery_lep_pair_mass"                );
	c1_name       .push_back("c6"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.75,0.9,0.9)          );
	x_name        .push_back("M_{l^{+}l^{-}} (GeV/c^{2})"           );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(60                                     );
	x_histmax     .push_back(130                                    );
	//plot_switch   .push_back(false                                  );
	plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_recovery_lep_pair_costheta"            );
	c1_name       .push_back("c7"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos#theta_{l^{+}l^{-}}"               );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_recovery_lep_pair_costheta_pair"       );
	c1_name       .push_back("c8"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos#theta_{lep}"                      );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	title_name    .push_back("po_muon_recovery_lep_pair_costheta_track1"     );
	c1_name       .push_back("c9"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos#theta_{1}"                        );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                );
	//plot_switch   .push_back(true                                   );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	title_name    .push_back("po_muon_recovery_lep_pair_costheta_track2"     );
	c1_name       .push_back("c11"                                  );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos#theta_{2}"                        );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                );
	//plot_switch   .push_back(true                                   );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	variable.resize(c1_name.size());
	for(unsigned int i=0;i<filename.size();i++){
		hvariable[i].resize(c1_name.size());
	}


	if(!plot_all){
		plot_switch.clear();
		for(int i=0;i<c1_name.size();i++){
			plot_switch   .push_back(false);
		}
	}


	//variables that do not want to plot
	//aux_0
	aux_iname     .push_back("po_muon_num_recovery"				            );
	aux_iname     .push_back("po_muon_num_recovery_plus"				        );
	aux_iname     .push_back("po_muon_num_recovery_minus"				    );
	aux_i.resize(aux_iname.size());


	//aux_0
	aux_f.resize(aux_fname.size());




	//loop for all input files
	for(unsigned int i=0;i<filename.size();i++){
		TTree *MyLCTuple=(TTree*)in_file[i]->Get("datatrain");
		nevent.push_back(MyLCTuple->GetEntries());



		//loop for init variables which are prepared for plot 
		for(unsigned int j=0;j<title_name.size();j++){
			MyLCTuple->SetBranchAddress(title_name[j].c_str(), &variable[j]);
			if(plot_switch[j]){
				hvariable[i][j] = new TH1F(title_name[j].c_str(),title_name[j].c_str(),x_histbin[j],x_histmin[j],x_histmax[j]);
			}
		}

		//loop for init variables which are used in calculation 
		for(unsigned int j=0;j<aux_iname.size();j++){
			MyLCTuple->SetBranchAddress(aux_iname[j].c_str(), &aux_i[j]);
		}

		//loop for init variables which are used in calculation 
		for(unsigned int j=0;j<aux_fname.size();j++){
			MyLCTuple->SetBranchAddress(aux_fname[j].c_str(), &aux_f[j]);
		}

		//loop for reading variables and setting cuts 
		for(int event=0;event<nevent[i];event++){
			MyLCTuple->GetEntry(event);

			for(unsigned int j=0;j<title_name.size();j++){
				if(plot_switch[j]){
					hvariable[i][j]->Fill(variable[j]);
				}
			}

			// add cuts
			if(!GetCut(aux_i[0]   ,    2,   100                   )){continue;}
			if(!GetCut(aux_i[1]   ,    1,   100                   )){continue;}
			if(!GetCut(aux_i[2]   ,    1,   100,pass_basic_muon[i])){continue;}
	//		if(!GetCut(variable[3],100.0, 300.0,pass_pre       [i])){continue;}
			if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
			if(!GetCut(variable[1], 10.0,  70.0,pass_ptll      [i])){continue;}
			if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
			if(!GetCut(variable[3],110.0, 155.0,pass_rec       [i])){continue;}
			if(!GetCut(variable[4], 10.0, 250.0,pass_vis       [i])){continue;}
			pass[i]++;
		}

		ShowMessage(2,filename        [i]              , ""                 );
		ShowMessage(3,"weight                         ", arg_string2     [i]);
		ShowMessage(3,"all entries                    ", nevent          [i]);
		ShowMessage(3,"passed cut muon precut         ", pass_basic_muon [i]);
		ShowMessage(3,"passed cut m ll                ", pass_mll        [i]);
		ShowMessage(3,"passed cut pt ll               ", pass_ptll       [i]);
		ShowMessage(3,"passed cut mis_theta           ", pass_mis_theta  [i]);
		ShowMessage(3,"passed cut BDT                 ", pass_BDT        [i]);
		ShowMessage(3,"passed cut recoil mass         ", pass_rec        [i]);
		ShowMessage(3,"passed cut vis E               ", pass_vis        [i]);
		ShowMessage(3,"passed cut all                 ", pass            [i]);

	}


	//plot
	int k=0;
	for(unsigned int j=0;j<title_name.size();j++){
		if(plot_switch[j]){
			c1.push_back(new TCanvas(c1_name[j].c_str()," ",c1_width[j],c1_height[j]));
			if(log_switch[j]){
				gPad->SetLogy();
			}


			THStack *s = new THStack();
			for(unsigned int i=0;i<filename.size();i++){
				if(norm_switch[j]){
					gPad->SetLogy();
					Double_t norm=hvariable[i][j]->GetEntries();
					if(norm>0){hvariable[i][j]->Scale(1/norm);}
				}
				hvariable[i][j]->SetLineColor(colornum[i]);
				leg[j]->AddEntry(hvariable[i][j],filename[i].c_str(),"l");
				s->Add(hvariable[i][j]);
			}
			s->Draw("nostack");
			s->GetYaxis()->SetTitle(y_name[j].c_str());
			s->GetXaxis()->SetTitle(x_name[j].c_str());
			leg[j]->Draw();

			//print to png
			c1[k]->Update();
			TImage *img = TImage::Create();
			img->FromPad(c1[k]);
			std::string pngname=title_name[j]+".png";
			img->WriteImage(pngname.c_str());
			k++;

		}
	}

	myfile.close();

	return;
}
