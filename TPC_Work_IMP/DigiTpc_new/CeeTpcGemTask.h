//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      PndTpc Gem reads in MCPoints and produces primary electrons
//
//
// Environment:
//      Software developed for the PANDA Detector at FAIR.
//
// Author List:
//      Sebastian Neubert    TUM            (original author)
//
//
//-----------------------------------------------------------

#ifndef CEETPCGEMTASK_HH
#define CEETPCGEMTASK_HH

// Base Class Headers ----------------
#include "FairTask.h"
#include "TH1F.h"
#include "TFile.h"
// Collaborating Class Headers -------


// Collaborating Class Declarations --
class TClonesArray;


class CeeTpcGemTask : public FairTask {
public:

  // Constructors/Destructors ---------
  CeeTpcGemTask();
  ~CeeTpcGemTask();

  // Operators
  

  // Accessors -----------------------


  // Modifiers -----------------------
  void SetDriftedBranchName(const TString& name) {fdriftedBranchName=name;}
  void SetPersistence(Bool_t opt=kTRUE) {fpersistence=opt;}
  void SetGainFluctuations(Bool_t opt=kTRUE) {fgainFluctuations=opt;}
  void SetGain(Double_t gain){fgain = gain;}

  // Operations ----------------------
  
  virtual InitStatus Init();

  virtual void Exec(Option_t* opt);

private:

  // Private Data Members ------------
  TString fdriftedBranchName;
  TClonesArray* fdriftedArray;
  TClonesArray* favalancheArray;
  TH1F *hgain;
  TFile *file;
  
  Bool_t fpersistence;
  Bool_t fgainFluctuations;
  Double_t fgain;

  Bool_t finitialized;

  // Private Methods -----------------

public:
  ClassDef(CeeTpcGemTask,1)

};

#endif
