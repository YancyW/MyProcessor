#include "Fbasic.h"
TString Int_to_String(int i){
	TString fnum;
	std::stringstream num;

	num << i;
	num >> fnum;
	return(fnum);
}

TString Int_to_String_descrip(int i){
	TString fnum;
	TString fdescrip[6]= {"leading","second","third","fourth","fifth","sixth"};

	fnum = fdescrip[i];
	return(fnum);
}

TString NameHistogram(TString signal_name, TString class_name, TString observable_name, int i){

	TString Final_Name;
	TString fnum;

	fnum=Int_to_String(i);
	Final_Name=signal_name+"_"+class_name+"_"+observable_name+"_"+fnum;

	return(Final_Name);
}

TString NameDescrip(TString class_name, TString observable_name, int i){

	TString Final_Name;
	TString desnum;

	desnum=Int_to_String_descrip(i);
	Final_Name=desnum+"_"+class_name+"_"+observable_name;

	return(Final_Name);
}

TString GetUnit(TString observe_name){
	TString unit_name;
	if(observe_name == "pt"){
		unit_name="GeV";
	}
	else if(observe_name == "mass"){
		unit_name="";
	}
	else{unit_name="";}

	return(unit_name);
}



void CountNumber(Int_t variable, Int_t tot_num, Int_t slot,TString  Message){
	if(tot_num>slot*100){
		if(variable%slot == 0 && variable!=0){
			ShowMessage(2,Message,variable);
		}
	}
	else{
		if(variable%(tot_num/100) == 0 && variable!=0){
			ShowMessage(2,Message,variable/(tot_num/100),"%");
		}
	}
	
}
