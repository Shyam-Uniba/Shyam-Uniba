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

void plotPadPlane_CEE_TPC(){

 Bool_t debug = false;
 TString PadPlanefile ="PadCoordinate.txt"; // PadPlane

 CeeTpcPadPlane* PadPlane= new CeeTpcPadPlane(PadPlanefile);

  TCanvas* c=new TCanvas("c","regions",1400,1000);
  c->SetMargin(0.09, 0.03 ,0.1,0.05);

  gPad->Range(0,0,120,80); // x0, z0, x1, z1

  gStyle->SetOptStat(0);
 // gStyle->SetTickLength(0.0,"xy");
 // gStyle->SetLabelSize(0.0,"xy");
 // gStyle->SetFrameLineWidth(0);
 // gStyle->SetAxisColor(kWhite,"xy");
  gStyle->SetCanvasPreferGL(kTRUE);
  
  TH2D* hist = new TH2D("gsfdg","",1240,-620,620,1000,-500,500);
  hist->Draw();
  hist->GetXaxis()->SetTitle("X [mm]");
  hist->GetXaxis()->CenterTitle();
  hist->GetYaxis()->SetTitle("Z [mm]");
  hist->GetYaxis()->CenterTitle();
  hist->GetYaxis()->SetTitleOffset(1.0);
  
  // plot regions
  int npads=PadPlane->GetNPads(); // Total Pads
  cout<<"Total Pads: "<<npads<<endl;
  cout<<"PadId: "<<PadPlane->GetPadIdfromArray(0,10,10)<<endl;
  cout<<"LocalZmin: "<<PadPlane->GetRowMin0LocalZ(1)<<endl;
  cout<<"Pad List: "<<PadPlane->GetRowMin0LocalZ(1)<<endl;
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
    if (debug){
    int padid=apad->GetPadId();
    int sectorid=apad->GetSectorId();
    int rowid = apad->GetRowId();
    int padinrowid = apad->GetPadInRowId();
    std::vector<Double_t> gPosX, gPosZ;
    cout<<"PadId: "<<padid<<"\t SectorId: "<<sectorid<<"\t RowId: "<<rowid<<"\t PadInRowId: "<<padinrowid<<endl;
    gPosX = apad->GetGlobalXCoordinates();
    gPosZ = apad->GetGlobalZCoordinates();
    cout<<"Global X Position: "<<gPosX[0]<<"\t"<<gPosX[1]<<"\t"<<gPosX[2]<<"\t"<<gPosX[3]<<endl;
    cout<<"Global Z Position: "<<gPosZ[0]<<"\t"<<gPosZ[1]<<"\t"<<gPosZ[2]<<"\t"<<gPosZ[3]<<endl;
    cout<<endl;
    }

    int color = 2;
    apad->Draw(color); // Put color number.
    c->Modified();
    c->Update();
  //  c->Print("trap_padplane.gif+5");
  //  c->Draw();
  }
  
  c->SaveAs("padplane.pdf");
  c->SaveAs("padplane.root");
}

