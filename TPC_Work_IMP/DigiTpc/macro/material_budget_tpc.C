/*shyam kumar: shyam055119@gmail.com
{For calculation of material budget go to
 gconfig folder there is a g3Config.C file (if you are using geant3, g4Config.C if with geant4), please edit it and modify the line:
st->SetMinPoints(1);
with
st->SetMinPoints(0);*/
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <TGeoNode.h>
#include "TGeoVolume.h"
#include <TFile.h>
#include <TTree.h>
#include <TGeoManager.h>
#include <TClonesArray.h>
#include <TH1D.h>
#include <TROOT.h> 
#include <TH2D.h>
#include <TVector3.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TStopwatch.h>
#include <THStack.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TColor.h>

void material_budget_tpc(int nEvents=100)
{
   //gStyle->SetOptStat("nemruoi");
         gStyle->SetOptStat(1);
         gStyle->SetTitleSize(0.05,"");
         gStyle->SetTitleSize(0.045,"XY");
         gStyle->SetLabelSize(0.045,"XY");
         gStyle->SetTitleOffset(1.0,"XY");
         gStyle->SetTitleOffset(1.5,"Z");
         gStyle->SetPadTickX(1);
         gStyle->SetPadTickY(1);
         gStyle->SetOptTitle(0);

// -----   Timer   --------------------------------------------------------
  TStopwatch timer;
  timer.Start();
  // ------------------------------------------------------------------------

  TFile* f = new TFile("tpc_sim.root"); //
  TTree *t=(TTree *) f->Get("ceesim") ;
  TFile* f1 = new TFile("tpc_params.root");
  f1->Get("FairGeoParSet"); // if it is not working use FairBaseParSet
  TClonesArray* mc_array=new TClonesArray("CeeMCTrack");
  t->SetBranchAddress("MCTrack",&mc_array);//Branch names
  TClonesArray* rad_array=new TClonesArray("FairRadLenPoint");
  t->SetBranchAddress("RadLen",&rad_array);

  TString detname,volname;
  double radlen=0.,effradl=0.,effradlsum=0.,theta=0.,phi=0.,d=0.;
  TVector3 in, out, dist, point, mom;
  Bool_t debug =kFALSE;

  // histos
  int res = 200; int angres = 180;
  TH1D* hRadLenDistMat = new TH1D("radldm","Material thicknesses",100,0,100.);
  hRadLenDistMat->SetTitle("Effective distance ;Effective distance; Entries");
  TH1D* hRadLenDistEff = new TH1D("radlde","Effective Radiation length",100,0,0.01);
  hRadLenDistEff->SetTitle("Effective radiation length (X/X_{0}) ;X/X_{0};Entries");
  TH2D* hisxy = new TH2D("hisxy","MC Points, xy view",res,-80.,80.,res,-60.,60.);
  hisxy->SetTitle("X-Y View;X Points;Y Points");
  TH2D* hisrz = new TH2D("hisrz","MC Points, rz view",res,-20.,80.,res,-80.,80.);
  hisrz->SetTitle("R-Z View;Z Points;r Points");
  

  TProfile* thetaprofile = new TProfile("thetaprof","Radiaion length (#theta)",angres,0.,(TMath::Pi())*2/3);
  thetaprofile->SetTitle("Effective radiation length (X/X_{0}) vs #theta ;#theta;X/X_{0}");
  TProfile* phiprofile = new TProfile("phiprof","Radiaion length (#phi)",angres,-1.*TMath::Pi(),TMath::Pi());
  phiprofile->SetTitle("Effective radiation length (X/X_{0}) vs #phi;#phi;X/X_{0}");

  TProfile2D* histhephi = new TProfile2D("hThePhi","",90,0.,(TMath::Pi())*2/3,90,-1.*TMath::Pi(),TMath::Pi());
  histhephi->SetTitle("Effective radiation length (X/X_{0}) vs #theta & #phi;#theta;#phi;X/X_{0}");
  TH1D* hisz = new TH1D("hZ","Z",100,-20.,80.);
  hisz->SetTitle("(X/X_{0}) vs Z-Points ;Z-Points; Entries");
// Centering the Titles
  hRadLenDistMat->GetXaxis()->CenterTitle();
  hRadLenDistMat->GetYaxis()->CenterTitle();
  hRadLenDistEff->GetXaxis()->CenterTitle();
  hRadLenDistEff->GetYaxis()->CenterTitle();
  hisxy->GetXaxis()->CenterTitle();
  hisxy->GetYaxis()->CenterTitle();
  hisrz->GetXaxis()->CenterTitle();
  hisrz->GetYaxis()->CenterTitle();
  thetaprofile->GetXaxis()->CenterTitle();
  thetaprofile->GetYaxis()->CenterTitle();
  phiprofile->GetXaxis()->CenterTitle();
  phiprofile->GetYaxis()->CenterTitle();
  histhephi->GetXaxis()->CenterTitle();
  histhephi->GetYaxis()->CenterTitle();
  histhephi->GetZaxis()->CenterTitle();
//-----------------Main part of code-----------------------------------
  for (Int_t event=0; event<t->GetEntriesFast(); event++)// Reading the entries in tree
  {
    t->GetEntry(event);
   
   for (Int_t trackno=0; trackno<mc_array->GetEntriesFast();trackno++){
      CeeMCTrack* aTrack = (CeeMCTrack*)mc_array->At(trackno);
      aTrack->GetMomentum(mom);
      theta = mom.Theta(); phi = mom.Phi();
      effradl=0.;  effradlsum=0.; d=0.;
      for (Int_t k=0; k<rad_array->GetEntriesFast(); k++){
        FairRadLenPoint* radpoint = (FairRadLenPoint*)rad_array->At(k);
          if (radpoint->GetTrackID() != trackno) continue;
         in = radpoint->GetPosition();
         out = radpoint->GetPositionOut();
        point.SetXYZ(0.5*(in.x()+out.x()),0.5*(in.y()+out.y()),0.5*(in.z()+out.z()));
       TGeoNode* node = gGeoManager->FindNode(point.x(),point.y(),point.z());
        if(!node) continue;
        node->cd();
        volname = node->GetVolume()->GetName();
        if((out.z()-in.z())<0 || (in - out).Mag()<1.0e-5 ) continue;     // if in-position=out-position looks sometimes step =0 due to random step size
        if (volname !="TPC_vol") continue;;
           radlen = radpoint->GetRadLength();
           dist = in - out;
           effradl = dist.Mag()/radlen;
           d+=dist.Mag();
           //if(d<0.06)continue;
           effradlsum+=effradl;
           cout<<"Radiation Length material\t"<<radlen<<endl;
           if(debug){
    	   cout<<"In Position\t"<<in.X()<<"\t"<<in.Y()<<"\t"<<in.Z()<<endl;
    	   cout<<"Out Position\t"<<out.X()<<"\t"<<out.Y()<<"\t"<<out.Z()<<endl;
    	   cout<<"Radiation Length material\t"<<radlen<<endl;
    	   cout<<"Distance\t"<<dist.Mag()<<endl;
    	   cout<<"Effective Distance"<<d<<endl;
    	   cout<<"Volume Name\t"<<volname<<endl;
    	   cout<<"Radiation length"<<effradl<<endl;
    	   cout<<"Effective Radiation Length"<<effradlsum<<endl;
    	   cout<<"\t"<<endl;
       }
        hisxy->Fill(in.X(),in.Y());
        if(in.Y()>0.) hisrz->Fill(in.Z(),in.Perp());
        else hisrz->Fill(in.Z(),-1.*in.Perp());
        hisz->Fill(in.Z());
      } //radiation points
        hRadLenDistMat->Fill(d);
        hRadLenDistEff->Fill(effradlsum);
        thetaprofile->Fill(theta,effradlsum);
        phiprofile->Fill(phi,effradlsum);
        histhephi->Fill(theta,phi,effradlsum);
 } // Track loop

 } // Main event loop
//------------------------------End of the main part of code--
Int_t a=2,b=2;
int resol = 250;
TPad* mypad = (TPad*) gPad;
TCanvas* can1 = new TCanvas("can1","MCHit view in TPC",0,0,2000,1200);
can1->Divide(a,b);
can1->cd(1);
hisxy->SetStats(true);
hisxy->Draw("colz");
can1->cd(2);
hisrz->SetStats(true);
hisrz->Draw("colz");
can1->cd(3);
hisz->SetFillColor(38);
hisz->Draw("hist");
can1->cd(4);
gPad->SetLogy();
hRadLenDistMat->SetFillColor(38);
hRadLenDistMat->Draw();
can1->Print("basic_plot.pdf");

TCanvas* can2 = new TCanvas("can2","Radiation Length for TPC detector",0,0,2000,1200);
can2->Divide(a,b);
can2->cd(1);
gPad->SetLogy();
hRadLenDistEff->SetFillColor(38);
hRadLenDistEff->Draw();
can2->cd(2);
gPad->SetLogy();
phiprofile->SetFillColor(kYellow);
phiprofile->SetLineColor(kBlue);
phiprofile->Draw("hist");
can2->cd(3);
gPad->SetLogy();
thetaprofile->SetFillColor(kRed);
thetaprofile->SetLineColor(kBlue);
thetaprofile->Draw("hist");
can2->cd(4);
histhephi->SetFillColor(kBlack);
histhephi->SetStats(true);
histhephi->Draw("lego");
can2->Print("effradl_tpc_plot.pdf");
// -----   Finish   -------------------------------------------------------
  timer.Stop();
  Double_t rtime = timer.RealTime();
  Double_t ctime = timer.CpuTime();
  cout << endl << endl;
  cout << "Macro finished succesfully." << endl;
  cout << "Real time " << rtime << " s, CPU time " << ctime << " s" << endl;
  cout << endl;
  // ------------------------------------------------------------------------

}

