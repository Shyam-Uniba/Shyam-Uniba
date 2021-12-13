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
 const Double_t mp = 0.9383; // GeV/c^2

 void Reco_Lambda0()
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

  TH1D *h_InvMass = new TH1D("h_InvMass","#Lambda^{0} Mass Spectra",20,1.1,1.12);
  h_InvMass->GetXaxis()->SetTitle("m_{#Lambda^{0}}");
  h_InvMass->GetYaxis()->SetTitle("Entries (a.u.)");
  h_InvMass->GetXaxis()->CenterTitle();
  h_InvMass->GetYaxis()->CenterTitle();
  
  //Analysis Variables............

  const Int_t k=100000;
  
  Int_t Ntracks =0;
  Double_t px[k], py[k], pz[k], pcax[k], pcay[k], pcaz[k];
  Int_t trackID[k], charge[k];

  t->SetBranchAddress ("tracks",&Ntracks);  
  t->SetBranchAddress ("px",&px);  
  t->SetBranchAddress ("py", &py);                   
  t->SetBranchAddress ("pz", &pz);           
  t->SetBranchAddress ("pcax", &pcax);
  t->SetBranchAddress ("pcay", &pcay);
  t->SetBranchAddress ("pcaz", &pcaz);
  t->SetBranchAddress ("charge", &charge);

 
  TVector3 Mom_Track1, Mom_Track2, Mom_Sum;

  Int_t n = (Int_t)t->GetEntries();
  cout << "Number of Events" << n << endl;

   for(Int_t iEvent = 0; iEvent < n; iEvent++) //Event loop start
  {
      t->GetEntry(iEvent);
      
      for(Int_t i=0 ; i <Ntracks; i++) // Assume first Track K-
	   { 

    if (trackID[i]<0) continue; // Skip fake tracks
    
    Mom_Track1.SetXYZ(px[i],py[i],pz[i]);
 
    Double_t E_Proton1 = sqrt(Mom_Track1.Mag2()+mp*mp);       // Asssume Proton
    Double_t E_PiPlus = sqrt(Mom_Track1.Mag2()+mPi*mPi); // Asssume Pion+
    
    Bool_t check_d0_track1 = ImpactParameterSelection(pcax[i],pcay[i],pcaz[i],300.0e-4);
    if (!check_d0_track1) continue;
         
      for(Int_t j=0 ; j < Ntracks; j++) //Assume first Track Pi+
	   {	     

     if (trackID[i]<0) continue; // Skip fake tracks
     if (i==j) continue; // Same Tracks

     Mom_Track2.SetXYZ(px[j],py[j],pz[j]);
     Mom_Sum = Mom_Track1+Mom_Track2;

  //  Bool_t check_d0_track2 = ImpactParameterSelection(pcax[j],pcay[j],pcaz[j],100.0e-4);
   // if (!check_d0_track2) continue;
    
     Double_t E_Proton2 = sqrt(Mom_Track2.Mag2()+mp*mp); // Assume Proton-
     Double_t E_PiMinus = sqrt(Mom_Track2.Mag2()+mPi*mPi); // Assume Pion-


     if (charge[i]>0 && charge[j]<0)
     {
     Double_t mD0 = sqrt((E_Proton1+E_PiMinus)*(E_Proton1+E_PiMinus)-Mom_Sum.Mag2()); // proton+Pi-
     h_InvMass->Fill(mD0);
     }
     
     if (charge[j]>0 && charge[i]<0)
     { 
     Double_t mD0bar = sqrt((E_Proton2+E_PiPlus)*(E_Proton2+E_PiPlus)-Mom_Sum.Mag2()); // antiproton+Pi+
     h_InvMass->Fill(mD0bar);
     } 	          
	 
	}//Particle loop j
      
 }//Particle loop i

 }//Event loop i

  TFile *fout = new TFile("Lambda0_invmass.root","recreate");
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
 if (d0>d0cut) return kTRUE;
 return kFALSE;
 }
