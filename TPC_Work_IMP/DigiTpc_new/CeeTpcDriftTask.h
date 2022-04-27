//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      CeeTpc Drifter reads in CeeTpcPrimaryClusters and produces drifted electrons
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

#ifndef CEETPCDRIFTTASK_HH
#define CEETPCDRIFTTASK_HH

// Base Class Headers ----------------
#include "FairTask.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TVector3.h"
#include "QAPlotCollection.h"
// Collaborating Class Declarations --
class TClonesArray;
class CeeTpcGas;
class CeeTpcDigiPar;

class CeeTpcDriftTask : public FairTask {
public:

  // Constructors/Destructors ---------
  CeeTpcDriftTask();
  ~CeeTpcDriftTask();

  // Operators
  

  // Accessors -----------------------


  // Modifiers -----------------------
  void SetPrimBranchName(const TString& name) {fPrimBranchName=name;}
  void SetPersistence(Bool_t opt=kTRUE) {fPersistence=opt;}
  void SetAttach(Bool_t opt = kTRUE) {fAttach = opt;}
  void SetDiffuse(Bool_t opt = kTRUE) {fDiffuse = opt;}
  void SetDistort(Bool_t opt = kTRUE) {fDistort = opt;}
  void SetQAPlotCol(QAPlotCollection* col){fqa=col;}
  void SetUniformCharge(Bool_t opt = kFALSE){fUniform = opt;}

 

  // Operations ----------------------
  
  virtual InitStatus Init();
  virtual void Exec(Option_t* opt); 
  virtual void SetParContainers();
  // Distribuite Charge Uniformly in InPosition and OutPosition of TPC Step Volume
  TVector3 DistribuiteChargeUniform(TVector3 InPos, TVector3 OutPos);
  void FillHistograms(double, double, double, double, double, double, double, int);   //to keep track of the drifts
  void WriteHistograms();                        //has to be called in Digi macro




private:

  // Private Data Members ------------
  // Private Data Members ------------

  TString fPrimBranchName;
  TClonesArray* fPrimArray;
  TClonesArray* fDriftedArray;
  
  const CeeTpcGas* fGas;
  CeeTpcDigiPar* fpar;
  Double_t fyGem; // GeM layers will be in Y direction
  
  Bool_t fPersistence;
  Bool_t fAttach;
  Bool_t fDiffuse;
  Bool_t fDistort;
  
  QAPlotCollection* fqa;
  TH1D* fxVariation;
  TH1D* fzVariation;
  TH2D* fxVarAndDriftL;
  TH2D* fzVarAndDriftL;
  TH2D* fyVarAnddl;
  TH2D* fxzVar;
  TH1D* fCharge;
  TH1D* ftVariation;
  TH3D* fxzVarAnddl;
  TH2D* fxzCharge;
  TH2D* fyzCharge;
  Bool_t finitialized;
  Bool_t fUniform;
 


  // Private Methods -----------------

public:
  ClassDef(CeeTpcDriftTask,1)

};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------

