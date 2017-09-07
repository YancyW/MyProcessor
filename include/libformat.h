#pragma once
#include <iostream>
#include <string> 
#include <iomanip> 
#include "TStyle.h"

template <typename TMess1, typename TMess2,typename TMess3> 
void ShowMessage(Int_t level, TMess1 mess, TMess2 ddata, TMess3 ddata_comm){
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
	int  fillnum;

	switch(level){
		case 1:fill=fill1;fillnum=fillnum1;break;
		case 2:fill=fill2;fillnum=fillnum2;break;
		case 3:fill=fill3;fillnum=fillnum3;break;
		case 4:fill=fill4;fillnum=fillnum4;break;
		case 5:fill=fill5;fillnum=fillnum5;break;
		case 6:fill=fill6;fillnum=fillnum6;break;
		default:fill=fill1;
	}

		std::cout << fill << std::setw(65-fillnum) << std::setfill(' ') << mess  << ":    " << ddata << "" << ddata_comm << std::endl;
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
	int  fillnum;

	switch(level){
		case 1:fill=fill1;fillnum=fillnum1;break;
		case 2:fill=fill2;fillnum=fillnum2;break;
		case 3:fill=fill3;fillnum=fillnum3;break;
		case 4:fill=fill4;fillnum=fillnum4;break;
		case 5:fill=fill5;fillnum=fillnum5;break;
		case 6:fill=fill6;fillnum=fillnum6;break;
		default:fill=fill1;
	}

		std::cout << fill << std::setw(65-fillnum) << std::setfill(' ') << mess  << ":    " << ddata << std::endl;
}



void ShowMessage (Int_t level, TString mess);
void ShowMessage (Int_t level);
void ShowMessage ();
