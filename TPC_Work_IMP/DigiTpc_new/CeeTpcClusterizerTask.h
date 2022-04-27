//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      CeeTpc Clusterizer reads in MCPoints and produces primary electrons
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

#ifndef CEETPCCLUSTERIZERTASK_HH
#define CEETPCCLUSTERIZERTASK_HH

// Base Class Headers ----------------
#include "FairTask.h"

// Collaborating Class Headers -------


// Collaborating Class Declarations --
class TClonesArray;
class CeeTpcGas;

class CeeTpcClusterizerTask : public FairTask {
public:

  // Constructors/Destructors ---------
  CeeTpcClusterizerTask();
  ~CeeTpcClusterizerTask();

  // Operators
  

  // Accessors -----------------------


  // Modifiers -----------------------
  void SetPointBranchName(const TString& name) {fpointBranchName=name;}
  void SetPersistence(Bool_t opt=kTRUE) {fpersistence=opt;}
  
    /**
  	if activeated there will be no Interpolation between hits
	This has to be set
	if the ALICE Monte Carlo is activeated in CeeTpcDetector
  */
  void SetMereChargeConversion(Bool_t opt=kTRUE) {fmereChargeConversion=opt;}
  void SetFirstPoti(Float_t pot) {fPoti=pot*1.e-9;} // (eV) e.g. for TestChamber

  // Operations ----------------------
  
  virtual InitStatus Init();

  virtual void Exec(Option_t* opt);
  
  void ChargeConversion();


private:

  // Private Data Members ------------
  
  
  TString fpointBranchName;
  TClonesArray* fpointArray;
  TClonesArray* fprimArray;
  
  const CeeTpcGas* fgas;
  Float_t fPoti; //first ionization potential, used in ALCIE charge conv.


  Bool_t fpersistence;
  Bool_t fmereChargeConversion;

  // Private Methods -----------------

public:
  ClassDef(CeeTpcClusterizerTask,1)

};

#endif
