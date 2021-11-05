/* Macro for fitting the DPlus Invariant Mass */
//Modified by Shyam Kumar
#include <Riostream.h>
#include <TStyle.h>
#include <TFile.h>
#include <TList.h>
#include <TDirectoryFile.h>
#include "THnSparse.h"
#include <TClonesArray.h>
#include <TH1F.h>
#include <TCanvas.h>
#include "AliHFMassFitter.h"
#include "AliHFMassFitter.cc"
#include <fstream>
#include <cmath>
//-----------------Function---------------------------------------------
void fitInvMassNew(Int_t rebin=0,TString listname="coutputDplusLoose0100",Int_t nsigma=2, TString pathin="./",TString pathout="./",Int_t btype=0)
{
 TCanvas *c2 = new TCanvas("c2", "Fitted Mass Spectra of DPlus-Tight Cuts",1400,1000);
	c2->SetMargin(0.11,0.04,0.12,0.05);
 //------------Root Style----------------------------------------
 gStyle->SetOptTitle(0);
 gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
	gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
	gStyle->SetLabelSize(.02,"X");gStyle->SetLabelSize(.02,"Y");
	gStyle->SetHistLineWidth(2);
	gStyle->SetOptFit(1);
 gStyle->SetOptFit(0111);
 gStyle->SetOptStat("nemroui");
 // -----   Timer   --------------------------------------------------------
	 TStopwatch timer;
	 timer.Start();
	  
  //--------------Reading the file--------------------
  TFile* f = TFile::Open("D0_invmass.root");
   
  
   //--------------Mass fitter-------------------------
  AliHFMassFitter *fitter=new AliHFMassFitter();
  cout<<"mean = "<<fitter->GetMean()<<"\tsigma= "<<fitter->GetSigma()<<endl;
  Bool_t init=kTRUE;
   Int_t i=0;
  //-------------------D Plus Invariant Mass for Tight Cuts-----------------------
 
    TH1F *hMass=(TH1F*)f->Get("h_InvMass"); 
    hMass->SetTitle("Inv Mass of D^{0}");
    hMass->GetXaxis()->SetTitle("m_{D^{0}} (GeV/c^{2})");
    hMass-> SetLabelSize(0.05,"X");
    hMass-> SetLabelSize(0.05,"Y");
   // hMass-> SetTitleFont (62,"X");
  //  hMass-> SetTitleFont (62,"X");
    hMass->SetTitleSize (0.05, "X");
    hMass->SetTitleSize (0.05, "Y");
    hMass->SetTitleOffset (1.0, "X");
    hMass->SetTitleOffset (1.1, "Y");
    hMass-> GetXaxis()-> CenterTitle();
    hMass->GetYaxis()->SetTitle("Entries");
    hMass-> GetYaxis() -> CenterTitle();
    hMass->SetStats(false);
    hMass->SetMarkerStyle (20);
    hMass->Rebin(4);
    //---------------fitting-----------------------------
    Double_t signal=0.,errsignal=0.,background=0.,errbackground=0.;;
    Double_t significance3sigma=0.,err3=0.;
    fitter->SetHisto(hMass);
    fitter->SetType(btype,0);
    fitter->SetInitialGaussianMean(1.864);
    fitter->SetInitialGaussianSigma(0.011);
    fitter->SetRangeFit(1.70,2.05);
    fitter->MassFitter(0);
    fitter->Signal(2,signal,errsignal);
    fitter->Background(2,background,errbackground);
    fitter->Significance(3,significance3sigma,err3);
    c2->cd();
    fitter->DrawHere(gPad,3.0,1); //Remove fit Info write 0 in place of 1
    //fitter->DrawFit(3);

  // -----  Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
	  Double_t ctime = timer.CpuTime();
	 cout << endl << endl;
	 cout << "Macro finished succesfully." << endl;
	 cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	  cout << endl;	

}	  
