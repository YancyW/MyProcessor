#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "libformat.h" 

TString Int_to_String(int i);
TString Int_to_String_descrip(int i);

TString NameHistogram(TString signal_name, TString class_name, TString observable_name, int i);
TString NameDescrip(TString class_name, TString observable_name, int i);

TString GetUnit(TString observe_name);

#define Get_VarName(x) #x

template <typename TTname>
TString Get_ClassName(TTname x){

	TString tname;
	tname = typeid(x).name();

	return(tname);
}


void CountNumber(Int_t variable, Int_t tot_num, Int_t slot,TString  Message);
