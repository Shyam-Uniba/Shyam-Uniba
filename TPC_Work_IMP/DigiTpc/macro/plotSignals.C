#include "TFile.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TH2D.h"
#include "TStyle.h"

void plotSignals(){

  //gStyle->SetFillColor(kWhite);
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(1);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.04,"XY");gStyle->SetTitleSize(.04,"t");
   gStyle->SetLabelSize(.04,"XY");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0); 
  //TODO: Get these things from Database!!!
  CeeTpcGem* _gem=new CeeTpcGem(2000, 0.028, 14.9); // Gain and Spread sigmaxz = 0.028, sigmat=14.9 ns

  CeeTpcPadShapePool* _padShapes = new CeeTpcPadShapePool("Cee_Tpc_Pad_Shape.dat",
				   *_gem,
				   0.5, // lookup range
				   0.02, // Lookup Step
				   0.01); // LookupIntegrationStep


   CeeTpcPadPlane* _padPlane=new CeeTpcPadPlane("padplane_cee_tpc.dat", _padShapes);





  TFile* file = new TFile("../../../macro/digi_tpc.root");
  //TGeoManager *geoMan = (TGeoManager*) file->Get("FAIRGeom");
  TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
  c1->SetMargin(0.08,0.1,0.08,0.07);
  TCanvas* c2 = new TCanvas("Simple plot2", "Simulation", 1500, 1000);
  c2->SetMargin(0.08,0.1,0.08,0.07);
 
  TTree *t=(TTree*)file->Get("ceesim") ;
  
  TClonesArray *sa=new TClonesArray("CeeTpcSignal");
  t->SetBranchAddress("CeeTpcSignal",&sa);

  TH2F *xz=new TH2F("xz","Hit Pads (X-Z (cm))",75,-45.,45.,240,-60.,60.);
  TH2F *tz =new TH2F("tz","Hit Pads (TimeBin vs Z (cm))",75,-45.,45.,200,0.,200.);
  xz->GetXaxis()->SetTitle("Z [cm]");
  xz->GetYaxis()->SetTitle("X [cm]");
  xz->GetXaxis()->CenterTitle();
  xz->GetYaxis()->CenterTitle();
  tz->GetXaxis()->SetTitle("Z [cm]");
  tz->GetYaxis()->SetTitle("TimeBin");
  tz->GetXaxis()->CenterTitle();
  tz->GetYaxis()->CenterTitle();

  for (Int_t j=0; j< t->GetEntriesFast(); j++)	{
    t->GetEntry(j);
    for (Int_t i=0; i<sa->GetEntriesFast(); i++){
      CeeTpcSignal *sig=(CeeTpcSignal*)sa->At(i);
      double x,z;
      _padPlane->GetPadXZ(sig->padId(),x,z);
      int t = (sig->t())/100; // 100 ns timestep (course time bin)

      xz->Fill(z+1.2/2,x+0.5/2,sig->amp());
      tz->Fill(z+1.2/2,t+1.0/2,sig->amp());
    }
  }
  c1->cd();
  xz->Draw("colz");
  c2->cd();
  tz->Draw("colz");
  c1->SaveAs("Pad_Plane_signals_XZ.pdf");
  c2->SaveAs("Pad_Plane_signals_tZ.pdf");
}


