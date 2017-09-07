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

void analysis_cut(std::vector<std::vector<std::string> > arg_string1,
		                      std::vector<std::string>   arg_string3, 
				  std::vector<std::vector<std::string> > arg_string2,
				  float mass) {
	//(array for file name with different polarization , out put root name/and name in the table, array for cross section , output name,  higgs_mass){
//void analysis_cut() {

//	float mass=10;
  //  std::vector<std::vector<std::string> > arg_string1={ { "/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/4f_zz_l_eL.pR.root" , "/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/4f_zz_l_eR.pL.root"  } , { "/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/4f_zz_sl_eL.pR.root" , "/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/4f_zz_sl_eR.pL.root"  } , { "/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/nh_10_eL.pR.root" , "/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/nh_10_eR.pL.root"  }  };
//	{{"/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/4f_ww_l_eL.pR.root","/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/4f_ww_l_eR.pL.root"},{"/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/nh_10_eL.pR.root","/afs/desy.de/user/y/ywang/Code/MyTest2/bin/../results_10/nh_10_eR.pL.root"}};
//	std::vector<std::string >arg_string3={ "4f_{zz}^{l}" , "4f_{zz}^{sl}" , "nh_{10}^{10}"  };
//	std::vector<std::vector<std::string> >arg_string2={ { "157.96041" , "99.506102"  } , { "1422.1429" , "713.52633"  } , { "79.026852" , "50.415099"  }  };

	//{{"0.1","0.2"},{"0.4","0.3"}};
//	std::string out_name="out.dat"; 
	float lumi=250;




	int filenum=arg_string1.size();
	if(arg_string1.size()!=arg_string2.size()){
		std::cout << "Error in analysis: input arguments1 size not equal to argument3!" << std::endl;
		return;
	}

	std::vector<int>          filenum_i;
	for(int i=0;i<filenum;i++){
		if(arg_string1[i].size()!=arg_string2[i].size()){
			std::cout << "Error in analysis: input arguments1 size not equal to argument3 in i th array: " <<  i<< std::endl;
			return;
		}
		filenum_i.push_back(arg_string2[i].size());
	}


	//pol number doesn't rely on how many input file
	//pol array sequense is lr rl ll rr,  input file should be the same sequense

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





	std::vector<std::vector<float> >       xection         ;
	std::vector<std::vector<std::string> > filename        ;
	std::vector<std::vector<TFile*> >      in_file         ;
	std::vector<std::vector<int> >         nevent          ;
	std::vector<std::vector<float> >       weight          ;
	std::vector<std::vector<TTree*> >      MyLCTuple       ;
	std::vector<std::vector<float> >       pass_basic_muon ;
	std::vector<std::vector<float> >       pass_mll        ;
	std::vector<std::vector<float> >       pass_ptll       ;
	std::vector<std::vector<float> >       pass_mis_theta  ;
	std::vector<std::vector<float> >       pass_rec        ;
	std::vector<std::vector<float> >       pass_vis        ;
	std::vector<std::vector<float> >       pass            ;
	std::vector<std::vector<float> >       pass_pre        ;

	std::vector<float >       xection_i         ;
	std::vector<std::string>  filename_i        ;
	std::vector<TFile*>       in_file_i         ;
	std::vector<int>          nevent_i          ;
	std::vector<float>        weight_i          ;
	std::vector<TTree*>       MyLCTuple_i       ;
	std::vector<float>        pass_basic_muon_i ;
	std::vector<float>        pass_mll_i        ;
	std::vector<float>        pass_ptll_i       ;
	std::vector<float>        pass_mis_theta_i  ;
	std::vector<float>        pass_rec_i        ;
	std::vector<float>        pass_vis_i        ;
	std::vector<float>        pass_i            ;
	std::vector<float>        pass_pre_i        ;

	for(int i=0;i<filenum;i++){
		for(int j=0;j<filenum_i[i];j++){
			xection_i                .push_back(String_To_Num<float>(arg_string2[i][j]));
			filename_i               .push_back(arg_string1[i][j]                      );
			in_file_i                .push_back(new TFile(filename_i[j].c_str())       );
			MyLCTuple_i              .push_back((TTree*)in_file_i[j]->Get("datatrain") );
			nevent_i                 .push_back(MyLCTuple_i[j]->GetEntries()           );
			weight_i                 .push_back(xection_i[j]/nevent_i[j]*pol[j]        );
			pass_basic_muon_i        .push_back(0.0                                    );
			pass_mll_i               .push_back(0.0                                    );
			pass_ptll_i              .push_back(0.0                                    );
			pass_mis_theta_i         .push_back(0.0                                    );
			pass_rec_i               .push_back(0.0                                    );
			pass_vis_i               .push_back(0.0                                    );
			pass_i                   .push_back(0.0                                    );
			pass_pre_i               .push_back(0.0                                    );
		}
		filename_i               .push_back("total");
		nevent_i                 .push_back(0.0    );
		pass_basic_muon_i        .push_back(0.0    );
		pass_mll_i               .push_back(0.0    );
		pass_ptll_i              .push_back(0.0    );
		pass_mis_theta_i         .push_back(0.0    );
		pass_rec_i               .push_back(0.0    );
		pass_vis_i               .push_back(0.0    );
		pass_i                   .push_back(0.0    );
		pass_pre_i               .push_back(0.0    );

		xection                .push_back(xection_i        );
		filename               .push_back(filename_i       );
		in_file                .push_back(in_file_i        );
		MyLCTuple              .push_back(MyLCTuple_i      );
		nevent                 .push_back(nevent_i         );
		weight                 .push_back(weight_i         );
		pass_basic_muon        .push_back(pass_basic_muon_i);
		pass_mll               .push_back(pass_mll_i       );
		pass_ptll              .push_back(pass_ptll_i      );
		pass_mis_theta         .push_back(pass_mis_theta_i );
		pass_rec               .push_back(pass_rec_i       );
		pass_vis               .push_back(pass_vis_i       );
		pass                   .push_back(pass_i           );
		pass_pre               .push_back(pass_pre_i       );

		xection_i                .clear();
		filename_i               .clear();
		MyLCTuple_i              .clear();
		in_file_i                .clear();
		nevent_i                 .clear();
		weight_i                 .clear();
		pass_basic_muon_i        .clear();
		pass_mll_i               .clear();
		pass_ptll_i              .clear();
		pass_mis_theta_i         .clear();
		pass_rec_i               .clear();
		pass_vis_i               .clear();
		pass_i                   .clear();
		pass_pre_i               .clear();
	}






	// the vector --- for variable 
	std::vector<float>    variable;
	std::vector<string>   title_name;
	std::vector<string>   latex_name;

	std::vector<string>   aux_fname;
	std::vector<string>   aux_iname;
	std::vector<float>    aux_f;
	std::vector<Int_t>    aux_i;

	//setting for obseravables
	//
	//variables that do not want to plot
	title_name    .push_back("po_muon_kcut_zmass"                   );
	latex_name    .push_back("M_{l^{+}l^{-}}                 "      );


	title_name    .push_back("po_muon_kcut_zpt"                     );
	latex_name    .push_back("P_T^{l^{+}l^{-}}               "      );

	title_name    .push_back("po_muon_kcut_invis_costheta"          );
	latex_name    .push_back("cos\\theta_{missing}            "     );

	title_name    .push_back("po_muon_kcut_recoil_mass"             );
	latex_name    .push_back("M_{rec}                        "      );

	title_name    .push_back("po_muon_kcut_vis_e"                   );
	latex_name    .push_back("E_{vis}                        "      );

	title_name    .push_back("po_muon_lep_pair_mass"                );
	latex_name    .push_back("M_{l^{+}l^{-}}                 "      );

	title_name    .push_back("po_muon_lep_pair_costheta"            );
	latex_name    .push_back("cos\\theta_{Z}                 "      );

	title_name    .push_back("po_muon_lep_pair_costheta_pair"       );
	latex_name    .push_back("cos\\theta_{lep}               "      );


	title_name    .push_back("po_muon_lep_pair_costheta_track1"     );
	latex_name    .push_back("cos\\theta_{track,1}           "      );


	title_name    .push_back("po_muon_lep_pair_costheta_track2"     );
	latex_name    .push_back("cos\\theta_{track,2}           "      );


	variable.resize(title_name.size());


	//variables that do not want to plot
	//aux_0
	aux_iname     .push_back("po_muon_num"  			            );
	aux_iname     .push_back("po_muon_num_plus" 			        );
	aux_iname     .push_back("po_muon_num_minus"				    );
	aux_i.resize(aux_iname.size());


	//aux_0
	aux_f.resize(aux_fname.size());



	std::string mass_str=Num_To_String(mass);
	//loop for all input files
	for(unsigned int cnum=0;cnum<filenum;cnum++){
		std::string outroot_name=change_name(arg_string3[cnum]);
		std::string out_name="../data_"+mass_str+"/"+outroot_name+"_"+mass_str+".dat";
		myfile.open(out_name.c_str());
		ShowMessage(1,"filenum", "");
		for(unsigned int i=0;i<filenum_i[cnum];i++){



			//loop for init variables which are prepared for plot 
			for(unsigned int j=0;j<title_name.size();j++){
				MyLCTuple[cnum][i]->SetBranchAddress(title_name[j].c_str(), &variable[j]);
			}

			//loop for init variables which are used in calculation 
			for(unsigned int j=0;j<aux_iname.size();j++){
				MyLCTuple[cnum][i]->SetBranchAddress(aux_iname[j].c_str(), &aux_i[j]);
			}

			//loop for init variables which are used in calculation 
			for(unsigned int j=0;j<aux_fname.size();j++){
				MyLCTuple[cnum][i]->SetBranchAddress(aux_fname[j].c_str(), &aux_f[j]);
			}

			//loop for reading variables and setting cuts 
			for(int event=0;event<nevent[cnum][i];event++){
				MyLCTuple[cnum][i]->GetEntry(event);

				// add cuts
				if(!GetCut(aux_i[0]   ,    2,   100                         )){continue;}
				if(!GetCut(aux_i[1]   ,    1,   100                         )){continue;}
				if(!GetCut(aux_i[2]   ,    1,   100,pass_basic_muon[cnum][i])){continue;}


				if(!GetCut(variable[3], 90.0, 300.0,pass_pre       [cnum][i])){continue;}
				if(!GetCut(variable[0], 73.0, 120.0,pass_mll       [cnum][i])){continue;}
				if(!GetCut(variable[1], 10.0,  75.0,pass_ptll      [cnum][i])){continue;}
				if(!GetCut(variable[2],-0.98,  0.98,pass_mis_theta [cnum][i])){continue;}
				if(!GetCut(variable[3],100.0, 145.0,pass_rec       [cnum][i])){continue;}
				if(!GetCut(variable[4],  0.0, 250.0,pass_vis       [cnum][i])){continue;}

				pass[cnum][i]++;
			}

			pass_basic_muon[cnum][filenum_i[cnum]]+=pass_basic_muon[cnum][i]*weight[cnum][i];
			pass_pre       [cnum][filenum_i[cnum]]+=pass_pre       [cnum][i]*weight[cnum][i];
			pass_mll       [cnum][filenum_i[cnum]]+=pass_mll       [cnum][i]*weight[cnum][i];
			pass_ptll      [cnum][filenum_i[cnum]]+=pass_ptll      [cnum][i]*weight[cnum][i];
			pass_mis_theta [cnum][filenum_i[cnum]]+=pass_mis_theta [cnum][i]*weight[cnum][i];
			pass_rec       [cnum][filenum_i[cnum]]+=pass_rec       [cnum][i]*weight[cnum][i];
			pass_vis       [cnum][filenum_i[cnum]]+=pass_vis       [cnum][i]*weight[cnum][i];
			pass           [cnum][filenum_i[cnum]]+=pass           [cnum][i]*weight[cnum][i];
			nevent         [cnum][filenum_i[cnum]]+=nevent         [cnum][i]*weight[cnum][i];

			ShowMessage(3,filename                      [cnum][i], ""                 );
			ShowMessage(4,"weight                         "      , weight          [cnum][i]);
			ShowMessage(4,"no~cut                         "      , nevent          [cnum][i]);
			ShowMessage(4,"2~ISO~\\mu                      "     , pass_basic_muon [cnum][i]);
			ShowMessage(4,"precut~\\mu                     "     , pass_pre        [cnum][i]);
			ShowMessage(4,latex_name                    [0]      , pass_mll        [cnum][i]);
			ShowMessage(4,latex_name                    [1]      , pass_ptll       [cnum][i]);
			ShowMessage(4,latex_name                    [2]      , pass_mis_theta  [cnum][i]);
			ShowMessage(4,latex_name                    [3]      , pass_rec        [cnum][i]);
			ShowMessage(4,latex_name                    [4]      , pass_vis        [cnum][i]);
			ShowMessage(4,"all~cut                        "      , pass            [cnum][i]);

		}

		ShowMessage(3,"total result"                    , ""                                    );
		ShowMessage(4,arg_string3                 [cnum], 0                                     );
		ShowMessage(4,"no~cut                         " , nevent         [cnum][filenum_i[cnum]]);
		ShowMessage(4,"2~ISO~\\mu                      ", pass_basic_muon[cnum][filenum_i[cnum]]);
		ShowMessage(4,"precut~\\mu                     ", pass_pre       [cnum][filenum_i[cnum]]);
		ShowMessage(4,latex_name                    [0] , pass_mll       [cnum][filenum_i[cnum]]);
		ShowMessage(4,latex_name                    [1] , pass_ptll      [cnum][filenum_i[cnum]]);
		ShowMessage(4,latex_name                    [2] , pass_mis_theta [cnum][filenum_i[cnum]]);
		ShowMessage(4,latex_name                    [3] , pass_rec       [cnum][filenum_i[cnum]]);
		ShowMessage(4,latex_name                    [4] , pass_vis       [cnum][filenum_i[cnum]]);
		ShowMessage(4,"all~cut                        " , pass           [cnum][filenum_i[cnum]]);
		myfile.close();
	}

	//plot

	return;
}
