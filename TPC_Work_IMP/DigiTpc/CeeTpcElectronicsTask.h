//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      CeeTpc PSA: Pulse shape analysis
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

#ifndef CEETPCELECTRONICSTASK_HH
#define CEETPCELECTRONICSTASK_HH

// Base Class Headers ----------------
#include "FairTask.h"

// Collaborating Class Headers -------
#include <vector>

// Collaborating Class Declarations --
class TClonesArray;
class CeeTpcFrontend;
class CeeTpcDigiPar;
class TH1D;
class TH2D;
class TFile;
class QAPlotCollection;
class CeeTpcPadPlane;
class TF1;

class CeeTpcElectronicsTask : public FairTask {
public:

  // Constructors/Destructors ---------
  CeeTpcElectronicsTask();
  ~CeeTpcElectronicsTask();

  // Operators
  

  // Accessors -----------------------


  // Modifiers -----------------------
  void SetSignalBranchName(const TString& name) {fsignalBranchName=name;}
  void SetPersistence(Bool_t opt=kTRUE) {fpersistence=opt;}
  void SetADCThreshold (Int_t threshold){fADCTheshold = threshold;}

  // Operations ----------------------
  
  virtual InitStatus Init();

  virtual void Exec(Option_t* opt);

  virtual void SetParContainers();
  
  virtual void SetDebug(Bool_t debug){fdebug=debug;}

  void SetSignalRange(Float_t signalstart,Float_t signalstop){
	  fSignalStart = signalstart;
	  fSignalStop = signalstop;
  }

  void WriteHistograms();

private:

  unsigned int fCount;  //event counter
  
  // Private Data Members ------------
  TString fsignalBranchName;
  TClonesArray* fsignalArray;
  TClonesArray* fdigiArray;
  const CeeTpcFrontend* ffrontend;
  CeeTpcDigiPar* fpar;
  const CeeTpcPadPlane* fpadPlane;
  TFile *fSignalfile;
  TFile *fNoisefile;
  TH1D *hNoise;
  TH2D *hSig;
  TH2D *hsignal_timebin;
  TH1D *htimebin_proj;
  TH1D *htrack_proj;
  
  Bool_t fpersistence;
  Int_t fntimebins;
  Int_t ftimestep;
  Bool_t finitialized;
  Bool_t fdebug;
  Int_t fbinSignalStart;
  Int_t fbinSignalEnd;
  Int_t fSigBinNumber;
  Int_t fADCTheshold;
  Int_t fMaxPadsinRow;
  const Int_t fmaxmctrackId;
  Int_t fNoiseTimeBins;
  Float_t fSignalStart, fSignalStop;
  Float_t fPedestal; // Pedestal estimated by fitting a histogram with a constant

public:
  ClassDef(CeeTpcElectronicsTask,1)

};

#endif

//--------------------------------------------------------------
// $Log$
//--------------------------------------------------------------
