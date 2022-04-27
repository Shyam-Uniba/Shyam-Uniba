//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcPadResponseTask
//      see CeeTpcPadResponseTask.hh for details
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

// Panda Headers ----------------------

// This Class' Header ------------------
#include "CeeTpcPadResponseTask.h"
#include <fstream>
// C/C++ Headers ----------------------
#include <iostream>

// Collaborating Class Headers --------
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include "CeeTpcDigiPar.h"
#include "TClonesArray.h"
#include "TRandom.h"
#include "CeeTpcSignal.h"
#include "CeeTpcAvalanche.h"
#include "CeeTpcPad.h"
#include "CeeTpcPadPlane.h"
#include "CeeTpcGem.h"
#include "CeeDetectorList.h"
#include "QAPlotCollection.h"

#include <iostream>

// Class Member definitions -----------


CeeTpcPadResponseTask::CeeTpcPadResponseTask()
  : FairTask("TPC PadResponse"), fpersistence(kFALSE), fminSignalAmp(0), fMeanGemTime(0),
	fSigmaGemTime(0), fselected(false), finitialized(kFALSE), fdebug(kFALSE), table(0),fsigma(0.028),
	fmin(-50.), fmax(50.), fmaxentries(100000)
    //TODO: parameter management
{
  favalancheBranchName = "CeeTpcAvalanche";
  fPadPlaneFileName = "../tpc/DigiTpc_new/macro/PadCoordinate.txt";
  fcloudShapeX = new TF1("CloudShapeX","gausn(0)",-10,10);
  fcloudShapeZ = new TF1("CloudShapeZ","gausn(0)",-10,10);
  fcloudShapeX->SetParameter(0,1); // normalized gaussian
  fcloudShapeZ->SetParameter(0,1); // normalized gaussian
  fstepsize = (fmax-fmin)/fmaxentries;
}


CeeTpcPadResponseTask::~CeeTpcPadResponseTask()
{}

void
CeeTpcPadResponseTask::SetParContainers() {

  std::cout<<"CeeTpcPadResponseTask::SetParContainers"<<std::endl;
  std::cout.flush();

  // Get run and runtime database
  FairRun* run = FairRun::Instance();
  if ( ! run ) Fatal("SetParContainers", "No analysis run");

  FairRuntimeDb* db = run->GetRuntimeDb();
  if ( ! db ) Fatal("SetParContainers", "No runtime database");

  // Get CeeTpc digitisation parameter container
  fpar= (CeeTpcDigiPar*) db->getContainer("CeeTpcDigiPar");
  if (! fpar ) Fatal("SetParContainers", "CeeTpc not found");
}




InitStatus CeeTpcPadResponseTask::Init()
{
  finitialized=false;

  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("CeeTpcPadResponseTask::Init","RootManager not instantiated!");
      return kERROR;
    }

  // Get input collection
  favalancheArray=(TClonesArray*) ioman->GetObject(favalancheBranchName);
  
  if(favalancheArray==0)
    {
      Error("CeeTpcPadResponseTask::Init","Avalanche-array not found!");
      return kERROR;
    }
  
  // create and register output array
  fsignalArray = new TClonesArray("CeeTpcSignal"); 
  ioman->Register("CeeTpcSignal","CeeTpc",fsignalArray,fpersistence);

  fpadPlane= new CeeTpcPadPlane(fPadPlaneFileName.Data());
  if(!fpadPlane)std::cout<<"CeeTpcDigiPar::getPadPlane: Pad plane not found"<<std::endl;

  fgem=fpar->getGem();
  fminSignalAmp=fpar->getMinSigAmp();
  fMeanGemTime = fpar->getMeanTimeInGEM();
  fSigmaGemTime = fpar->getSigmaTimeInGEM();
  feventcounter=0;

  std::cout<<"CeeTpcGem Parameters:"<<std::endl;
  std::cout<<*fgem<<std::endl;
  fcloudShapeZ->SetParameter(1,0);
  fcloudShapeZ->SetParameter(2,1);
  // Table for Gaussian Integrals
  fcloudShapeX->SetParameter(1,0);
  fcloudShapeX->SetParameter(2,1); // use standard Gaussian
  if(table!=0) delete [] table;
  table=new double[fmaxentries];
  double x= fmin;
 // 1D Gaussian Integration table in X-direction
  for(int ix=0;ix<fmaxentries;++ix){
     table[ix] = fcloudShapeX->Integral(fmin,x);
     x+=fstepsize;
  }
  fsigma = fgem->spread_xz();

  fselected=fsecids.size()>0;
  finitialized=true;
  return kSUCCESS;
}


