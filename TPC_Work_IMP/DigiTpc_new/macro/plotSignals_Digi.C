#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TH2D.h"
#include "TStyle.h"

void plotSignals_Digi(){

  //gStyle->SetFillColor(kWhite);
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(1);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.04,"XY");gStyle->SetTitleSize(.04,"t");
   gStyle->SetLabelSize(.04,"XY");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0); 


   TString PadPlanefile ="PadCoordinate.txt"; // PadPlane
   CeeTpcPadPlane* PadPlane= new CeeTpcPadPlane(PadPlanefile);

  TFile* file = new TFile("../../../macro/digi_tpc.root");
  //TGeoManager *geoMan = (TGeoManager*) file->Get("FAIRGeom");
  TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
  c1->SetMargin(0.08,0.1,0.08,0.07);
  TCanvas* c2 = new TCanvas("Simple plot2", "Simulation", 1500, 1000);
  c2->SetMargin(0.08,0.1,0.08,0.07);
 
  TTree *t=(TTree*)file->Get("ceesim") ;
  
  TClonesArray *sa=new TClonesArray("CeeTpcDigi");
  t->SetBranchAddress("CeeTpcDigi",&sa);

  TH2F *xz=new TH2F("xz","Hit Pads (X-Z (cm))",240,-60.,60., 100,-50.,50.);
  TH2F *tz =new TH2F("tz","Hit Pads (TimeBin vs Z (cm))",100,-45.,45.,200,0.,200.);
  xz->GetXaxis()->SetTitle("X [cm]");
  xz->GetYaxis()->SetTitle("Z [cm]");
  xz->GetXaxis()->CenterTitle();
  xz->GetYaxis()->CenterTitle();
  tz->GetXaxis()->SetTitle("Z [cm]");
  tz->GetYaxis()->SetTitle("TimeBin");
  tz->GetXaxis()->CenterTitle();
  tz->GetYaxis()->CenterTitle();

  for (Int_t j=0; j< t->GetEntriesFast(); j++)	{
    t->GetEntry(j);
    for (Int_t i=0; i<sa->GetEntriesFast(); i++){
      CeeTpcDigi *digi=(CeeTpcDigi*)sa->At(i);
      CeeTpcPad *apad = PadPlane->GetPad(digi->padId());
      TVector2 gCent = apad->GetGlobalCenterPosition(); 
      int timebin = digi->GetTimeBin(); // 100 ns timestep (course time bin)

      xz->Fill(gCent.X()*0.1+0.0001, gCent.Y()*0.1+0.0001,digi->GetAdc());
      tz->Fill(gCent.Y()*0.1+0.0001,timebin+1.0/2,digi->GetAdc());
    }
  }
  c1->cd();
  xz->Draw("colz");
  c2->cd();
  tz->Draw("colz");
  c1->SaveAs("Pad_Plane_digi_XZ.pdf");
  c2->SaveAs("Pad_Plane_digi_tZ.pdf");
}


