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

void drawPad_fromPadId(int PadId, double xAv, double zAv){ // Pass Argruments of PadId, X and Z Position of Avalanche
	
 TString PadPlanefile ="PadCoordinate.txt"; // PadPlane
 Bool_t drawPadIDs = kTRUE;
 std::vector<Double_t> gPosX, gPosZ;
 std::vector<Double_t> lPosX, lPosZ;

 CeeTpcPadPlane* PadPlane= new CeeTpcPadPlane(PadPlanefile);

  TCanvas* c=new TCanvas("c","regions",1400,1000);
  c->SetMargin(0.09, 0.03 ,0.1,0.05);

  gPad->Range(0,0,120,80); // x0, z0, x1, z1

  gStyle->SetOptStat(0);

  Double_t histXcent, histYcent;
 
  TH2D* hist = new TH2D("Pads","",1240,-620,620,1000,-500,500);
  hist->Draw();
  hist->GetXaxis()->SetTitle("X [mm]");
  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->SetTitle("Z [mm]");
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleOffset(1.0);
  // plot regions
  for(int i=PadId;i<PadId+1;++i){
    if (i==-1) continue;	
    CeeTpcPad* apad=0;
    try{
      apad=PadPlane->GetPad(i);
    }
    catch (std::exception &e){
      cout << e.what() << endl;
      cout.flush();
      continue;
    }
    int color = 2;
    apad->Draw(color); // Put color number.    
   
    TVector2 gCent = apad->GetGlobalCenterPosition();
    histXcent = gCent.X();
    histYcent = gCent.Y();
    gPosX = apad->GetGlobalXCoordinates();
    gPosZ = apad->GetGlobalZCoordinates();
    lPosX = apad->GetLocalXCoordinates();
    lPosZ = apad->GetLocalZCoordinates();

    if(drawPadIDs) {     
      TLatex* tex = new TLatex();
      tex->SetTextSize(0.02);
      tex->SetTextColor(kBlue);
      tex->SetTextAlign(22);      
      tex->DrawLatex(gCent.X(), gCent.Y(), Form("(%1.2f, %1.2f, %d, %d, %d, %d)",gCent.X()*0.1, gCent.Y()*0.1, i,apad->GetSectorId(),apad->GetRowId(),apad->GetPadInRowId())); // Global Center coordinates and Pad Id

      TLatex* x0_z0 = new TLatex();
      x0_z0->SetTextSize(0.02);
      x0_z0->SetTextColor(kBlue);
      x0_z0->SetTextAlign(22);      
      x0_z0->DrawLatex(gPosX[0], gPosZ[0], Form("(gx_{0},gz_{0}) =(%1.2f, %1.2f)",gPosX[0]*0.1, gPosZ[0]*0.1));

      TLatex* x1_z1 = new TLatex();
      x1_z1->SetTextSize(0.02);
      x1_z1->SetTextColor(kBlue);
      x1_z1->SetTextAlign(22);      
      x1_z1->DrawLatex(gPosX[1], gPosZ[1], Form("(gx_{1},gz_{1}) =(%1.2f, %1.2f)",gPosX[1]*0.1, gPosZ[1]*0.1));

      TLatex* x2_z2 = new TLatex();
      x2_z2->SetTextSize(0.02);
      x2_z2->SetTextColor(kBlue);
      x2_z2->SetTextAlign(22);      
      x2_z2->DrawLatex(gPosX[2], gPosZ[2], Form("(gx_{2},gz_{2}) =(%1.2f, %1.2f)",gPosX[2]*0.1, gPosZ[2]*0.1));

      TLatex* x3_z3 = new TLatex();
      x3_z3->SetTextSize(0.02);
      x3_z3->SetTextColor(kBlue);
      x3_z3->SetTextAlign(22);      
      x3_z3->DrawLatex(gPosX[3], gPosZ[3], Form("(gx_{3},gz_{3}) =(%1.2f, %1.2f)",gPosX[3]*0.1, gPosZ[3]*0.1));

      TPaveText* t1=new TPaveText(0.11,0.88,0.80,0.95,"NDC");
      t1->SetFillStyle(0);
      t1->SetBorderSize(0);
      t1->SetTextSize(0.025);
      t1->SetTextFont(42);
      t1->SetTextAlign(13);
      t1->AddText(0.,0., Form("(lx_{0},lz_{0}) =(%1.2f, %1.2f) \t (lx_{1},lz_{1}) =(%1.2f, %1.2f) \t (lx_{2},lz_{2}) =(%1.2f, %1.2f) \t (lx_{3},lz_{3}) =(%1.2f, %1.2f) ",lPosX[0]*0.1, lPosZ[0]*0.1, lPosX[1]*0.1, lPosZ[1]*0.1, lPosX[2]*0.1, lPosZ[2]*0.1,  lPosX[3]*0.1, lPosZ[3]*0.1)); 
      t1->SetTextColor(kMagenta);
      t1->Draw();
    }

  }
    TLatex* Avalanche = new TLatex();
    Avalanche->SetTextSize(0.02);
    Avalanche->SetTextColor(kMagenta);
    Avalanche->SetTextAlign(22);      
    Avalanche->DrawLatex(xAv*10+5, zAv*10, Form("Av = (%1.4f, %1.4f)",xAv, zAv));
    hist->SetMarkerStyle(31);
    hist->SetMarkerColor(kMagenta);
    hist->SetMarkerSize(0.0);
    hist->Fill(xAv*10,zAv*10);
    hist->GetXaxis()->SetRangeUser(histXcent-15,histXcent+15);
    hist->GetYaxis()->SetRangeUser(histYcent-15,histYcent+15);
    c->Modified();
    c->Update();

   TF2 *f2 = new TF2("bigaus","bigaus",xAv*10-1.0,xAv*10+1.0,zAv*10-1.0,zAv*10+1.0);
   f2->SetParameters(1,xAv*10,0.28,zAv*10,0.28);
   f2->SetLineColor(kRed);
   f2->Draw("same");
  
}