void
CeeTpcPadResponseTask::Exec(Option_t* opt)
{
  std::cout<< "CeeTpcPadResponseTask::Exec" << std::endl;
  // Reset output Array
  if(fsignalArray==0) Fatal("CeeTpcPadResponse::Exec","No SignalArray");
  fsignalArray->Delete(); // Note: signals will be cleand up in the adc!
  int nValidHits=0;
  Int_t na=favalancheArray->GetEntriesFast();
  for(Int_t ia=0;ia<na;++ia){
    CeeTpcAvalanche* Aval=(CeeTpcAvalanche*)favalancheArray->At(ia);
    std::vector<CeeTpcPad*> hitPads;
    double xAv=Aval->x();
    double zAv=Aval->z();
    if (fdebug){
    std::cout<<std::endl;
    std::cout<<"\033[1;34m Avalanche No. \033[0m"<<ia<<std::endl;
    std::cout<<Form("Avalanche Position (XAv,ZAv) = (%1.4f,%1.4f) ",xAv,zAv)<<std::endl;
    }
    double smeared_time = Aval->t()+gRandom->Gaus(fMeanGemTime,fSigmaGemTime);

    if(((xAv< -60. || xAv>60.) || (zAv< -45. || zAv>45.)) || (xAv>-10. && xAv<10.)) {
    if(fdebug){
     Warning("Exec","Avalanche outside of allowed region! X=%f, Z=%f",xAv,zAv);
    }
     continue;
    }
    fpadPlane->GetPadList(xAv,zAv,5.0, hitPads); // radius of pad search = fgem->spread_xz()*3
    // Build Signals
    int nHits=hitPads.size();

   for(int iHit=0; iHit<nHits; ++iHit){
      CeeTpcPad* pad=hitPads[iHit];
      int padId = pad->GetPadId();
      int sectorId = pad->GetSectorId();
      int rowId = pad->GetRowId();
      double Amp=Aval->amp()*GetInducedCharge(pad,xAv,zAv,fsigma);
      if(fdebug){
      std::cout<<"Hit No: "<<iHit<<" PadId: "<<padId<<" Fractional Charge: "<<Amp/Aval->amp()<<std::endl;
      }
      if(fpar->getGaussianNoise()){
	  Amp+=gRandom->Gaus(0.,fpar->getGaussianNoiseAmp());
      }

      // cut on amplitude
      if(Amp>=fminSignalAmp){
        CeeTpcSignal* sig=new((*fsignalArray)[nValidHits]) CeeTpcSignal(feventcounter,smeared_time,Amp,padId,sectorId,rowId,0);
        sig->setmcTrackId(Aval->mcTrackId());
        sig->setmcMotherId(Aval->mcMotherId());
        sig->setmcEventId(feventcounter);
      //  sig->SetLink(FairLink(favalancheBranchName, ia));
        ++nValidHits;
      }
      else {
	 //std::cout<<"PadAmp too small! Amp="<<Amp<<"<"<<fminSignalAmp<<std::endl;
      }
    } // end loop over hit pads

  } // end loop over avalanches

  ++feventcounter;
  std::cout<<fsignalArray->GetEntriesFast()<<" Signals created"<<std::endl;

  return;
}

double CeeTpcPadResponseTask::GetInducedCharge(CeeTpcPad* p, const double Xav,
			    const double Zav, const double sigma) const
{
  double localXAv, localZAv;
  double Zmin, Zmax;
   // Transfer from global to local coordinates
  localXAv = Xav; localZAv = Zav; // Sector 1&2
  int iSector = p->GetSectorId();
  int padId = p->GetPadId();
  if (iSector == 0) {localXAv = -Zav; localZAv = Xav;}
  if (iSector == 3) {localXAv =  Zav; localZAv = -Xav;}

  std::vector<Double_t> lPosX, lPosZ;
  lPosX = p->GetLocalXCoordinates();
  lPosZ = p->GetLocalZCoordinates();
  Zmin = lPosZ[0]*0.1; Zmax = lPosZ[1]*0.1;
  double stepsize = (Zmax-Zmin)/100;
  double z1 = Zmin; double z2; double integral = 0.;
  for (int i=0; i<100; i++){
  z2 = z1+stepsize;
  double Zavg = (z1+z2)/2; double pitch = (Zmax-Zmin);
  double localXmin = ((pitch+Zmin-z1)*lPosX[0]*0.1+(z1-Zmin)*lPosX[1]*0.1)/pitch;
  double localXmax = ((pitch+Zmin-z1)*lPosX[3]*0.1+(z1-Zmin)*lPosX[2]*0.1)/pitch;
  integral+= GausIntegrate((localXmin-localXAv)/sigma,(localXmax-localXAv)/sigma)*(1/sigma)*fcloudShapeZ->Eval(fabs((Zavg-localZAv)/sigma))*stepsize;
  z1 += stepsize;
  }
   return integral;
}

double CeeTpcPadResponseTask::GausIntegrate(const float a, const float b) const
{
int ixa = fabs(floor((a-fmin)/fstepsize)); int ixb = fabs(floor((b-fmin)/fstepsize)); // Get Table index
return fabs(table[ixb]-table[ixa]);
}

ClassImp(CeeTpcPadResponseTask)
