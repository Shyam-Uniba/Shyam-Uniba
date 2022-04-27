///////////// code for checking the basic things works in ALICE and also in PANDA of particles by shyam kumar

#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
void read_digi_plots()
   {

////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
   gStyle->SetLabelSize(.05,"X");gStyle->SetLabelSize(.05,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
     
///////////////////////////////////////////////Reading the root file
 TFile *f = TFile::Open("digi_tpc.root");
 TTree *t = (TTree*)f->Get("ceesim");
 TDirectory *dir = (TDirectory*)f->Get("CeeTpcDriftTask");

  
   TCanvas* c1 = new TCanvas("Simple plot", "Simulation", 1100, 1000);
   c1->SetMargin(0.12,0.12,0.1,0.05);
   c1->cd();
   TH2D *Points =(TH2D*)dir->Get("xz");
// Points->SetMarkerColor(kBlue);
// Rectangular structure using Polyline
   TH2Poly *h2p = new TH2Poly();
   h2p->SetName("RectangularPads");
   h2p->SetTitle("Rectangular Pads");
   Int_t i,j;
   Int_t nx = 8;
   Int_t ny = 20;
   Double_t xval1,yval1,xval2,yval2;
   Double_t dx=0.2, dy=0.1;
   xval1 = -0.7;  
   xval2 = xval1+dx;  
   for (i = 0; i<nx; i++) {
      yval1 = -0.7;
      yval2 = dy;
      
      for (j = 0; j<ny; j++) {
         h2p->AddBin(xval1, yval1, xval2, yval2);
         yval1+=0.10;
         yval2=yval1+0.10;
      }
         xval1+=0.20;
         xval2=xval1+0.20; 
   }
    Points->GetXaxis()->SetTitle("Z-direction");
    Points->GetYaxis()->SetTitle("X-direction");
    Int_t nbins = h2p->GetNumberOfBins();
    c1->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    Points->Draw("colz");
    h2p->Draw("same");

}
