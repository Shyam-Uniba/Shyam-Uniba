
// You have to load the class before ... ;-)
// .L DetectorK.cxx++
// !!!!!!Change Magentic field always in class!!!!!!!!!!!!!!!!!!! 
//void standardPlots() {
#include "DetectorK.h"
#include "DetectorK.cxx"

void testDetectorUp(float etamin, float etamax) {


        gSystem->Exec("rm -rf Output");
        gSystem->Exec("mkdir Output");
        
  float eta = etamin;
  int nbins = 100;
  float binwidth = (etamax-etamin)/100;

  DetectorK its((char*)"ATHENA",(char*)"ITS");

  for (int i=0; i<nbins; i++){
  its.SetAvgRapidity(eta); // This is pesudorapidity eta
  its.SetParticleMass(0.140); // pion 
  its.SetBField(3.0); // set magnetic field 3 Tesla
  Double_t theta = 2.0*TMath::ATan(TMath::Exp(-1.0*eta));
  Double_t sin_theta = fabs(TMath::Sin(theta));
// Don't assign Rphi resolution and Z Resolution, it will be assigned very large number
  // and finally considered as dead layer
  its.AddLayer((char*)"bpipe",3.1,0.0022/sin_theta); // thickness 760 mum; x/x0 = 0.076/35 = 0.0022;
  its.AddLayer((char*)"vertex",     0,     0); // dummy vertex for matrix calculation
  // new ideal Pixel properties?
  Double_t x_x0VTX     = 0.0005; // Per layer VTX
  Double_t x_x0BARR    = 0.0055; // Per layer BARR
  Double_t x_x0MM      = 0.004; // Per layer Micromegas
  Double_t resRPhiVTX     = 10.0e-4/sqrt(12); 
  Double_t resRPhiBARR    = 10.0e-4/sqrt(12); 
  Double_t resRPhiMM      = 150.0e-4; 
  Double_t resZVTX        = 10.0e-4/sqrt(12); 
  Double_t resZBARR       = 10.0e-4/sqrt(12); 
  Double_t resZMM         = 150.0e-4;
  Double_t eff            = 1.0;
  //
  //  /*
  its.AddLayer((char*)"VTX1",  3.3 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTX2",  4.35 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"VTX3",  5.40 ,  x_x0VTX/sin_theta, resRPhiVTX, resZVTX,eff); 
  its.AddLayer((char*)"BARR1", 13.34, x_x0BARR/sin_theta, resRPhiBARR, resZBARR,eff); 
  its.AddLayer((char*)"BARR2", 17.96, x_x0BARR/sin_theta, resRPhiBARR, resZBARR,eff); 
  its.AddLayer((char*)"MM1",  47.72 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  its.AddLayer((char*)"MM2",  49.57 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  its.AddLayer((char*)"MM3",  75.61 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  its.AddLayer((char*)"MM4",  77.46 ,  x_x0MM/sin_theta, resRPhiMM, resZMM,eff); 
  
  //TCanvas *c = new TCanvas("c","c",1200,1000);
  //c->cd();
 // its.PlotLayout(1);
  its.PrintLayout();
  its.SolveViaBilloir(0);
  its.MakeStandardPlots(0,2,1,kTRUE);

  eta+=binwidth;
 
  }  // Loop for Pesudorapidity
}
