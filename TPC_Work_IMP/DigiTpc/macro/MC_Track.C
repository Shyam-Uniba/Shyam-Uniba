///////////// code for checking the basic things works in ALICE and also in PANDA of particles by shyam kumar

#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"

void MC_Track()
   {

////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.04,"X");gStyle->SetTitleSize(.04,"Y");
   gStyle->SetLabelSize(.04,"X");gStyle->SetLabelSize(.04,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
 gStyle->SetOptStat(111111);
     
///////////////////////////////////////////////Reading the root file
 TFile *f = TFile::Open("../../../macro/test.root");
 TTree *sim = (TTree*)f->Get("ceesim"); 
  
 TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
 c1->SetMargin(0.08,0.1,0.08,0.07);
 TCanvas* c2 = new TCanvas("Simple plot2", "Simulation", 1500, 1000);
 c2->SetMargin(0.08,0.1,0.08,0.07);

TH1D *Pdg = new TH1D("Pdg","Pdg code of Particles",2300,0.,2300);
TH1D *Mom = new TH1D("Mom","Momentum of Track",200,0,1);

//////////////////////////////////////////////////////////////
c1->cd();
sim->Draw("MCTrack.fPdgCode>> Pdg","MCTrack.fMotherId<0");
Pdg->GetXaxis()->SetTitle("Pdgcode");
Pdg->GetYaxis()->SetTitle("Entries (a.u.)");
c1->SaveAs("~/Desktop/Main_Plots/Pdg_code_Prim.pdf");
c2->cd();
sim->Draw("TMath::Sqrt(MCTrack.fPx*MCTrack.fPx+MCTrack.fPy*MCTrack.fPy+MCTrack.fPz*MCTrack.fPz) >> Mom"," MCTrack.fMotherId<0");
Mom->GetXaxis()->SetTitle("Momentum (GeV/c)");
Mom->GetYaxis()->SetTitle("Entries (a.u.)");
c2->SaveAs("~/Desktop/Main_Plots/Momentum_prim.pdf");
}
