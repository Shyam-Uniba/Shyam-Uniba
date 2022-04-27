//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcDriftTask
//      see CeeTpcDriftTask.h for details
//
// Environment:
//      Software developed for the MPD Detector at NICA.
//
// Author List:
//      Alexandr Zinchenko LHEP, JINR, Dubna - adapted for MPD from PANDARoot
//
//-----------------------------------------------------------

// This Class' Header ------------------
#include "CeeTpcDriftTask.h"
#include "CeeTpcPoint.h"

// Collaborating Class Headers --------
#include "CeeTpcGas.h"
#include "CeeTpcDriftedElectron.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "CeeTpcDigiPar.h"
#include <TClonesArray.h>
#include <TMath.h>
#include <TRandom.h>
#include <TSystem.h>
#include "QAPlotCollection.h"

// C/C++ Headers ----------------------
#include <math.h>
#include <iostream>

using std::cout;
using std::exp;
using std::sqrt;


// Class Member definitions -----------

//__________________________________________________________________________
CeeTpcDriftTask::CeeTpcDriftTask()
  : FairTask("TPC Drift"), fPersistence(kTRUE),
    fAttach(kTRUE), fDiffuse(kTRUE), fDistort(kFALSE),finitialized(kFALSE),fUniform(kFALSE),
    fqa(NULL)
{
  fPrimBranchName = "CeeTpcPoint";
  std::string tpcGasFile = "../geometry/Ar-90_CH4-10.asc";
  fGas= new CeeTpcGas(tpcGasFile, 130);
  //std::cout<<*_gas<<std::endl;
}
//__________________________________________________________________________

CeeTpcDriftTask::~CeeTpcDriftTask()
{
    delete fGas;
}
void
CeeTpcDriftTask::SetParContainers() {

  std::cout<<"CeeTpcDriftTask::SetParContainers"<<std::endl;
  std::cout.flush();

  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* db = run->GetRuntimeDb();
  if ( ! db ) Fatal("SetParContainers", "No runtime database");

  // Get CeeTpc digitisation parameter container
  fpar= (CeeTpcDigiPar*) db->getContainer("CeeTpcDigiPar");
  if (! fpar ) Fatal("SetParContainers", "CeeTpcDigiPar not found");
}
//__________________________________________________________________________

InitStatus CeeTpcDriftTask::Init()
{
  finitialized=false;
  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("CeeTpcDriftTask::Init","RootManager not instantiated!");
      return kERROR;
    }
  
  // Get input collection
  //std::cout << fkNsec2 << std::endl;
  fPrimArray = (TClonesArray*) ioman->GetObject(fPrimBranchName);
  
  if(fPrimArray==0)
    {
      Error("CTpcDriftTask::Init","PrimaryElectron-array not found!");
      return kERROR;
    }
  fDriftedArray = new TClonesArray("CeeTpcDriftedElectron");
  ioman->Register("CeeTpcDriftedElectron","CeeTpc",fDriftedArray,fPersistence); 
  
  fyGem=fpar->getYGem();
  
  if(fqa!=NULL){
  fxVariation = fqa->getTH1D("xShifts","x-coordinate shifts from drifting", 200, -0.7, 0.7);
  fzVariation = fqa->getTH1D("zShifts","z-coordinate shifts from drifting" , 200, -0.7, 0.7);
  fxVarAndDriftL = fqa->getTH2D("xDrift_vs_DriftLength", "x-shifts vs. DriftLength",200,-0.7,0.7,200,-100,100);
  fzVarAndDriftL = fqa->getTH2D("zDrift_vs_DriftLength","y-shift vs. DriftLength", 200,-0.7,0.7,200,-100,100);
  fCharge = fqa->getTH1D("TotalCharge","Charge created for Each MC Point",800,0,800);
  ftVariation = fqa->getTH1D("Time Variation","Time variation due to drift",500,-250,250);
  fyVarAnddl = fqa->getTH2D("y_vs_Drift_length", "Y-Position vs Drift length",200,-100,100,200,-100,100);
  fxzVar = fqa->getTH2D("xz", "X-Z Projection",200, -0.7, 0.7,200,-0.7, 0.7);
  }
  fxzVarAnddl = new TH3D("XZ_vsdl","X-Z shifts vs Drift length",200,-0.7,0.7,200,-0.7,0.7,200,-100,100);
  fxzCharge = new TH2D("XZ_Charge","XZ of Charge",450,-45.,45.,480,-60.,60.);
  fyzCharge = new TH2D("YZ_Charge","YZ of Charge",450,-45.,45.,320,-40.,40.);
  finitialized=true;
  return kSUCCESS;
}
//__________________________________________________________________________

