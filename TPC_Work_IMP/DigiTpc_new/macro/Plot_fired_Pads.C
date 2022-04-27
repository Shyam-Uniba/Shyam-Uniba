#include <iostream>
#include <vector>
#include <sstream>
#include "TLatex.h"
#include "TCanvas.h"
#include "TVector2.h"
#include "TArrow.h"
#include "TWbox.h"
#include "TPad.h"
#include "TH2D.h"
#include "TStyle.h"

void Plot_fired_Pads(){

 Bool_t debug = false;
 TString PadPlanefile ="PadCoordinate.txt"; // PadPlane

 CeeTpcPadPlane* PadPlane= new CeeTpcPadPlane(PadPlanefile);

  TCanvas* c=new TCanvas("c","regions",1400,1000);
  c->SetMargin(0.09, 0.03 ,0.1,0.05);

  gPad->Range(0,0,120,80); // x0, z0, x1, z1

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
 // gStyle->SetTickLength(0.0,"xy");
 // gStyle->SetLabelSize(0.0,"xy");
 // gStyle->SetFrameLineWidth(0);
 // gStyle->SetAxisColor(kWhite,"xy");
  TFile* fsim = new TFile("../../../macro/test.root");
  TTree *tsim=(TTree*)fsim->Get("ceesim") ;
  TH2D *hxzmc= new TH2D("hxzmc","MC Points",1240,-620,620,1000,-500,500);
  tsim->Draw("CeeTpcPoint.fZ*10:CeeTpcPoint.fX*10>> hxzmc","","");
  hxzmc->SetMarkerStyle(31);
  hxzmc->SetMarkerColor(kBlue);
  hxzmc->GetXaxis()->SetTitle("Z [cm]");
  hxzmc->GetYaxis()->SetTitle("X [cm]");
  hxzmc->GetXaxis()->CenterTitle();
  hxzmc->GetYaxis()->CenterTitle();


  TFile* fdigi = new TFile("../../../macro/digi_tpc.root");
  TTree *tdigi=(TTree*)fdigi->Get("ceesim") ;

  TClonesArray *Av=new TClonesArray("CeeTpcAvalanche");
  tdigi->SetBranchAddress("CeeTpcAvalanche",&Av);  

  TClonesArray *sa=new TClonesArray("CeeTpcSignal");
  tdigi->SetBranchAddress("CeeTpcSignal",&sa);
 
  TH2D* hist_MC_Pad = new TH2D("hist_MC_Pad","",1240,-620,620,1000,-500,500);
  hist_MC_Pad->Draw();
  hist_MC_Pad->GetXaxis()->SetTitle("X [mm]");
  hist_MC_Pad->GetXaxis()->CenterTitle();
  hist_MC_Pad->GetYaxis()->SetTitle("Z [mm]");
  hist_MC_Pad->GetYaxis()->CenterTitle();
  hist_MC_Pad->GetYaxis()->SetTitleOffset(1.0);
  CeeTpcPad* apad=0;
  for (Int_t j=0; j< tdigi->GetEntriesFast(); j++)	{
    tdigi->GetEntry(j);
    for (Int_t i=0; i<sa->GetEntriesFast(); i++){
      CeeTpcSignal *sig=(CeeTpcSignal*)sa->At(i);
      try{
      apad=PadPlane->GetPad(sig->padId());
      }
     catch (std::exception &e){
      cout << e.what() << endl;
      cout.flush();
      continue;
    }
    int color = 2;
    apad->Draw(color); // Put color number.
    c->Modified();
    //c->Update();
    }
  }
  c->cd();
  hxzmc->Draw("same");
  c->SaveAs("padplane_digi.pdf");
  c->SaveAs("padplane_digi.root");

  TCanvas* cyz=new TCanvas("cyz","regions",1400,1000);
  cyz->SetMargin(0.09, 0.03 ,0.1,0.05);
  TH2D *hyzmc= new TH2D("hyzmc","MC Points",1240,-620,620,1000,-500,500);
  tsim->Draw("CeeTpcPoint.fY*10:CeeTpcPoint.fZ*10>> hyzmc","","");
  hyzmc->SetMarkerStyle(31);
  hyzmc->SetMarkerColor(kBlue);
  hyzmc->GetXaxis()->SetTitle("Z [cm]");
  hyzmc->GetYaxis()->SetTitle("Y [cm]");
  hyzmc->GetYaxis()->SetTitleOffset(1.20);
  hyzmc->GetXaxis()->CenterTitle();
  hyzmc->GetYaxis()->CenterTitle();
  cyz->cd();
  hyzmc->Draw("same");

  TCanvas* cAv=new TCanvas("cAv","Avalanche",1400,1000);
  cAv->SetMargin(0.09, 0.095 ,0.1,0.05);
  cAv->SetLogz();
  TH2D *hxzAv= new TH2D("hxzAv","Avalanche Points",1240,-620,620,1000,-500,500);
  tdigi->Draw("CeeTpcAvalanche.fz*10:CeeTpcAvalanche.fx*10>> hxzAv","","colz");
  hxzAv->GetXaxis()->SetTitle("Avalanche X [mm]");
  hxzAv->GetYaxis()->SetTitle("Avalanche Z [mm]");
  hxzAv->GetYaxis()->SetTitleOffset(1.20);
  hxzAv->GetXaxis()->CenterTitle();
  hxzAv->GetYaxis()->CenterTitle(); 
  cAv->cd();
  hxzAv->Draw("colz");
}


