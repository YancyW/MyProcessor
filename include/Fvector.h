#pragma once
#ifndef FVECTOR_H
#define FVECTOR_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

template <typename TVector_add>
std::vector<TVector_add> operator + (std::vector<TVector_add> V1, std::vector<TVector_add> V2){
	std::vector <TVector_add> Vnew;

	Vnew.resize(V1.size()+V2.size());
	copy(V1.begin(),V1.end(),Vnew.begin());
	copy(V2.begin(),V2.end(),Vnew.begin()+V1.size());

//	stable_sort(Vnew.begin(),Vnew.end());
//	reverse(Vnew.begin(),Vnew.end());
	return(Vnew);
}



template <typename TVector_minus>
std::vector<TVector_minus> operator - (std::vector<TVector_minus> V1, std::vector<TVector_minus> V2){
	std::vector <TVector_minus> Vnew;
	if(V2.size()==0){
		return(V1);
	}
	if(V1.size()==0 ){
		return(Vnew);
	}


	std::vector<int> remove1;
	for(int i=0;i<V1.size();i++){
		for(int j=0;j<V2.size();j++){
			if(V1[i]==V2[j]){
				remove1.push_back(i);
				break;
			}
		}
	}

	stable_sort(remove1.begin(),remove1.end());

	bool	isminused=false;
	for(int i=0;i<V1.size();i++){
		for(int j=0;j<remove1.size();j++){
			if(i==remove1[j]){
				isminused=true;
				break;
			}
		}
		if(!isminused){
			Vnew.push_back(V1[i]);
		}
		isminused=false;
	}
	return(Vnew);
}
#endif
