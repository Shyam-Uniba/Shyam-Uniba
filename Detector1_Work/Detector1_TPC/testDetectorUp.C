
// You have to load the class before ... ;-)
// .L DetectorK.cxx++
// Detector 1 Fast Simulation
// Set Magnetic field 1.4 Tesla for Detector1
//void standardPlots() {
#include "DetectorK.h"
#include "DetectorK.cxx"

void testDetectorUp(float etamin=0.0, float etamax=0.0) {


        gSystem->Exec("rm -rf Output");
        gSystem->Exec("mkdir Output");
        
  float eta = etamin;
  float binwidth = 0.01;
  int nbins = (etamax-etamin)/binwidth;
  
  DetectorK its((char*)"ECCE",(char*)"Detector");

  for (int i=0; i<nbins+1; i++){
  if (fabs(eta) >= -0.0001 && fabs(eta) <= 0.0001) eta = 0.00; // avoid negative -0.00
  its.SetAvgRapidity(eta); // This is pesudorapidity eta
  its.SetParticleMass(0.140); // pion 
  its.SetBField(1.4); // Set Magnetic field 1.4 Tesla
  Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
  Double_t sin_theta = fabs(TMath::Sin(theta));

// Don't assign Rphi resolution and Z Resolution, it will be assigned very large number
  // and finally considered as dead layer
  its.AddLayer((char*)"bpipe",3.1,0.0022/sin_theta); // thickness 760 mum; x/x0 = 0.076/35 = 0.0022;
  its.AddLayer((char*)"vertex",     0,     0); // dummy vertex for matrix calculation
  // new ideal Pixel properties?
  Double_t x_x0VTX     = 0.0005; // Per layer VTX
  Double_t x_x0VTX_SUP     = 0.03/30; // VTX Support realistic by Ernst
  Double_t x_x0BARR    = 0.0055; // Per layer BARR
  Double_t x_x0BARR_SUP    = 0.03/30; // Per layer BARR
  Double_t x_x0MM      = 0.00773532/3; // Per layer Micromegas
  Double_t x_x0TOF      = 0.05582; // LGAD layer
  Double_t x_x0DIRC     = 0.12737983; // DIRC
  Double_t resRPhiVTX     = 10.0e-4/sqrt(12); 
  Double_t resRPhiBARR    = 10.0e-4/sqrt(12); 
  Double_t resRPhiMM      = 55.0e-4; 
  Double_t resZVTX        = 10.0e-4/sqrt(12); 
  Double_t resZBARR       = 10.0e-4/sqrt(12); 
  Double_t resZMM         = 55.0e-4;
  Double_t resRPhiTOF = 30.0e-4; // 30 mum
  Double_t resZTOF = 30.0e-4; // 30 mum
  Double_t eff            = 1.0;
  Double_t shift = 0.0; // cm
  
  Double_t resRPhiTPC = 95.0e-4; // 95 mum
  Double_t resZTPC = 145.0e-4; // 145 mum 
  Float_t radLPerRow = 0.1/13289.4;  
  Float_t radLInnerCage = 0.005; // 0.5%
  Float_t radLOuterCage = 0.005; // 0.5%
  Float_t tpcRadialPitch  =    0.1 ;    // cm
  Float_t tpcRows            =  200 ;
  Float_t rowOneRadius = 20.1; // cm
  Bool_t flag_addTPC = true;
  
  //
  //  
  its.AddLayer((char*)"VTX1",  3.6 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTX2",  4.8 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTX3",  10.5 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"Barr1",  18.0 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff);
//  its.AddLayer((char*)"VTXSUPPORT",  13.0 ,  x_x0VTX_SUP/sin_theta); 
  if (flag_addTPC){
   its.AddLayer((char*)"InnerCage",  19.5,  radLInnerCage/sin_theta); 
  for ( Int_t k = 0 ; k < tpcRows; k++ ) {    
    Float_t rowRadius =0;
    rowRadius =  rowOneRadius + k*tpcRadialPitch ;
    TString name = Form("tpc_%d",k);
    its.AddLayer((char*) name.Data(),rowRadius,radLPerRow,resRPhiTPC,resZTPC,eff);  
  }
  its.AddLayer((char*)"OuterCage",  40.5,  radLOuterCage/sin_theta); 
  }
  its.AddLayer((char*)"BARR2", 42.0+shift, x_x0BARR/sin_theta, resRPhiBARR, resZBARR,eff); 
 // its.AddLayer((char*)"BARRSUPPORT", 42.50+shift,  x_x0BARR_SUP/sin_theta); 
  its.AddLayer((char*)"MPGD1",  60.0 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  its.AddLayer((char*)"MPGD2",  64.0 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
 // its.AddLayer((char*)"DIRC",  72.96,  x_x0DIRC/sin_theta);
  its.AddLayer((char*)"MPGD3",  77.0 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff);   
  
  TCanvas *c = new TCanvas("c","c",1200,1000);
  c->cd();
  its.PlotLayout(1);
  its.PrintLayout();
//  exit(0);
  its.SolveViaBilloir(0);
  its.MakeStandardPlots(0,2,1,kTRUE);

  eta+=binwidth;
 
  }  // Loop for Pesudorapidity
}