void CeeTpcDriftTask::Exec(Option_t* opt)
{
  std::cout<< "CeeTpcDriftTask::Exec" << std::endl;
  // Reset output Array
  if(fDriftedArray==0) Fatal("CeeTpcPrimCluster::Exec)","No DriftedElectronArray");
  fDriftedArray->Delete();

  //loop over incoming electrons
  Int_t nc=fPrimArray->GetEntriesFast();

  if (fUniform){
  // Implementation of Uniform distribuition of Charge in step volume
  for(int ic=0;ic<nc;++ic){
     if (ic+1==nc) continue; // Index should be smaller than the array
     CeeTpcPoint *inp = (CeeTpcPoint*)fPrimArray->At(ic);
     CeeTpcPoint *outp = (CeeTpcPoint*)fPrimArray->At(ic+1);
	 if (inp->GetTrackID()!=outp->GetTrackID()) continue;

    //cut on the MCTime of 100 ns 15 Jan 2021
    Float_t mcpoint_time = inp->GetTime();
    if (mcpoint_time<0. || mcpoint_time>100.) continue; // Skip the track for which mc time is very large

    double e1 = inp->GetEnergyLoss(); // energyloss at the entering position
    double e2 = outp->GetEnergyLoss(); // energyloss at the exit position
    if (e2==0) continue; // one step volume may have more than two hits

    // access entering and exit position in step volume
    TVector3 p1;inp->Position(p1);
    TVector3 p2;outp->Position(p2);
    //create electrons
    Double_t dE = TMath::Abs(e2-e1)*1E9; //convert from GeV to eV
    Int_t q = (Int_t) TMath::Abs(dE / fGas->W());
    // Distribuite charge uniformely in the step volume
    for(Int_t ie = 0; ie < q; ++ie) {
      TVector3 rand_Pos = DistribuiteChargeUniform(p1,p2);
      Double_t y_length = rand_Pos.Y();
      Double_t driftl = 0;
      Double_t dx = 0, dz = 0, dt = 0, dt1 = 0.;
      Double_t xPos = 0, yPos = 0, zPos = 0;
      Int_t size = 0;
      // Calculate drift time
      driftl = fyGem - y_length;
      // attachment
      if (fAttach) {
	if ( exp(-driftl * fGas->k()) < gRandom->Uniform() ) continue;
      }
      // diffusion
      if (fDiffuse) {
	Double_t sqrtDrift = sqrt(driftl);
    Double_t sigmat = fGas->Dt() * sqrtDrift;
    Double_t sigmal = fGas->Dl() * sqrtDrift;
	dt = (driftl+gRandom->Gaus(0,sigmal)) / fGas->VDrift();
	dx = gRandom->Gaus(0,sigmat);
	dz = gRandom->Gaus(0,sigmat);
	dt1 = (gRandom->Gaus(0,sigmal)) / fGas->VDrift();
    }
      // drift distortions
      if (fDistort) {
	// TODO: to be implemented
      }

     Double_t xend = rand_Pos.X()+dx, zend = rand_Pos.Z()+dz, tend = inp->GetTime()+dt;
     size = fDriftedArray->GetEntriesFast();
     CeeTpcDriftedElectron* myElectron = new((*fDriftedArray)[size]) CeeTpcDriftedElectron(xend, zend, tend, inp);

     myElectron->SetLink(FairLink(fPrimBranchName, ic));

     xPos = rand_Pos.X(); yPos = rand_Pos.Y(); zPos = rand_Pos.Z();
     fxzCharge->Fill(zPos,xPos); // Position of charge is filled
	 fyzCharge->Fill(zPos,yPos); // Position of charge is filled
     FillHistograms(xPos, yPos, zPos, dx, dz, driftl,dt1, q);

    } // end loop over electrons
  } // end loop over clusters

  }
  else // All charge created at a point
  {
  for(int ic=0;ic<nc;++ic){ 
    CeeTpcPoint *mcpoint = (CeeTpcPoint*)fPrimArray->At(ic);
    //cut on the MCTime of 100 ns 15 Jan 2021
    Float_t mcpoint_time = mcpoint->GetTime();
    if (mcpoint_time<0. || mcpoint_time>100.) continue; // Skip the track for which mc time is very large
    //create single electrons
    Double_t dE = mcpoint->GetEnergyLoss() * 1E9; //convert from GeV to eV
    Int_t q = (Int_t) TMath::Abs(dE / fGas->W());

    for(Int_t ie = 0; ie < q; ++ie) {
      Double_t y_length = mcpoint->GetY();
      Double_t driftl = 0;
      Double_t dx = 0, dz = 0, dt = 0, dt1 = 0.;
      Double_t xPos = 0, yPos = 0, zPos = 0;
      Int_t size = 0;
      // Calculate drift time
      driftl = fyGem - y_length;
      // attachment
      if (fAttach) {
	if ( exp(-driftl * fGas->k()) < gRandom->Uniform() ) continue;
      }
      // diffusion
      if (fDiffuse) {
	Double_t sqrtDrift = sqrt(driftl);
    Double_t sigmat = fGas->Dt() * sqrtDrift;
    Double_t sigmal = fGas->Dl() * sqrtDrift;
	dt = (driftl+gRandom->Gaus(0,sigmal)) / fGas->VDrift();
	dx = gRandom->Gaus(0,sigmat);
	dz = gRandom->Gaus(0,sigmat);
	dt1 = (gRandom->Gaus(0,sigmal)) / fGas->VDrift();
    }
      // drift distortions
      if (fDistort) {
	// TODO: to be implemented
      }

     Double_t xend = mcpoint->GetX()+dx, zend = mcpoint->GetZ()+dz, tend = mcpoint->GetTime()+dt;

     size = fDriftedArray->GetEntriesFast();
     CeeTpcDriftedElectron* myElectron = new((*fDriftedArray)[size]) CeeTpcDriftedElectron(xend, zend, tend, mcpoint);
    										
     myElectron->SetLink(FairLink(fPrimBranchName, ic));
     
     xPos = mcpoint->GetX(); yPos = mcpoint->GetY(); zPos = mcpoint->GetZ();
     fxzCharge->Fill(zPos,xPos); // Position of charge is filled
	 fyzCharge->Fill(zPos,yPos); // Position of charge is filled
     FillHistograms(xPos, yPos, zPos, dx, dz, driftl,dt1, q);
     
    } // end loop over electrons
  } // end loop over clusters 
  }
  return;
}

