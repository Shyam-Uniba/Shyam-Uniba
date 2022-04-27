///////////// code for checking the basic things works in ALICE and also in PANDA of particles by shyam kumar
#include "TGraphErrors.h"
#include "TF1.h"
#include "TRandom.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TMath.h"
void read_digi_QA()
{
   gStyle->SetPalette(1);
   gStyle->SetOptTitle(0);
   gStyle->SetTitleOffset(.85,"X");gStyle->SetTitleOffset(.85,"Y");
   gStyle->SetTitleSize(.05,"X");gStyle->SetTitleSize(.05,"Y");
   gStyle->SetLabelSize(.05,"X");gStyle->SetLabelSize(.05,"Y");
   gStyle->SetHistLineWidth(2);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(1);
  
     
///////////////////////////////////////////////Reading the root file
 TFile *f = TFile::Open("../../../macro/digi_tpc.root");
 TTree *t = (TTree*)f->Get("ceesim");
 TDirectory *dir = (TDirectory*)f->Get("CeeTpcDriftTask");

  
   TCanvas* c1 = new TCanvas("Simple plot1", "Simulation", 1400, 1000);
   TCanvas* c2 = new TCanvas("Simple plot2", "Simulation", 1400, 1000);
   TCanvas* c3 = new TCanvas("Simple plot3", "Simulation", 1400, 1000);
   TCanvas* c4 = new TCanvas("Simple plot4", "Simulation", 1400, 1000);
   TCanvas* c5 = new TCanvas("Simple plot5", "Simulation", 1400, 1000);
   TCanvas* c6 = new TCanvas("Simple plot6", "Simulation", 1400, 1000);
   TCanvas* c7 = new TCanvas("Simple plot7", "Simulation", 1400, 1000);
   TCanvas* c8 = new TCanvas("Simple plot8", "Simulation", 1400, 1000);
   
   c1->SetMargin(0.1,0.12,0.1,0.05);
   c2->SetMargin(0.1,0.12,0.1,0.05);
   c3->SetMargin(0.13,0.03,0.1,0.05);
   c4->SetMargin(0.13,0.03,0.1,0.05);
   c5->SetMargin(0.1,0.03,0.1,0.05);
   c6->SetMargin(0.1,0.03,0.1,0.05);
   c7->SetMargin(0.1,0.03,0.1,0.05);
   c8->SetMargin(0.1,0.03,0.1,0.05);

   TH2D *Points =(TH2D*)dir->Get("xz");
   TH2D *hxDrift_vs_DriftLength =(TH2D*)dir->Get("xDrift_vs_DriftLength");
   TH1D *hxShifts =(TH1D*)dir->Get("xShifts");
   TH1D *hzShifts =(TH1D*)dir->Get("zShifts");
   TH1D *htime =(TH1D*)dir->Get("Time Variation");
   TH1D *hXZ_vsdl =(TH1D*)dir->Get("XZ_vsdl");
   TH2D *hXZ_Charge =(TH2D*)dir->Get("XZ_Charge");
   TH2D *hYZ_Charge =(TH2D*)dir->Get("YZ_Charge");

    Points->GetXaxis()->SetTitle("dZ (cm)");
    Points->GetYaxis()->SetTitle("dX (cm)");
    Points->GetXaxis()->CenterTitle();
    Points->GetYaxis()->CenterTitle();
    c1->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    Points->Draw("colz");
    c1->SaveAs("XZdrfit.pdf");

    hxDrift_vs_DriftLength->GetXaxis()->SetTitle("dX (cm)");
    hxDrift_vs_DriftLength->GetYaxis()->SetTitle("drift-length (cm)");
    hxDrift_vs_DriftLength->GetYaxis()->SetRangeUser(0.,100.);
    hxDrift_vs_DriftLength->GetXaxis()->CenterTitle();
    hxDrift_vs_DriftLength->GetYaxis()->CenterTitle();
    c2->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    hxDrift_vs_DriftLength->Draw("colz");
    c2->SaveAs("hxDrift_vs_DriftLength.pdf");

    hxShifts->GetXaxis()->SetTitle("dX (cm)");
    hxShifts->GetYaxis()->SetTitle("Entries (a.u.)");
    hxShifts->GetXaxis()->CenterTitle();
    hxShifts->GetYaxis()->CenterTitle();
    c3->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    hxShifts->Draw("colz");
    c3->SaveAs("hxShifts.pdf");

    hzShifts->GetXaxis()->SetTitle("dZ (cm)");
    hzShifts->GetYaxis()->SetTitle("Entries (a.u.)");
    hzShifts->GetXaxis()->CenterTitle();
    hzShifts->GetYaxis()->CenterTitle();
    c4->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    hzShifts->Draw("colz");
    c4->SaveAs("hzShifts.pdf");

    htime->GetXaxis()->SetTitle("dt (ns)");
    htime->GetYaxis()->SetTitle("Entries (a.u.)");
    htime->GetXaxis()->CenterTitle();
    htime->GetYaxis()->CenterTitle();
    c5->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    htime->Draw("colz");
    c5->SaveAs("htime.pdf");

    hXZ_vsdl->GetXaxis()->SetTitle("dX (cm)");
    hXZ_vsdl->GetYaxis()->SetTitle("dZ (cm)");
    hXZ_vsdl->GetZaxis()->SetTitle("drift-length (cm)");
    hXZ_vsdl->GetZaxis()->SetRangeUser(0.,100.);
    hXZ_vsdl->SetTitleOffset(1.60,"XYZ");
    hXZ_vsdl->GetXaxis()->CenterTitle();
    hXZ_vsdl->GetYaxis()->CenterTitle();
    hXZ_vsdl->GetZaxis()->CenterTitle();
    c6->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    hXZ_vsdl->Draw("");
    c6->SaveAs("hXZ_vsdl.pdf");

    hXZ_Charge->GetXaxis()->SetTitle("Z [cm]");
    hXZ_Charge->GetYaxis()->SetTitle("X [cm]");
    hXZ_Charge->GetXaxis()->CenterTitle();
    hXZ_Charge->GetYaxis()->CenterTitle();
    c7->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    hXZ_Charge->Draw("");
    c7->SaveAs("hxz_charge.pdf");

    hYZ_Charge->GetXaxis()->SetTitle("Z [cm]");
    hYZ_Charge->GetYaxis()->SetTitle("X [cm]");
    hYZ_Charge->GetXaxis()->CenterTitle();
    hYZ_Charge->GetYaxis()->CenterTitle();
    c8->cd();
   // Points->GetXaxis()->SetRangeUser(-0.8,0.8);
    hYZ_Charge->Draw("");
    c8->SaveAs("hyz_charge.pdf");

}
