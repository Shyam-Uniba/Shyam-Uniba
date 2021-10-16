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
#include <AliHFMassFitter.h>
#include <fstream>
#include <cmath>
//-----------------Function---------------------------------------------
void fitInvMassNew(Int_t rebin=0,TString listname="coutputDplusLoose0100",Int_t nsigma=2, TString pathin="./",TString pathout="./",Int_t btype=0)
{
 TCanvas *c2 = new TCanvas("c2", "Fitted Mass Spectra of DPlus-Tight Cuts",0,52,2000,1191);
 c2->Divide(2,2);
 //------------Root Style----------------------------------------
             gStyle->SetOptTitle(1);
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
    TString filename="AnalysisResults.root";
    TFile* f = new TFile(filename,"READ");
    if(!f->IsOpen()){
    cout<<"File "<<filename.Data()<<" not found"<<endl;
    return;
    }
  TString DirectoryoutputSE="PP_DplusHadCorr_SE_wTrkEff_wDkEff_PoolbyPool_Pass4SE";
  TString objectoutputSE="coutHistos_PP_DplusHadCorr_SE_wTrkEff_wDkEff_PoolbyPool_Pass4SE";
    //Fatching D+ Correlations task directory
    TDirectoryFile *DplusCorrdirectory = (TDirectoryFile*)f->Get(DirectoryoutputSE.Data());
    if(!DplusCorrdirectory){
        Printf("No Dplus Corr directory SE %s .. Exiting", DirectoryoutputSE.Data());
        return;
    }
    TList *DCorrlist = (TList*)DplusCorrdirectory->Get(objectoutputSE);
    if(!DCorrlist){
        Printf("No Dplus Corr list %s found, exiting... ", objectoutputSE.Data());
        return;
    }
    
  
   //--------------Mass fitter-------------------------
  AliHFMassFitter *fitter=new AliHFMassFitter();
  cout<<"mean = "<<fitter->GetMean()<<"\tsigma= "<<fitter->GetSigma()<<endl;
  Bool_t init=kTRUE;
   Int_t i=0;
  //-------------------D Plus Invariant Mass for Tight Cuts-----------------------
  const Int_t n=12; //Number of Histos
  for(Int_t i=3;i<7;i++){ //n
  
    THnSparseI *hsparse_DplusmassSE = (THnSparseI*)(DCorrlist->FindObject(Form("hnSparseM_Hdron_Data_Bin%d",i)));
    TH1F *hMassTC=(TH1F*)hsparse_DplusmassSE->Projection(0); //TC means Tight Cuts
    hMassTC->SetTitle(Form("Mass of D^{+}-Meson-Tight Cuts ( %d GeV/c < Pt <%d GeV/c ) ",i,i+1));
    hMassTC->GetXaxis()->SetTitle("Mass (GeV/c^{2})");
    hMassTC-> SetLabelSize(0.05,"X");
    hMassTC-> SetLabelSize(0.05,"Y");
   // hMassTC-> SetTitleFont (62,"X");
  //  hMassTC-> SetTitleFont (62,"X");
    hMassTC->SetTitleSize (0.05, "X");
    hMassTC->SetTitleSize (0.05, "Y");
    hMassTC->SetTitleOffset (1.0, "X");
    hMassTC->SetTitleOffset (1.1, "Y");
    hMassTC-> GetXaxis()-> CenterTitle();
    hMassTC->GetYaxis()->SetTitle("Entries");
    hMassTC-> GetYaxis() -> CenterTitle();
    hMassTC->SetStats(false);
    hMassTC->SetMarkerStyle (20);
    hMassTC->Rebin(4);
    //---------------fitting-----------------------------
    Double_t signal=0.,errsignal=0.,background=0.,errbackground=0.;
    Double_t significance1sigma[n]=0.,err1[n]=0.;
    Double_t significance3sigma[n]=0.,err3[n]=0.;
    Double_t significance6sigma[n]=0.,err6[n]=0.;
    fitter->SetHisto(hMassTC);
    fitter->SetType(btype,0);
    fitter->SetInitialGaussianMean(1.869);
    fitter->SetInitialGaussianSigma(0.011);
    fitter->SetRangeFit(1.70,2.05);
    fitter->MassFitter(0);
    fitter->Significance(1,significance1sigma[i],err1[i]);
    fitter->Significance(3,significance3sigma[i],err3[i]);
    fitter->Significance(6,significance6sigma[i],err6[i]);
    fitter->Signal(2,signal,errsignal);
    fitter->Background(2,background,errbackground);
    c2->cd(i-2);
    fitter->DrawHere(gPad,3.0,1); //Remove fit Info write 0 in place of 1
    //fitter->DrawFit(3);
}
c2->SaveAs("text.pdf");
c2->SaveAs("text.gif");
c2->SaveAs("text.root");
  // -----  Finish   -------------------------------------------------------
timer.Stop();
  Double_t rtime = timer.RealTime();
	  Double_t ctime = timer.CpuTime();
	 cout << endl << endl;
	 cout << "Macro finished succesfully." << endl;
	 cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
	  cout << endl;	

}	  
