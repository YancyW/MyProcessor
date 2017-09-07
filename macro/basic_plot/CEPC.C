#include "TLatex.h"

void CEPC() {

   TMultiGraph *mg = new TMultiGraph();
   mg->SetTitle(";r; #Psi(r)");

   const Int_t n = 13;
   Double_t xmin,ymin,Brb,Brg,Brc,Brb2,Brc2,Br,Br2,Br3,b,g,c,j,jb,jbc;
	xmin=0.1;

   Double_t x[n], y[n];

   for (Int_t i=0;i<n;i++) {
     x[i] = xmin+i*0.1;
//ee bveto
     y[i] = -1.14545*x[i]*x[i]*x[i]*x[i]+ 4.43336*x[i]*x[i]*x[i]-6.21044*x[i]*x[i]+4.13013*x[i]-0.334601;

     printf(" i %i %f %f \n",i,x[i],y[i]);
   }   
   TGraph *gr1 = new TGraph(n,x,y);
   gr1->SetLineWidth(3);
   gr1->SetLineStyle(7);
   mg->Add(gr1);


   for (Int_t i=0;i<n;i++) {
     x[i] = xmin+i*0.1;
//ee without veto
     y[i] = -0.889209*x[i]*x[i]*x[i]*x[i]+ 3.80913*x[i]*x[i]*x[i]-5.96984*x[i]*x[i]+4.30335*x[i]-0.332217;
     printf(" i %i %f %f \n",i,x[i],y[i]);
   }   
   TGraph *gr2 = new TGraph(n,x,y);
   gr2->SetLineWidth(3);
   gr2->SetLineStyle(1);
   mg->Add(gr2);


   mg->Draw("ac"); 
   mg->GetXaxis()->SetLimits(0.1,1.3);
   mg->GetXaxis()->SetTitleSize(0.09);
   mg->GetXaxis()->SetLabelSize(0.06);
   mg->GetXaxis()->SetNdivisions(210);
   mg->GetXaxis()->SetTitleOffset(0.8);
   mg->GetXaxis()->CenterTitle();

   mg->GetYaxis()->SetRangeUser(0.0,1.0);
   mg->GetYaxis()->SetTitleSize(0.07);
   mg->GetYaxis()->SetLabelSize(0.06);
   mg->GetYaxis()->SetTitleOffset(0.7);
   //mg->GetYaxis()->SetNdivisions(710);
   mg->GetYaxis()->CenterTitle();
   mg->GetXaxis()->SetDecimals();
   mg->GetYaxis()->SetDecimals();
   mg->GetYaxis()->SetTickLength(0.02);
   gPad->Modified();

   leg = new TLegend(0.785,0.195,0.945,0.385);
   leg->SetMargin(0.3);
   leg->AddEntry(gr2,"HZ","l");
   leg->AddEntry(gr1,"t #bar{t}","l");
   leg->Draw();


   gPad->SetTickx();
   gPad->SetTicky();
   gPad->SetLeftMargin(0.11);
   gPad->SetRightMargin(0.035);
   gPad->SetTopMargin(0.045);
   gPad->SetBottomMargin(0.16);

   // TCanvas::Update() draws the frame, after which one can change it
  /* c1->Update();
   c1->GetFrame()->SetFillColor(21);
   c1->GetFrame()->SetBorderSize(12);
   c1->Modified();*/
}
