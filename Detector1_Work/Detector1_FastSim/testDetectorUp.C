
// You have to load the class before ... ;-)
// .L DetectorK.cxx++
// Detector 1 Fast Simulation
// Set Magnetic field 1.4 Tesla for Detector1
//void standardPlots() {
#include "DetectorK.h"
#include "DetectorK.cxx"

void testDetectorUp(float etamin=0.0, float etamax=0.5) {


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
  //
  //  
  its.AddLayer((char*)"VTX1",  3.3 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTX2",  4.35 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTX3",  5.40 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTXSUPPORT",  6.3 ,  x_x0VTX_SUP/sin_theta); 
  its.AddLayer((char*)"BARR1", 21.0+shift, x_x0BARR/sin_theta, resRPhiBARR, resZBARR,eff); 
  its.AddLayer((char*)"BARR2", 22.68+shift, x_x0BARR/sin_theta, resRPhiBARR, resZBARR,eff); 
  its.AddLayer((char*)"BARRSUPPORT", 23.50+shift,  x_x0BARR_SUP/sin_theta); 
  its.AddLayer((char*)"MM1",  33.14 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  its.AddLayer((char*)"MM2",  51.0 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  its.AddLayer((char*)"TOF",  64.0 ,  x_x0TOF/sin_theta, resRPhiTOF, resZTOF,eff); 
  its.AddLayer((char*)"DIRC",  72.96,  x_x0DIRC/sin_theta);
  its.AddLayer((char*)"MM3",  77.0 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff);   
  
  //TCanvas *c = new TCanvas("c","c",1200,1000);
  //c->cd();
 // its.PlotLayout(1);
  its.PrintLayout();
 //exit(0);
  its.SolveViaBilloir(0);
  its.MakeStandardPlots(0,2,1,kTRUE);

  eta+=binwidth;
 
  }  // Loop for Pesudorapidity
}
