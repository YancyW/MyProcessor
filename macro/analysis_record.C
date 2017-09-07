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
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string> 
#include <time.h> 
#include <vector>


//#include <gsl/gsl_errno.h>
//#include <gsl/gsl_matrix.h>
//#include <gsl/gsl_odeiv2.h>

ofstream myfile;

template <class TType>
TType String_To_Num(const std::string& str){
	stringstream iss(str);
	TType num;
	iss >> num;
	return(num);
}

template <class TType>
std::string Num_To_String(TType i){
	std::string fnum;
	stringstream num;

	num << i;
	num >> fnum;
	return(fnum);
}


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
bool GetCut(TCut1 data, TCut2 limit_down, TCut2 limit_up, float &pass){
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

std::string change_name(std::string arg_string){
	std::string out_name=arg_string;
	int fs1=out_name.find("{");
	out_name.replace(fs1,1,"");
	fs1=out_name.find("{",fs1+1);
	out_name.replace(fs1,1,"");
	fs1=out_name.find("}");
	out_name.replace(fs1,1,"");
	fs1=out_name.find("}",fs1+1);
	out_name.replace(fs1,1,"");
	fs1=out_name.find("^");
	out_name.replace(fs1,1,"_");
	return(out_name);
}

void analysis_record(std::vector<std::string> arg_string1,std::string arg_string3, std::vector<std::string> arg_string2,std::string out_name,float mass) {
//(array for file name with different polarization , out put root name/and name in the table, array for cross section , output name,  higgs_mass){

	float lumi=250;


	if(arg_string1.size()!=arg_string2.size()){
		std::cout << "Error in analysis: input arguments size not equal to each other!" << std::endl;
		return;
	}

	std::vector<float> xection;
	for(int i=0;i<arg_string2.size();i++){
		xection.push_back(String_To_Num<float>(arg_string2[i]));
	}
	//controler
	bool plot_all=true;

	myfile.open(out_name.c_str());
	ShowMessage(1,"filenum", "");

	//setting
	int colornum[12]={1,2,4,6,7,9,12,13,14,15,16,17};

	//file
	std::vector<std::string> filename;
	std::vector<int>         nevent;
	std::vector<float>       weight;
	std::vector<float>       pol;

	float elec_pol=-0.8;
	float posi_pol=+0.3;
	float elecm=(1 - elec_pol)/2;
	float elecp=(1 + elec_pol)/2;
	float posim=(1 - posi_pol)/2;
	float posip=(1 + posi_pol)/2;
	pol.push_back( elecm * posip * lumi);
	pol.push_back( elecp * posim * lumi);
	pol.push_back( elecm * posim * lumi);
	pol.push_back( elecp * posip * lumi);

	int filenum=arg_string1.size();
	for(int i=0;i<filenum;i++){
		filename.push_back(arg_string1[i]);
	}
	filename.push_back("total");


	std::vector<TFile*> in_file;

	std::vector<float> pass_basic_muon ;
	std::vector<float> pass_mll        ;
	std::vector<float> pass_ptll       ;
	std::vector<float> pass_mis_theta  ;
	std::vector<float> pass_BDT        ;
	std::vector<float> pass_rec        ;
	std::vector<float> pass_vis        ;
	std::vector<float> pass            ;
	std::vector<float> pass_pre        ;

	for(unsigned int i=0;i<filenum+1;i++){
		if(i!=filenum){
			TFile* in_file_tmp=new TFile(filename[i].c_str());
			in_file.push_back(in_file_tmp);
		}

		nevent         .push_back(0);
		weight         .push_back(0.0);
		pass_basic_muon.push_back(0.0);
		pass_mll       .push_back(0.0);
		pass_ptll      .push_back(0.0);
		pass_mis_theta .push_back(0.0);
		pass_BDT       .push_back(0.0);
		pass_rec       .push_back(0.0);
		pass_vis       .push_back(0.0);
		pass           .push_back(0.0);
		pass_pre       .push_back(0.0);
	}


	std::vector<TTree*> MyLCTuple;
	for(unsigned int i=0;i<filenum;i++){
		MyLCTuple.push_back((TTree*)in_file[i]->Get("datatrain"));
	}





	//create root file in the ./results_Mh  folder
	std::string mass_str=Num_To_String(mass);
	std::string outroot_name=change_name(arg_string3);
	std::string outroot="../results_"+mass_str+"/"+outroot_name+"_"+mass_str+".root";
	TFile* otfile = TFile::Open( outroot.c_str() , "RECREATE" );
	if (!otfile) {
		std::cout << "ERROR: could not open data file" << std::endl;
		exit(1);
	}
	TTree* datatrain= new TTree( "datatrain11" , "events" );
	//TH1F *hout=new TH1F("hout","x distribution",100,-4,4);
	//TNtuple *ntuple = new TNtuple("ntuple","data","x:y:z");


	// the first  vector --- for file
	// the second vector --- for plot 
	std::vector< std::vector<TH1F*> >    hvariable;
	hvariable.resize(filenum+1);

	std::vector<float>    variable;
	std::vector<float>    rootvar;
	std::vector<TCanvas*> c1;
	std::vector<string>   c1_name;
	std::vector<float>    c1_width;
	std::vector<float>    c1_height;
	std::vector<TLegend*> leg;
	std::vector<string>   title_name;
	std::vector<string>   latex_name;
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
	title_name    .push_back("mc_muon_kcut_zmass"                   );
	latex_name    .push_back("M_{l^{+}l^{-}}                 "      );
	c1_name       .push_back("c1"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("M_{Z} (GeV/c^{2})"                    );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(150                                    );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	title_name    .push_back("po_muon_kcut_zpt"                     );
	latex_name    .push_back("P_T^{l^{+}l^{-}}               "      );
	c1_name       .push_back("c2"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.75,0.7,0.92,0.9)         );
	x_name        .push_back("P_{T}^{l^{+}l^{-}} (GeV/c^{2})"       );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(100                                    );
	//plot_switch   .push_back(false                                );
	plot_switch   .push_back(true                                   );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_kcut_invis_costheta"          );
	latex_name    .push_back("cos\\theta_{missing}            "     );
	c1_name       .push_back("c3"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos #theta_{missing}"                 );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	//plot_switch   .push_back(false                                );
	plot_switch   .push_back(true                                   );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("mc_muon_kcut_recoil_mass"             );
	latex_name    .push_back("M_{rec}                        "      );
	c1_name       .push_back("c4"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("Recoil Mass (GeV/c^{2})"              );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(0                                      );
	x_histmax     .push_back(300                                    );
	//plot_switch   .push_back(false                                );
	plot_switch   .push_back(true                                   );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 

	title_name    .push_back("po_muon_kcut_vis_e"                   );
	latex_name    .push_back("E_{vis}                        "      );
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

	title_name    .push_back("po_muon_lep_pair_costheta"            );
	latex_name    .push_back("cos\\theta_{Z}                 "      );
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

	title_name    .push_back("po_muon_lep_pair_costheta_pair"       );
	latex_name    .push_back("cos\\theta_{lep}               "      );
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


	title_name    .push_back("po_muon_lep_pair_costheta_track1"     );
	latex_name    .push_back("cos\\theta_{track,1}           "      );
	c1_name       .push_back("c9"                                   );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos#theta_{1}"                        );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	title_name    .push_back("po_muon_lep_pair_costheta_track2"     );
	latex_name    .push_back("cos\\theta_{track,2}           "      );
	c1_name       .push_back("c10"                                  );
	c1_width      .push_back(1000                                   );
	c1_height     .push_back( 700                                   );
	leg           .push_back(new TLegend(0.6,0.7,0.9,0.9)           );
	x_name        .push_back("cos#theta_{2}"                        );
	y_name        .push_back("Events"                               );
	x_histbin     .push_back(100                                    );
	x_histmin     .push_back(-1                                     );
	x_histmax     .push_back( 1                                     );
	plot_switch   .push_back(false                                  );
	//plot_switch   .push_back(true                                 );
	log_switch    .push_back(true                                   );
	norm_switch   .push_back(false                                  );
	leg[leg.size()-1]->SetHeader("e^{+}e^{-}->#mu^{+}#mu^{-} + X @ 250 GeV","C"); 


	variable.resize(c1_name.size());
	rootvar.resize(c1_name.size());
	for(unsigned int i=0;i<filenum+1;i++){
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
	aux_fname     .push_back("po_muon_num"  			            );
	aux_fname     .push_back("po_muon_num_plus" 			        );
	aux_fname     .push_back("po_muon_num_minus"				    );
	aux_f.resize(aux_fname.size());




	float out_weight;
	datatrain->Branch( "weight"              , &out_weight );        
	datatrain->Branch( title_name[0].c_str() , &rootvar[0] );        
	datatrain->Branch( title_name[1].c_str() , &rootvar[1] );        
	datatrain->Branch( title_name[2].c_str() , &rootvar[2] );        
	datatrain->Branch( title_name[3].c_str() , &rootvar[3] );        
	datatrain->Branch( title_name[4].c_str() , &rootvar[4] );        
	datatrain->Branch( title_name[5].c_str() , &rootvar[5] );        
	datatrain->Branch( title_name[6].c_str() , &rootvar[6] );        
	datatrain->Branch( title_name[7].c_str() , &rootvar[7] );        
	datatrain->Branch( title_name[8].c_str() , &rootvar[8] );        



	//loop for all input files
	for(unsigned int i=0;i<filenum;i++){
		nevent[i]=(MyLCTuple[i]->GetEntries());
		weight[i]=(xection[i]/nevent[i]*pol[i]);



		//loop for init variables which are prepared for plot 
		for(unsigned int j=0;j<title_name.size();j++){
			MyLCTuple[i]->SetBranchAddress(title_name[j].c_str(), &variable[j]);
			if(plot_switch[j]){
				if(i==0){
					hvariable[filenum][j] = new TH1F(title_name[j].c_str(),title_name[j].c_str(),x_histbin[j],x_histmin[j],x_histmax[j]);
				}
				hvariable[i][j] = new TH1F(title_name[j].c_str(),title_name[j].c_str(),x_histbin[j],x_histmin[j],x_histmax[j]);
			}
		}

		//loop for init variables which are used in calculation 
		for(unsigned int j=0;j<aux_iname.size();j++){
			MyLCTuple[i]->SetBranchAddress(aux_iname[j].c_str(), &aux_i[j]);
		}

		//loop for init variables which are used in calculation 
		for(unsigned int j=0;j<aux_fname.size();j++){
			MyLCTuple[i]->SetBranchAddress(aux_fname[j].c_str(), &aux_f[j]);
		}

		//loop for reading variables and setting cuts 
		for(int event=0;event<nevent[i];event++){
			MyLCTuple[i]->GetEntry(event);

			for(unsigned int j=0;j<title_name.size();j++){
				if(plot_switch[j]){
					hvariable[i][j]->Fill(variable[j]);
					hvariable[filenum][j]->Fill(variable[j],weight[i]);
				}
				rootvar[j]=variable[j];
				out_weight=weight[i];
			}

			datatrain->Fill();

			// add cuts
			if(!GetCut(aux_f[0]   ,    2,   100                   )){continue;}
			if(!GetCut(aux_f[1]   ,    1,   100                   )){continue;}
			if(!GetCut(aux_f[2]   ,    1,   100,pass_basic_muon[i])){continue;}


			if(mass==125){
				if(!GetCut(variable[3],100.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1], 10.0,  70.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3],110.0, 155.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4], 10.0, 250.0,pass_vis       [i])){continue;}
			}
			else if(mass==10){
				if(!GetCut(variable[3],  0.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1],  0.0, 120.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3],  0.0,  40.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4],  0.0, 250.0,pass_vis       [i])){continue;}
			}
			else if(mass==30){
				if(!GetCut(variable[3],  5.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1], 10.0, 100.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3], 15.0,  60.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4],  0.0, 250.0,pass_vis       [i])){continue;}
			}
			else if(mass==50){
				if(!GetCut(variable[3], 25.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1], 10.0,  90.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3], 35.0,  80.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4], 10.0, 250.0,pass_vis       [i])){continue;}
			}
			else if(mass==70){
				if(!GetCut(variable[3], 45.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1], 10.0,  80.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3], 55.0, 100.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4], 10.0, 250.0,pass_vis       [i])){continue;}
			}
			else if(mass==90){
				if(!GetCut(variable[3], 65.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1], 10.0,  70.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3], 75.0, 120.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4],  0.0, 250.0,pass_vis       [i])){continue;}
			}
			else if(mass==115){
				if(!GetCut(variable[3], 90.0, 300.0,pass_pre       [i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [i])){continue;}
				if(!GetCut(variable[1], 10.0,  75.0,pass_ptll      [i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [i])){continue;}
				if(!GetCut(variable[3],100.0, 145.0,pass_rec       [i])){continue;}
				if(!GetCut(variable[4],  0.0, 250.0,pass_vis       [i])){continue;}
			}
			else{
				ShowMessage(2,"no such input mass! ","");
				return;

			}


			pass[i]++;
		}

		pass_basic_muon[filenum]+=pass_basic_muon[i]*weight[i];
		pass_pre       [filenum]+=pass_pre       [i]*weight[i];
		pass_mll       [filenum]+=pass_mll       [i]*weight[i];
		pass_ptll      [filenum]+=pass_ptll      [i]*weight[i];
		pass_mis_theta [filenum]+=pass_mis_theta [i]*weight[i];
		pass_rec       [filenum]+=pass_rec       [i]*weight[i];
		pass_vis       [filenum]+=pass_vis       [i]*weight[i];
		pass           [filenum]+=pass           [i]*weight[i];
		nevent         [filenum]+=nevent         [i]*weight[i];

		ShowMessage(2,filename                      [i], ""                 );
		ShowMessage(3,"weight                         ", weight          [i]);
		ShowMessage(3,"no~cut                         ", nevent          [i]);
		ShowMessage(3,"2~ISO~\\mu                      ", pass_basic_muon [i]);
		ShowMessage(3,"precut~\\mu                     ", pass_pre        [i]);
		ShowMessage(3,latex_name                    [0], pass_mll        [i]);
		ShowMessage(3,latex_name                    [1], pass_ptll       [i]);
		ShowMessage(3,latex_name                    [2], pass_mis_theta  [i]);
		//ShowMessage(3,"BDT                            ", pass_BDT        [i]);
		ShowMessage(3,latex_name                    [3], pass_rec        [i]);
		ShowMessage(3,latex_name                    [4], pass_vis        [i]);
		ShowMessage(3,"all~cut                        ", pass            [i]);

	}

	ShowMessage(2,"total result"                   , ""                       );
	ShowMessage(3,arg_string3                      , 0                        );
	ShowMessage(3,"no~cut                         ", nevent          [filenum]);
	ShowMessage(3,"2~ISO~\\mu                      ", pass_basic_muon [filenum]);
	ShowMessage(3,"precut~\\mu                     ", pass_pre        [filenum]);
	ShowMessage(3,latex_name                    [0], pass_mll        [filenum]);
	ShowMessage(3,latex_name                    [1], pass_ptll       [filenum]);
	ShowMessage(3,latex_name                    [2], pass_mis_theta  [filenum]);
	//ShowMessage(3,"BDT                            ", pass_BDT        [filenum]);
	ShowMessage(3,latex_name                    [3], pass_rec        [filenum]);
	ShowMessage(3,latex_name                    [4], pass_vis        [filenum]);
	ShowMessage(3,"all~cut                        ", pass            [filenum]);

	//plot
	int k=0;
	for(unsigned int j=0;j<title_name.size();j++){
		if(plot_switch[j]){
			c1.push_back(new TCanvas(c1_name[j].c_str()," ",c1_width[j],c1_height[j]));
			if(log_switch[j]){
				gPad->SetLogy();
			}


			THStack *s = new THStack();
			for(unsigned int i=0;i<filenum+1;i++){
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
			std::string pngname="../plot_"+mass_str+"/"+title_name[j]+outroot_name+"_"+mass_str+".png";
			img->WriteImage(pngname.c_str());
			k++;

		}
	}

	myfile.close();

	otfile->cd();
	datatrain->Write();
	otfile->Close();
	return;
}