// Distribuite charge uniform in a step volume
TVector3 CeeTpcDriftTask:: DistribuiteChargeUniform(TVector3 InPos, TVector3 OutPos){
	TVector3 p;
	float rand = gRandom->Uniform();
	double x = InPos.X()+(OutPos.X()-InPos.X())*rand;
	double y = InPos.Y()+(OutPos.Y()-InPos.Y())*rand;
	double z = InPos.Z()+(OutPos.Z()-InPos.Z())*rand;
	p.SetXYZ(x,y,z);
	return p;
}

void CeeTpcDriftTask::FillHistograms(double x, double y, double z, double dx, double dz, double dl, double dt, int charge) {
	if(fqa!=NULL){
		fxVariation->Fill(dx);
		fzVariation->Fill(dz);
		ftVariation->Fill(dt);
		fCharge->Fill(charge);
		fxVarAndDriftL->Fill(dx, dl);
		fzVarAndDriftL->Fill(dz, dl);
		fyVarAnddl->Fill(y,dl);
		fxzVarAnddl->Fill(dx,dz,dl);
		fxzVar->Fill(dx,dz);
	}
}

//WriteHistograms() has to be called once in the runDigi.C macro!
void CeeTpcDriftTask::WriteHistograms() {
  if(!finitialized)return;
  TFile* file=FairRootManager::Instance()->GetOutFile();
  file->mkdir("CeeTpcDriftTask");
  file->cd("CeeTpcDriftTask");
  fxVariation->Write();
  delete fxVariation;
  fzVariation->Write();
  delete fzVariation;
  fxVarAndDriftL->Write();
  delete fxVarAndDriftL;
  fzVarAndDriftL->Write();
  delete fzVarAndDriftL;
  ftVariation->Write();
  delete ftVariation;
  fCharge->Write();
  delete fCharge;
  fyVarAnddl->Write();
  delete fyVarAnddl;
  fxzVarAnddl->Write();
  delete fxzVarAnddl;
  fxzVar->Write();
  delete fxzVar;
  fxzCharge->Write();
  delete fxzCharge;
  fyzCharge->Write();
  delete fyzCharge;
 }

ClassImp(CeeTpcDriftTask)

