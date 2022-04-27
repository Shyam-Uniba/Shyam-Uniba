///////////// code for checking the basic things works in ALICE and also in PANDA of particles by shyam kumar

#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"

void Plot_MCPoints()
   {

////////////////////////////////////////////////////// style of the plot
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(1.0,"XYZ");
   gStyle->SetTitleSize(.05,"XYZ");
   gStyle->SetLabelSize(.05,"XYZ");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
     
///////////////////////////////////////////////Reading the root file
 TFile *f = TFile::Open("../../../macro/test.root");
 TTree *t = (TTree*)f->Get("ceesim"); 
 TClonesArray *tpc_array=new TClonesArray("CeeTpcPoint");
 t->SetBranchAddress("CeeTpcPoint",&tpc_array);//Branch name
 Int_t nEntries =  t->GetEntriesFast(); // Number of events in the tree

  
 TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1500, 1000);
 c1->Divide(2,2);
 c1->SetMargin(0.12,0.12,0.1,0.05);


 TH2F* hyz=new TH2F("hyz","Y-Z Points",75,-45.,45.,160,-40.,40.); // binwidth = 1.2 cm
 hyz->GetXaxis()->SetTitle("Z [cm]");
 hyz->GetYaxis()->SetTitle("Y [cm]");
 hyz->GetXaxis()->CenterTitle();
 hyz->GetYaxis()->CenterTitle();
 hyz->SetMarkerStyle(20);
 hyz->SetMarkerSize(0.7);

 TH2F* hxz=new TH2F("hxz","X-Z Points",75,-45.,45.,240,-60.,60.);
 hxz->GetXaxis()->SetTitle("Z [cm]");
 hxz->GetYaxis()->SetTitle("X [cm]");
 hxz->GetXaxis()->CenterTitle();
 hxz->GetYaxis()->CenterTitle();
 hxz->SetMarkerStyle(20);
 hxz->SetMarkerSize(0.7);

 TH2F* hxy=new TH2F("hxy","X-Y Points",240,-60.,60.,160,-40.,40.);
 hxy->GetXaxis()->SetTitle("X [cm]");
 hxy->GetYaxis()->SetTitle("Y [cm]");
 hxy->GetXaxis()->CenterTitle();
 hxy->GetYaxis()->CenterTitle();
 hxy->SetMarkerStyle(20);
 hxy->SetMarkerSize(0.7);

 TH3F* hxyz=new TH3F("hxyz","X-Y-Z Points",75,-45.,45.,240,-60.,60.,160,-40.,40.);
 hxyz->GetXaxis()->SetTitle("Z [cm]");
 hxyz->GetYaxis()->SetTitle("X [cm]");
 hxyz->GetZaxis()->SetTitle("Y [cm]");
 hxyz->GetXaxis()->CenterTitle();
 hxyz->GetYaxis()->CenterTitle();
 hxyz->GetZaxis()->CenterTitle();
 hxyz->SetMarkerStyle(20);
 hxyz->SetMarkerSize(0.7);

  CeeTpcPoint* tpcpoint;
  Double_t x, y, z;
  

  for (Int_t j=0; j< nEntries; j++)
	{
		t->GetEntry(j);
		//--------------------Start detector Entries--------------------
		for (Int_t k=0; k<tpc_array->GetEntriesFast(); k++) // tpc_array->GetEntriesFast()
		{ 
			tpcpoint = (CeeTpcPoint*)tpc_array->At(k);
                        if (tpcpoint->GetMotherId()>=0) continue;
                            x = tpcpoint->GetX();
                            y = tpcpoint->GetY();
                            z = tpcpoint->GetZ();
                            hyz->Fill(z,y);
                            hxy->Fill(x,y);
                            hxz->Fill(z,x);
                            hxyz->Fill(z,x,y);
			
		}
	      	             
       }
    c1->cd(1);
    gPad->SetMargin(0.12,0.02,0.1,0.05);
    hyz->Draw("");
    c1->cd(2);
    gPad->SetMargin(0.12,0.02,0.1,0.05);
    hxz->Draw("");
    c1->cd(3);
    gPad->SetMargin(0.12,0.02,0.1,0.05);
    hxy->Draw("");
    c1->cd(4);
    gPad->SetMargin(0.12,0.02,0.1,0.05);
    hxyz->SetTitleOffset(1.40,"XY");
    hxyz->Draw("");
    c1->SaveAs("MC_Points_Sim.pdf");

}
