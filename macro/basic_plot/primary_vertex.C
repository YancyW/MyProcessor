#include <iostream>
#include<TROOT.h>
#include<TFile.h>
#include<TTree.h>
#include<TCanvas.h>
#include<TStyle.h>
#include<TMath.h>
#include<TH2F.h>
#include<TProfile2D.h>
#include "TGraph.h"
#include "TGraphErrors.h"
#include<TLegend.h>

//void draw_ptra(const char* gamma.root)

void primary_vertex()
{
  //////////////////////////////////////////////////////////
  //   This file has been automatically generated 
  //     (Thu Feb  4 15:07:08 2016 by ROOT version5.28/00f)
  //   from TTree MyLCTuple/columnwise ntuple with LCIO data
  //   found on file: mysgv_1-0.1.root
  //////////////////////////////////////////////////////////


  //Reset ROOT and connect tree file
  gStyle->SetOptStat(1111);
  gROOT->Reset();
  int i;
  i=11;
 TCanvas *c1=new TCanvas("c1"," ",1000,800);
 c1->SetGrid(); 
 //c1->SetLogx();
 Float_t x1[11]={2,3,4,5,6,7,8,9,10,11,12};
 // Float_t y1[11]={53.88,47.73,37.13,31.39,25.8,23.31,22.1,18.05,17.1,15.88,13.84};
 Float_t y1[11]={22.106,9.509,8.570,5.553,4.424,3.126,2.239,1.431,0.964,0.624,0.394};
 //Float_t ey[11]={0.2564,0.3464,0.28374,0.2980,0.2743,0.2947,0.3303,0.3375,0.3894,0.4494,0.4929};

 //TGraphErrors *gr1 = new TGraphErrors(i,x1,y1,0,ey);
TGraph *gr1 = new TGraph(i,x1,y1);
 gr1->SetMarkerColor(4);
 gr1->SetMarkerStyle(23); 
  gr1->SetMarkerSize(1.5); 
 gr1->Draw("AP"); 
}
