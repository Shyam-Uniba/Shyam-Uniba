//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      Implementation of class CeeTpcEvtTimeGenTask
//      see CeeTpcEvtTimeGenTask.h for details
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
#include "CeeTpcEvtTimeGenTask.h"

// C/C++ Headers ----------------------


// Collaborating Class Headers --------
#include "FairRootManager.h"
#include "FairRun.h"
#include "TClonesArray.h"
#include "CeeTpcEvtTime.h"
#include "TRandom.h"
#include "TTree.h"
#include "TBranch.h"

#include <iostream>

// Class Member definitions -----------


CeeTpcEvtTimeGenTask::CeeTpcEvtTimeGenTask()
  : FairTask("TPC Background Event Timer"),
     fpersistence(kFALSE),
    fmeanEvtSpacing(100),
    ft0(0),feventid(1)
{}

CeeTpcEvtTimeGenTask::~CeeTpcEvtTimeGenTask()
{
}

InitStatus CeeTpcEvtTimeGenTask::Init()
{

  std::cout<< "CeeTpcEvtTimeGenTask::Init()" <<std::endl;

  //Get ROOT Manager
  FairRootManager* ioman= FairRootManager::Instance();

  if(ioman==0)
    {
      Error("Init","RootManager not instantiated!");
      return kERROR;
    }


  ftimeArray = new TClonesArray("CeeTpcEvtTime");
  ioman->Register("CeeTpcEvtTime","CeeTpc",ftimeArray,fpersistence);

  ftevent=ft0;

  return kSUCCESS;
}



void CeeTpcEvtTimeGenTask::Exec(Option_t* opt)
{
  std::cout<< "CeeTpcEvtTimeGenTask::Exec" << std::endl;

  // clean up fbkgArray;
  ftimeArray->Delete();

  ftevent+=gRandom->Exp(fmeanEvtSpacing);
  new ((*ftimeArray)[0]) CeeTpcEvtTime(ftevent,fmeanEvtSpacing,feventid++);

  return;
}

ClassImp(CeeTpcEvtTimeGenTask)
