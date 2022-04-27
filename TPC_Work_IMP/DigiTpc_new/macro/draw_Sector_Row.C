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

void draw_Sector_Row(int SectorId, int RowId, double xAv, double zAv){ // Pass Argruments of PadId, X and Z Position of Avalanche
	
 TString PadPlanefile ="PadCoordinate.txt"; // PadPlane
 Bool_t drawPadIDs = kTRUE;
 std::vector<Double_t> gPosX, gPosZ;

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
  int npads=PadPlane->GetNPads(); // Total Pads
  for(int i=0;i<npads;++i){	
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
    if (apad->GetSectorId()!=SectorId || apad->GetRowId()>RowId) continue;
    apad->Draw(color); // Put color number.    
   
    TVector2 gCent = apad->GetGlobalCenterPosition();
    histXcent = gCent.X();
    histYcent = gCent.Y();
    gPosX = apad->GetGlobalXCoordinates();
    gPosZ = apad->GetGlobalZCoordinates();

    if(drawPadIDs) {     
      TLatex* tex = new TLatex();
      tex->SetTextSize(0.02);
      tex->SetTextColor(kBlue);
      tex->SetTextAlign(22);      
      tex->DrawLatex(gCent.X(), gCent.Y(), Form("%d",i)); // Global Center coordinates and Pad Id

    }

  }
    TLatex* Avalanche = new TLatex();
    Avalanche->SetTextSize(0.02);
    Avalanche->SetTextColor(kMagenta);
    Avalanche->SetTextAlign(22);      
    Avalanche->DrawLatex(xAv*10, zAv*10, Form("Av = (%1.4f, %1.4f)",xAv, zAv));
    hist->SetMarkerStyle(31);
    hist->SetMarkerColor(kMagenta);
    hist->SetMarkerSize(2.0);
    hist->Fill(xAv*10,zAv*10);
   // hist->GetXaxis()->SetRangeUser(histXcent-500,histXcent+500);
   // hist->GetYaxis()->SetRangeUser(histYcent-500,histYcent+500);
    c->Modified();
    c->Update();
  
}
