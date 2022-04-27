//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcClusterizerTask
//      see CeeTpcClusterizerTask.hh for details
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
#include "CeeTpcClusterizerTask.h"

// C/C++ Headers ----------------------


// Collaborating Class Headers --------
#include "FairRootManager.h"
#include "TClonesArray.h"
#include "CeeTpcPoint.h"
#include "CeeTpcGas.h"
#include "TRandom.h"
#include "CeeTpcPrimaryCluster.h"
#include "LinearInterpolPolicy.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::fabs;
using std::floor;

// Class Member definitions -----------


CeeTpcClusterizerTask::CeeTpcClusterizerTask()
  : FairTask("TPC Clusterizer"), fpersistence(kFALSE),
    fmereChargeConversion(kFALSE), fPoti(20.77e-9)
{
  fpointBranchName = "CeeTpcPoint";
  std::string tpcGasFile = "../geometry/Ar-90_CH4-10.asc";
  fgas= new CeeTpcGas(tpcGasFile, 130);
}


CeeTpcClusterizerTask::~CeeTpcClusterizerTask()
{}



InitStatus CeeTpcClusterizerTask::Init()
{
  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("CeeTpcClusterizerTask::Init","RootManager not instantiated!");
      return kERROR;
    }
  
  // Get input collection
  fpointArray=(TClonesArray*) ioman->GetObject(fpointBranchName);
  
  if(fpointArray==0)
    {
      Error("CeeTpcClusterizerTask::Init","Point-array not found!");
      return kERROR;
    }

  // create and register output array
  fprimArray = new TClonesArray("CeeTpcPrimaryCluster"); 
  ioman->Register("CeeTpcPrimaryCluster","CeeTpc",fprimArray,fpersistence);
  return kSUCCESS;
 
}

void CeeTpcClusterizerTask::Exec(Option_t* opt)
{
  std::cout<< "CeeTpcClusterizerTask::Exec" << std::endl;
  // Reset output Array
  if(fprimArray==0) Fatal("CeeTpcPrimCluster::Exec","No PrimClusterArray");
  fprimArray->Delete();
  Int_t np=fpointArray->GetEntriesFast();
  if(np<2){
    Warning("CeeTpcClusterizerTask::Exec","Not enough Hits in CeeTpc for Digitization (<2)");
    return;
  }
  
  if(fmereChargeConversion){
  	ChargeConversion();  
  	return; //goodbye, you wretched world!
  }
  	
  
  CeeTpcPoint* point;
  CeeTpcPoint* theLastPoint;
  Int_t icluster=0; 
  theLastPoint= (CeeTpcPoint*)fpointArray->At(0);

  for(Int_t ip=1;ip<np;++ip){
    point=(CeeTpcPoint*) fpointArray->At(ip);
    //point->Print();

    // check if points are not too far from each other
    TVector3 p1;point->Position(p1);
    TVector3 p2;theLastPoint->Position(p2);

    TVector3 d=p1-p2;
    if(d.Mag()>1){
      theLastPoint=point;
      continue;
    }

    //check if hits ly on the same track
    if(point->GetTrackID()==theLastPoint->GetTrackID()){
      Double_t dE=point->GetEnergyLoss()*1E9; //convert from GeV to eV

      //Step 0: calculate the overall ammount of charge, produced
      if(dE<0){
        Error("CeeTpcClusterizerTask::Exec","Note: particle:: negative Energy loss!");
        theLastPoint=point;
        continue;
      }
      Int_t q_total =(Int_t)floor(fabs(dE / fgas->W()));
      Int_t q_cluster=0;
      Int_t ncluster=0;
      //Step 1: Create Clusters
      //while still charge not used-up distribute charge into next cluster

      while(q_total>0){
        //roll dice for next clustersize
        q_cluster=fgas->GetRandomCS(gRandom->Uniform());
        if(q_cluster>q_total)q_cluster=q_total;
        q_total-=q_cluster;
        // create cluster
        Int_t size = fprimArray->GetEntriesFast();
       
        new((*fprimArray)[size]) CeeTpcPrimaryCluster(point->GetTime(),
						   q_cluster,
						   TVector3(0,0,0),
						   point->GetTrackID(),
						   point->GetMotherId());
	
        ++ncluster;
     }// finish loop for cluster creation

      //Step 2: Distribute Clusters along track segment
      LinearInterpolPolicy().Interpolate(theLastPoint,point,fprimArray,icluster,ncluster);
      icluster+=ncluster;
      }//end check for same track
    theLastPoint=point;
  } // finish loop over GHits

  std::cout<<"CeeTpcClusterizer:: "<<fprimArray->GetEntriesFast()<<" clusters created"<<std::endl;

  return;
}

void CeeTpcClusterizerTask::ChargeConversion()
{
  //hardcoded value from ALICE paper:
  //const Float_t w_ion = 35.97e-9; //mean energy for pair creation 
  Float_t w_ion = fgas->W()*1.e-9;
 
  
  Int_t np=fpointArray->GetEntriesFast();	
  for(Int_t ip=1;ip<np;++ip)
  {
  	CeeTpcPoint* point=(CeeTpcPoint*) fpointArray->At(ip);
	//Do no clustering just convert energy deposition to ionisation
	
	if(point->GetEnergyLoss() < fPoti)
	  continue;
       
	Int_t nel = Int_t(floor(((point->GetEnergyLoss())-fPoti)/w_ion)) + 1;
	  
	//nel=TMath::Min(nel,300); // 300 electrons corresponds to 10 keV

	Int_t size = fprimArray->GetEntriesFast();
	new((*fprimArray)[size])CeeTpcPrimaryCluster(point->GetTime(),
						     nel,
						     TVector3(point->GetX(),
							      point->GetY(),
							      point->GetZ()),
						     point->GetTrackID(),
						     point->GetMotherId());
  }
}


ClassImp(CeeTpcClusterizerTask)

