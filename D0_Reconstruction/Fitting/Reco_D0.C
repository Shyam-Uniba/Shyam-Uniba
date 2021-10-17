#include "TH1.h"
#include "TNtuple.h"
#include "TSystem.h"
#include "TFile.h"
#include "TMath.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TRandom.h"
#include <iostream>
#include <string>
#include <vector>

Bool_t ImpactParameterSelection(Int_t dcax, Int_t dcay, Int_t dcaz, Double_t d0cut);
// https://www.star.bnl.gov/public/comp/simu/newsite/gstar/Manual/particle_id.html
 const Double_t pi = TMath::Pi();
 const Double_t mPi = 0.1350; // 0.135 GeV/c^2
 const Double_t mK = 0.4937; // GeV/c^2

 void Reco_D0()
 {  

	       gStyle->SetTitleSize(0.04, "XY");
        gStyle->SetTitleOffset(1.1, "XY");
        gStyle->SetLegendBorderSize(0);
        gStyle->SetPadTickX(1);
        gStyle->SetPadTickY(1);
        gStyle->SetOptStat(0.);	
        gStyle->SetOptFit(0);
        gStyle->SetOptTitle(0);
        gStyle->SetPalette(55);

  TFile *f = TFile::Open("D0_Analysis_AllSi_vbd_0.05_0.55_0.24_GEM_res_50.0um_DIRC_12_sect_B_ATHENA_210507_FastSimEval.root");
  TTree *t = (TTree*)f->Get("D0Analysis"); 

  TH1D *h_InvMass = new TH1D("h_InvMass","D^{0} Mass Spectra",100,1.72,2.05);
  h_InvMass->GetXaxis()->SetTitle("m_{D^{0}}");
  h_InvMass->GetYaxis()->SetTitle("Entries (a.u.)");
  h_InvMass->GetXaxis()->CenterTitle();
  h_InvMass->GetYaxis()->CenterTitle();
  
  //Analysis Variables............

  const Int_t k=100000;
  
  Int_t Ntracks =0;
  Double_t px[k], py[k], pz[k], pcax[k], pcay[k], pcaz[k];
  Int_t trackID[k];

  t->SetBranchAddress ("tracks",&Ntracks);  
  t->SetBranchAddress ("px",&px);  
  t->SetBranchAddress ("py", &py);                   
  t->SetBranchAddress ("pz", &pz);           
  t->SetBranchAddress ("pcax", &pcax);
  t->SetBranchAddress ("pcay", &pcay);
  t->SetBranchAddress ("pcaz", &pcaz);

      
  Int_t n = (Int_t)t->GetEntries();
  cout << "Number of Events" << n << endl;

   for(Int_t i = 0; i < n; i++) //Event loop start
  {
      t->GetEntry(i);
      
      for(Int_t j=0 ; j <Ntracks; j++) // Assume first Track K-
	   { 

    if (trackID[i]<0) continue; // Skip fake tracks
    Double_t E_Kminus1 = sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]+mK*mK); // Asssume Kaon+
    Double_t E_PionMinus1 = sqrt(px[i]*px[i]+py[i]*py[i]+pz[i]*pz[i]+mPi*mPi); // Asssume Pion-
    
    Bool_t check_d0_track1 = ImpactParameterSelection(pcax[i],pcay[i],pcaz[i],300.0e-4);
    if (!check_d0_track1) continue;
         
      for(Int_t k=0 ; k < Ntracks; k++) //Assume first Track Pi+
	   {
	     
     if (trackID[i]<0) continue; // Skip fake tracks
     if (i==j) continue; // Same Tracks

     Bool_t check_d0_track2 = ImpactParameterSelection(pcax[j],pcay[j],pcaz[j],100.0e-4);
     if (!check_d0_track2) continue;

     // Sum d02 cut
      Double_t sumd02_cut = 10000.e-8; 
      Double_t sum_d02_1 = (pcax[i]*pcax[i]+pcax[j]*pcax[j]);
      if (sum_d02_1>sumd02_cut) continue; // sum d02cut
  
     Double_t E_PiPlus1 = sqrt(px[j]*px[j]+py[j]*py[j]+pz[j]*pz[j]+mPi*mPi); // Assume Pion+
     Double_t E_KPlus2 = sqrt(px[j]*px[j]+py[j]*py[j]+pz[j]*pz[j]+mK*mK); // Assume Kaon+

     Double_t mD0 = sqrt((E_Kminus1+E_PiPlus1)*(E_Kminus1+E_PiPlus1) -((px[i]+px[j])*(px[i]+px[j])+(py[i]+py[j])*(py[i]+py[j])+(pz[i]+pz[j])*(pz[i]+pz[j]))); // K-Pi+
     Double_t mD0bar = sqrt((E_KPlus2+E_PionMinus1)*(E_KPlus2+E_PionMinus1) -((px[i]+px[j])*(px[i]+px[j])+(py[i]+py[j])*(py[i]+py[j])+(pz[i]+pz[j])*(pz[i]+pz[j]))); // // K+Pi-

     h_InvMass->Fill(mD0);
     h_InvMass->Fill(mD0bar);
    } 	          
	 
	}//Particle loop ends
      
 }// event loop ends

  TFile *fout = new TFile("D0_invmass.root","recreate");
  fout->cd();
  h_InvMass->SetMarkerStyle(20);
  h_InvMass->SetMarkerSize(1.3);
  h_InvMass->SetDrawOption("EP");
  h_InvMass->Write();
  fout->Close();
  delete fout;
}

 Bool_t ImpactParameterSelection(Int_t dcax, Int_t dcay, Int_t dcaz, Double_t d0cut)
 {
 Double_t d0 = sqrt(dcax*dcax+dcay*dcay+dcaz*dcaz);
 if (d0<d0cut) return kTRUE;
 return kFALSE;
 }
