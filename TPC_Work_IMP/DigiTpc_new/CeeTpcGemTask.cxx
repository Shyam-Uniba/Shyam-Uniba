//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcGemTask
//      see CeeTpcGemTask.hh for details
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
#include "CeeTpcGemTask.h"

// C/C++ Headers ----------------------


// Collaborating Class Headers --------
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "TRandom.h"
#include "CeeTpcDriftedElectron.h"
#include "CeeTpcAvalanche.h"
#include "CeeTpcGem.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"

#include "CeeDetectorList.h"

#include <iostream>
#include <cmath>


using std::log;

// Class Member definitions -----------


CeeTpcGemTask::CeeTpcGemTask()
  : FairTask("TPC Gem"), fpersistence(kFALSE), fgainFluctuations(kFALSE),
    finitialized(kFALSE)
{
  fdriftedBranchName = "CeeTpcDriftedElectron";
  file = TFile::Open("../tpc/DigiTpc_new/gain.root");
  hgain = (TH1F*) file->Get("gain1");
}


CeeTpcGemTask::~CeeTpcGemTask()
{}



InitStatus CeeTpcGemTask::Init()
{

  finitialized=false;
  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("CeeTpcGemTask::Init","RootManager not instantiated!");
      return kERROR;
    }
  
  // Get input collection
  fdriftedArray=(TClonesArray*) ioman->GetObject(fdriftedBranchName);
  
  if(fdriftedArray==0)
    {
      Error("CeeTpcGemTask::Init","DriftedElectron-array not found!");
      return kERROR;
    }
  
  // create and register output array
  favalancheArray = new TClonesArray("CeeTpcAvalanche"); 
  ioman->Register("CeeTpcAvalanche","CeeTpc",favalancheArray,fpersistence);

  finitialized=true;
  return kSUCCESS;
}


void CeeTpcGemTask::Exec(Option_t* opt)
{
  std::cout<< "CeeTpcGemTask::Exec" << std::endl;
  // Reset output Array
  if(favalancheArray==0) Fatal("CeeTpcPrimCluster::Exec)","No AvalancheArray");
  favalancheArray->Delete();
  Int_t counter=0;
  Int_t nd=fdriftedArray->GetEntriesFast();
  std::cout<<"Aggregating drifted electrons into avalanches ";
  // loop over drifted electrons
  for(Int_t id=0;id<nd;++id){
    CeeTpcDriftedElectron* e=(CeeTpcDriftedElectron*)fdriftedArray->At(id);
    int na=favalancheArray->GetEntriesFast();
    // look if there is already an avalanche where we can put this electron
    bool found=false;

    if(!found){
	  double gainFactor = 1.;
	  if(fgainFluctuations){ // Gain fluctuations read from the Gain histogram randomly.
      fgain = 1.0;
	  gainFactor = hgain->GetRandom();
	  }
	  //  gainFactor = -log(gRandom->Uniform());
	  
      CeeTpcAvalanche* myAvalanche = new ((*favalancheArray)[na]) CeeTpcAvalanche(e->x(), e->z(), e->t(),
						fgain*gainFactor, e);
      //myAvalanche->SetLink(FairLink(fdriftedBranchName, id));
    }
  } // end loop over drifted electrons
  std::cout<<" finished."<<std::endl;
  std::cout<<favalancheArray->GetEntriesFast()<<" Avalanches created"<<std::endl;
  std::cout<<counter<<" aggregations done."<<std::endl;
  return;
}

ClassImp(CeeTpcGemTask)

