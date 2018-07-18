#ifndef DEBUG_H
#define DEBUG_H
#include "TStyle.h"
#include "local-yaml-cpp-0.5.1/include/yaml-cpp/yaml.h"
#include <iostream>
#include <fstream>
#include "libformat.h"
//#include "RWbasic.h"
class CDebug{
	private:
	public:
		bool mode;

		Int_t    num_debug_mode;
		Int_t    level;
		std::vector<Bool_t> debug;
		void Read_Debug(){
			ShowMessage(3, "read debug");
			this->mode= false;
			this->num_debug_mode=1000;
			this->level=5;
			this->debug.resize(this->num_debug_mode);
			this->debug[1] =true;
			this->debug[2] =false;
			this->debug[11]=false;
			this->debug[12]=false;
			this->debug[13]=false;
			this->debug[15]=false;
			this->debug[21]=false;
			this->debug[23]=false;
			this->debug[25]=false;
			this->debug[31]=false;
			this->debug[32]=false;
			this->debug[33]=false;
			this->debug[34]=false;
			this->debug[35]=false;
			this->debug[41]=false;
			this->debug[42]=false;
			this->debug[50]=false;
			this->debug[51]= true;
			this->debug[52]= true;
			this->debug[53]=false;
			this->debug[900] = true;
			this->debug[901] = true;
			this->debug[902] = true;
			this->debug[903] =false;
			//= YAML::LoadFile("./control/debug.dat");

			//RW_element("Debug_mode"                 ,debug_node,this->mode );
			//RW_element("Debug_mode_num"             ,debug_node,this->num_debug_mode  );
			//RW_element("Debug_level"                ,debug_node,this->level);

			ShowMessage(4);

			//	for(int i=0;i<this->num_debug_mode;i++){
			//	RW_vector_element("Debug"       , i, debug_node,  this->debug);
			//	}
		}

		template <typename TDebug1, typename TDebug2, typename TDebug3> 
			void Message(Int_t itlevel, Int_t itpos, TString mess, TDebug1 ddata, TDebug2 ddata2, TDebug3  ddata3){
				if(!mode){return;}
				if(itpos>=num_debug_mode) {std::cout<< "wrong itpos number in debug message" << std::endl;}
				if (itlevel<level && debug[itpos])	std::cout << "debugging : " << mess  << "   " << ddata << "  " << ddata2 << " "<< ddata3 << std::endl;
			}
		template <typename TDebug1, typename TDebug2> 
			void Message(Int_t itlevel, Int_t itpos, TString mess, TDebug1 ddata, TDebug2 ddata2){
				if(!mode){return;}
				if(itpos>=num_debug_mode) {std::cout<< "wrong itpos number in debug message" << std::endl;}
				if (itlevel<level && debug[itpos])	std::cout << "debugging : " << mess  << "   " << ddata << "  " << ddata2 << std::endl;
			}
		template <typename TDebug> 
			void Message(Int_t itlevel, Int_t itpos, TString mess, TDebug ddata){
				if(!mode){return;}
				if(itpos>=num_debug_mode) {std::cout<< "wrong itpos number in debug message" << std::endl;}
				if (itlevel<level && debug[itpos])	std::cout << "debugging : " << mess  << "   " << ddata << std::endl;
			}
		void Message(Int_t itlevel, Int_t itpos,TString mess){
			if(!mode){return;}
			if(itpos>=num_debug_mode) {std::cout<< "wrong itpos number in debug message" << std::endl;}
			if(itlevel<level && debug[itpos]) std::cout << "debugging:  " << mess  << std::endl;
		}
};


#endif
