//-----------------------------------------------------------
// File and Version Information:
// $Id$
//
// Description:
//      CeeTpc PadResponse reads in MCPoints and produces primary electrons
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

#ifndef CEETPCPADRESPONSETASK_HH
#define CEETPCPADRESPONSETASK_HH

// Base Class Headers ----------------
#include "FairTask.h"
#include "TH1.h"
#include "TH2.h"

// Collaborating Class Headers -------
#include <map>

// Collaborating Class Declarations --
class TClonesArray;
class CeeTpcPadPlane;
class CeeTpcGem;
class QAPlotCollection;
class CeeTpcDigiPar;

class CeeTpcPadResponseTask : public FairTask {
public:

  // Constructors/Destructors ---------
  CeeTpcPadResponseTask();
  ~CeeTpcPadResponseTask();

  // Operators
  

  // Accessors -----------------------


  // Modifiers -----------------------
   void SetAvalancheBranchName(const TString& name) {favalancheBranchName=name;}
   void SetPersistence(Bool_t opt=kTRUE) {fpersistence=opt;}
   void SelectSector(unsigned int id){fsecids[id]=true;}
   void SetQAPlotCol(QAPlotCollection* col){fqa=col;}
   void SetDebug(Bool_t debug){fdebug=debug;}

  // Operations ----------------------
  
  virtual InitStatus Init();

  virtual void Exec(Option_t* opt);

  virtual void SetParContainers();
  
  void FillHistograms(double, double, double, double);
  void WriteHistograms();

private:

  // Private Data Members ------------
  TString favalancheBranchName;
  TClonesArray* favalancheArray;
  TClonesArray* fsignalArray;

  Bool_t fpersistence;
  Double_t fminSignalAmp;
  Double_t fMeanGemTime; // Mean time in the GEM read from tpc.par file
  Double_t fSigmaGemTime;// Sigma time in the GEM read from tpc.par file
  Bool_t fdebug;
  const CeeTpcPadPlane* fpadPlane;
  const CeeTpcGem* fgem;
  
  CeeTpcDigiPar* fpar;  

  Int_t feventcounter;

  TH1D* fxVariation;
  TH1D* fzVariation;
  TH2D* f2DHisto;

  std::map<unsigned int,bool> fsecids;
  bool fselected; // true if only selected sectors are processed

  Bool_t finitialized;
  QAPlotCollection* fqa;
  
  // Private Methods -----------------

public:
  ClassDef(CeeTpcPadResponseTask,1)

};

#endif

