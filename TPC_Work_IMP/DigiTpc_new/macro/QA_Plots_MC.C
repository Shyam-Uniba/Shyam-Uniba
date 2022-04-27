///////////// code for checking the basic things works in ALICE and also in PANDA of particles by shyam kumar

#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"

void QA_Plots_MC()
   {

////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
   gStyle->SetLabelSize(.05,"X");gStyle->SetLabelSize(.05,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
     
///////////////////////////////////////////////Reading the root file
 TFile *f = TFile::Open("/home/shyam/GSI/CeeRoot/macro/test.root");
 TFile *f1 = TFile::Open("/home/shyam/GSI/CeeRoot/macro/hit_tpc.root");
 TTree *sim = (TTree*)f->Get("ceesim"); 
 TTree *sim1 = (TTree*)f1->Get("ceesim"); 
  
 TCanvas * c1 = new TCanvas("c1","coutput",1400,1000);
 c1->SetMargin(0.09, 0.03 ,0.1,0.05);

 TCanvas * c2 = new TCanvas("c2","coutput",1400,1000);
 c2->SetMargin(0.09, 0.03 ,0.1,0.05);

TH2D *hxzmc= new TH2D("hxzmc","MC Points",1000,-50,50,900,-45,45);
TH2D *hxzhit= new TH2D("hxzhit","Hit Points",1000,-50,50,900,-45,45);

TH2D *hxymc= new TH2D("hxymc","MC Points",1200,-60,60,1000,-50,50);
TH2D *hxyhit= new TH2D("hxyhit","Hit Points",1200,-60,60,1000,-50,50);

//////////////////////////////////////////////////////////////////////
c1->cd();
sim->Draw("CeeTpcPoint.fY:CeeTpcPoint.fZ>> hxzmc","","");
sim1->Draw("CeeTpcHit.fY:CeeTpcHit.fZ>> hxzhit","","");
hxzmc->SetMarkerStyle(31);
hxzmc->SetMarkerColor(kBlue);
hxzmc->GetXaxis()->SetTitle("Z [cm]");
hxzmc->GetYaxis()->SetTitle("Y [cm]");
hxzmc->GetXaxis()->CenterTitle();
hxzmc->GetYaxis()->CenterTitle();
hxzmc->Draw();
hxzhit->SetMarkerStyle(20);
hxzhit->SetMarkerColor(kMagenta);
hxzhit->Draw("same");

 TLegend *l= new TLegend(0.20,0.80,0.40,0.90);
 l->SetTextSize(0.04);
 l->SetBorderSize(0);
 l->AddEntry(hxzmc,"MC Point");
 l->AddEntry(hxzhit,"Hit Position");
 c1->cd();
 l->Draw();

c2->cd();
sim->Draw("CeeTpcPoint.fZ:CeeTpcPoint.fX>> hxymc","","");
sim1->Draw("CeeTpcHit.fZ:CeeTpcHit.fX>> hxyhit","","");
hxymc->SetMarkerStyle(31);
hxymc->SetMarkerColor(kBlue);
hxymc->GetXaxis()->SetTitle("X [cm]");
hxymc->GetYaxis()->SetTitle("Z [cm]");
hxymc->GetXaxis()->CenterTitle();
hxymc->GetYaxis()->CenterTitle();
hxymc->Draw();
hxyhit->SetMarkerStyle(20);
hxyhit->SetMarkerColor(kMagenta);
hxyhit->Draw("same");

 c2->cd();
 l->Draw();
 c1->SaveAs("mcyz.png");
 c2->SaveAs("mcxy.png");

}
