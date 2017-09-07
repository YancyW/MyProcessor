#include "libformat.h"
//------------------------------------------------------------------------------
//

//

void ShowMessage(Int_t level, TString mess){
	std::string fill1="";
	std::string fill2="     ";
	std::string fill3="          ";
	std::string fill4="               ";
	std::string fill5="                    ";
	std::string fill6="                         ";
	std::string fill;
	std::string fillter;

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

	int size = mess.Sizeof();
	int fillsize= (90 - fillnum - size)/2; 
	if(fillsize%2==1){
		fillter=" *";
	}
	else{
		fillter=" ";
	}
	std::cout << fill << std::setw(fillsize) << std::setfill('*') << "" << " " << mess << fillter << std::setw(fillsize) << std::setfill('*') << "" << std::endl;
}

void ShowMessage(){
	std::cout << "" << std::endl;
}

void ShowMessage(Int_t level){
	std::cout << "" << std::endl;

}
